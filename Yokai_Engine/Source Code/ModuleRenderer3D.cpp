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

#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "Glew/libx86/glew32.lib")

ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled),
vsync(true), wireframe(false), exit(false)
{
	depth_test = cull_face = lighting = color_material = texture_2d = true;
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	// Set Up OpenGL Attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

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
		ToggleVSync(vsync);

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);
		
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
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);

		// Enable opacity
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	model_render.Init();
	line_shader = new Re_Shader("Assets/shaders/lines.vertex.shader", "Assets/shaders/lines.fragment.shader");
	InitAABB_Buffer();

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
		app->camera->sceneCamera.frameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		app->camera->currentDrawingCamera = &app->camera->sceneCamera;
		drawing_scene = true;
		app->camera->currentDrawingCamera->CheckGoInCamera();

		app->engine_order->DrawEO();
		model_render.Draw();

		if (drawing_lines.size() > 0) DrawLines();
	}
	if (app->camera->activeGameCamera->active)
	{
		app->camera->activeGameCamera->frameBuffer.Bind();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		app->camera->currentDrawingCamera = app->camera->activeGameCamera;
		drawing_scene = false;

		app->engine_order->DrawEO();
		model_render.Draw();
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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

void ModuleRenderer3D::DrawLines()
{
	glUseProgram(line_shader->program_id);
	glUniformMatrix4fv(glGetUniformLocation(line_shader->program_id, "view"), 1, GL_FALSE, app->camera->currentDrawingCamera->GetViewMatrix());
	glUniformMatrix4fv(glGetUniformLocation(line_shader->program_id, "projection"), 1, GL_FALSE, app->camera->currentDrawingCamera->GetProjectionMatrix());
	glUniform4f(glGetUniformLocation(line_shader->program_id, "lineColor"), 0.75f, 0.36f, 0.32f, 1.0f);
	//Re_Mesh go_mesh = dynamic_cast<C_MeshRenderer*>(app->engine_order->editor->GetSelectedGameObject()->GetComponent(Component::TYPE::MESH_RENDERER))->GetMesh();
	//glUniformMatrix4fv(glGetUniformLocation(line_shader->program_id, "model"), 1, GL_FALSE, &go_mesh.model_matrix.v[0][0]);

	UpdateAABB_Buffer(drawing_lines);

	glBindVertexArray(linesVAO);
	glDrawArrays(GL_LINES, 0, drawing_lines.size());
	glBindVertexArray(0);

	drawing_lines.clear();
}

void ModuleRenderer3D::AddLines(std::vector<float3> lines)
{
	drawing_lines.insert(drawing_lines.end(), lines.begin(), lines.end());
}

void ModuleRenderer3D::InitAABB_Buffer()
{
	glGenVertexArrays(1, &linesVAO);
	glBindVertexArray(linesVAO);

	// Create Vertex Buffer Object
	glGenBuffers(1, &linesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float3) * MAX_LINES * 2, NULL, GL_DYNAMIC_DRAW);

	// vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float3), (void*)0);

	glBindVertexArray(0);
}

void ModuleRenderer3D::UpdateAABB_Buffer(std::vector<float3> lines)
{
	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float3) * lines.size(), &lines[0]);
}