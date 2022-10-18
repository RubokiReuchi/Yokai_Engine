#include "ModuleCamera3D.h"
#include "Application.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Geometry/Plane.h"
#include "MathGeoLib/include/Geometry/AABB.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	game_cameras.emplace_back();
	currentDrawingCamera = activeGameCamera = &game_cameras[0];
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	sceneCamera.frameBuffer.SetBufferInfo();
	sceneCamera.frameBuffer.SetDimensions(app->window->width, app->window->height);
	sceneCamera.changeFOVWithBufferSize = true;

	for (size_t i = 0; i < game_cameras.size(); i++)
	{
		game_cameras[i].frameBuffer.SetBufferInfo();
		game_cameras[i].frameBuffer.SetDimensions(app->window->width, app->window->height);
		game_cameras[i].changeFOVWithBufferSize = false;
	}

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (frameBufferRegenCamera != nullptr)
	{
		frameBufferRegenCamera->RegenerateFrameBuffer(newBufferWidth, newBufferHeight);
		frameBufferRegenCamera = nullptr;
	}

	if (updateSceneCamera) sceneCamera.UpdateCameraInput(dt);
	if (updateGameCamera) activeGameCamera->UpdateCameraInput(dt);

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::RequestFrameBufferRegen(Camera* camera, int width, int height)
{
	frameBufferRegenCamera = camera;
	newBufferWidth = width;
	newBufferHeight = height;
}