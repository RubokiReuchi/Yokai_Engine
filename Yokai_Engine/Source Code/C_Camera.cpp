#include "C_Camera.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"

C_Camera::C_Camera(GameObject* gameObject) : Component(gameObject, Component::TYPE::CAMERA)
{
	cameraID = app->camera->game_cameras.size();

	float3 empty = { 0,0,0 };
	lookingDir = Quat::identity;
	app->camera->game_cameras[cameraID].cameraFrustum.WorldMatrix().Decompose(empty, lookingDir, empty);
	original_lookingDir = lookingDir;
}

C_Camera::~C_Camera()
{
}

void C_Camera::OnEditor()
{
	if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImVec2 gameDimensions = ImGui::GetContentRegionAvail();

		float aspect_ratio = 1.0f / app->camera->activeGameCamera->GetAspectRatio();
		ImGui::Image((ImTextureID)app->camera->activeGameCamera->frameBuffer.GetTexture(), ImVec2(gameDimensions.x, gameDimensions.x * aspect_ratio), ImVec2(0, 1), ImVec2(1, 0));
	}
}

void C_Camera::OnPositionUpdate(float3 pos)
{
	app->camera->game_cameras[cameraID].cameraFrustum.pos = pos;
	app->camera->game_cameras[cameraID].CalculateViewMatrix();
}

void C_Camera::OnRotationUpdate(float3 rotation)
{
	Quat rotx = Quat::identity;
	rotx.SetFromAxisAngle({ 1.0f, 0.0f, 0.0f }, math::DegToRad(rotation.x));
	Quat roty = Quat::identity;
	roty.SetFromAxisAngle({ 0.0f, 1.0f, 0.0f }, math::DegToRad(rotation.y));
	Quat rotz = Quat::identity;
	rotz.SetFromAxisAngle({ 0.0f, 0.0f, 1.0f }, math::DegToRad(rotation.z));

	lookingDir = original_lookingDir * rotx * roty * rotz;

	float4x4 newWorldMatrix = app->camera->game_cameras[cameraID].cameraFrustum.WorldMatrix();
	newWorldMatrix.SetRotatePart(lookingDir.Normalized());
	app->camera->game_cameras[cameraID].cameraFrustum.SetWorldMatrix(newWorldMatrix.Float3x4Part());
}

void C_Camera::OnTransformUpdate(float3 pos, float3 scale, float3 rotation)
{
	app->camera->game_cameras[cameraID].cameraFrustum.pos = pos;
	app->camera->game_cameras[cameraID].CalculateViewMatrix();

	Quat rotx = Quat::identity;
	rotx.SetFromAxisAngle({ 1.0f, 0.0f, 0.0f }, math::DegToRad(rotation.x));
	Quat roty = Quat::identity;
	roty.SetFromAxisAngle({ 0.0f, 1.0f, 0.0f }, math::DegToRad(rotation.y));
	Quat rotz = Quat::identity;
	rotz.SetFromAxisAngle({ 0.0f, 0.0f, 1.0f }, math::DegToRad(rotation.z));

	lookingDir = original_lookingDir * rotx * roty * rotz;

	float4x4 newWorldMatrix = app->camera->game_cameras[cameraID].cameraFrustum.WorldMatrix();
	newWorldMatrix.SetRotatePart(lookingDir.Normalized());
	app->camera->game_cameras[cameraID].cameraFrustum.SetWorldMatrix(newWorldMatrix.Float3x4Part());
}

Camera* C_Camera::GetCamera()
{
	return &app->camera->game_cameras[cameraID];
}