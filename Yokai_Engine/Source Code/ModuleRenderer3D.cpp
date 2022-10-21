#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SceneCamera.h"
#include "EO_Editor.h"
#include "OpenGL.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include"MathGeoLib/include/Geometry/Frustum.h"
#include"MathGeoLib/include/Geometry/Plane.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") /* link Microsoft OpenGL lib   */

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled),
vsync(true), wireframe(false), exit(false)
{
	depth_test = cull_face = lighting = /*color_material =*/ texture_2d = true;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(app->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum err = glewInit();

	SDL_GL_MakeCurrent(app->window->window, context);
	
	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(static_cast<int>(vsync)) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glEnable(GL_LIGHTING);
		lights[0].Active(true);
		//glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

		// Enable opacity
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	model_render.Init();

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// light 0 on cam pos
	lights[0].SetPos(app->camera->sceneCamera.Position.x, app->camera->sceneCamera.Position.y, app->camera->sceneCamera.Position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update(float dt)
{
	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	if (app->camera->sceneCamera.active)
	{
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf((GLfloat*)app->camera->sceneCamera.GetProjectionMatrix());

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)app->camera->sceneCamera.GetViewMatrix());

		app->camera->sceneCamera.frameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		app->camera->currentDrawingCamera = &app->camera->sceneCamera;

		app->engine_order->DrawEO();
		model_render.Draw();
	}
	if (app->camera->activeGameCamera->active)
	{
		glLoadIdentity();
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf((GLfloat*)app->camera->activeGameCamera->GetProjectionMatrix());

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf((GLfloat*)app->camera->activeGameCamera->GetViewMatrix());

		app->camera->activeGameCamera->frameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		app->camera->currentDrawingCamera = app->camera->activeGameCamera;

		app->engine_order->DrawEO();
		model_render.Draw();

		//app->camera->activeGameCamera->LookAt(app->engine_order->editor->GetSelectedGameObject()->transform->GetGlobalTransform().position);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	app->engine_order->DrawEO_Editor();

	// sets wireframe mode on/off
	(wireframe) ? glPolygonMode(GL_FRONT_AND_BACK, GL_LINE) : glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	SDL_GL_SwapWindow(app->window->window);

	if (exit)
	{
		return UPDATE_STOP;
	}
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	app->camera->RequestFrameBufferRegen(&app->camera->sceneCamera, width, height);

	app->window->width = width;
	app->window->height = height;
}

void ModuleRenderer3D::ToggleVSync(bool is_on)
{
	vsync = is_on;
	SDL_GL_SetSwapInterval(vsync);
}