#include "C_Camera.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"

C_Camera::C_Camera(GameObject* gameObject) : Component(gameObject, Component::TYPE::CAMERA)
{
	cameraID = app->camera->game_cameras.size();
}

C_Camera::~C_Camera()
{
}

void C_Camera::OnPositionUpdate(float3 pos)
{
	app->camera->game_cameras[cameraID].cameraFrustum.pos = pos;
	app->camera->game_cameras[cameraID].CalculateViewMatrix();
}

void C_Camera::OnRotationUpdate(float3 rotation)
{
	float3 forward = GetGameObject()->transform->GetForward();
	float3 up = GetGameObject()->transform->GetUp();
	app->camera->game_cameras[cameraID].cameraFrustum.front = GetGameObject()->transform->GetForward();
	app->camera->game_cameras[cameraID].cameraFrustum.up = GetGameObject()->transform->GetUp();
	app->camera->game_cameras[cameraID].CalculateViewMatrix();
}

void C_Camera::OnTransformUpdate(float3 pos, float3 scale, float3 rotation)
{
	app->camera->game_cameras[cameraID].cameraFrustum.pos = pos;
	app->camera->game_cameras[cameraID].cameraFrustum.front = GetGameObject()->transform->GetForward();
	app->camera->game_cameras[cameraID].cameraFrustum.up = GetGameObject()->transform->GetUp();
	app->camera->game_cameras[cameraID].CalculateViewMatrix();
}

Camera* C_Camera::GetCamera()
{
	return &app->camera->game_cameras[cameraID];
}