#include "ModuleCamera3D.h"
#include "Application.h"
#include "C_Camera.h"
#include "EO_Editor.h"

#include "MathGeoLib/include/Math/float3.h"
#include "MathGeoLib/include/Geometry/Plane.h"
#include "MathGeoLib/include/Geometry/AABB.h"

ModuleCamera3D::ModuleCamera3D(bool start_enabled) : Module(start_enabled)
{
	newBufferWidth = 0;
	newBufferHeight = 0;
}

ModuleCamera3D::~ModuleCamera3D()
{}

bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");

	// Create GameObject with camera and transform components
	GameObject* main_camera = new GameObject(app->engine_order->rootGameObject, "Main Camera", "Camera", true);
	C_Camera* camera = dynamic_cast<C_Camera*>(main_camera->AddComponent(Component::TYPE::CAMERA));
	dynamic_cast<C_Transform*>(main_camera->GetComponent(Component::TYPE::TRANSFORM))->SetTransform(float3(0.0f, 5.0f, -9.5f), float3(1, 1, 1), float3(8, 0, 0));
	currentDrawingCamera = activeGameCamera = camera->GetCamera();

	sceneCamera.frameBuffer.SetBufferInfo();
	sceneCamera.frameBuffer.SetDimensions(app->window->width, app->window->height);
	sceneCamera.changeFOVWithBufferSize = true;

	for (auto& camera : game_cameras)
	{
		camera.second.frameBuffer.SetBufferInfo();
		camera.second.frameBuffer.SetDimensions(app->window->width, app->window->height);
		camera.second.changeFOVWithBufferSize = false;
		camera.second.ChangeAspectRatio(16.0f / 9.0f);
	}

	return true;
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

	if (updateSceneCamera)
	{
		sceneCamera.UpdateCameraInput(dt);
		if (click_state != 0)
		{
			sceneCamera.CalculateMousePicking();
			click_state = 0;
		}
	}

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::RequestFrameBufferRegen(Camera* camera, int width, int height)
{
	frameBufferRegenCamera = camera;
	newBufferWidth = width;
	newBufferHeight = height;
}

void ModuleCamera3D::InitNewGameCamera(GameObject* go)
{
	C_Camera* camera = dynamic_cast<C_Camera*>(go->AddComponent(Component::TYPE::CAMERA));
	currentDrawingCamera = activeGameCamera = camera->GetCamera();
	app->engine_order->editor->GetGameWindow()->SetCurrentGameCamera(activeGameCamera);

	camera->GetCamera()->frameBuffer.SetBufferInfo();
	camera->GetCamera()->frameBuffer.SetDimensions(app->window->width, app->window->height);
	camera->GetCamera()->changeFOVWithBufferSize = false;
}