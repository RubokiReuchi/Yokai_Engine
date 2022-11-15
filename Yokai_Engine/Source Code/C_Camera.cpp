#include "C_Camera.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "GameObject.h"

C_Camera::C_Camera(GameObject* gameObject) : Component(gameObject, Component::TYPE::CAMERA)
{
	cameraID = app->camera->game_cameras.size();

	lookingDir = Quat::identity;
	app->camera->game_cameras[cameraID].cameraFrustum.WorldMatrix().Decompose(float3(0, 0, 0), lookingDir, float3(0, 0, 0));
	original_lookingDir = lookingDir;
}

C_Camera::~C_Camera()
{
}

void C_Camera::OnEditor()
{
	ImGui::AlignTextToFramePadding();
	bool no_collapsed = ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap); ImGui::SameLine(ImGui::GetWindowWidth() - 22);
	if (ImGui::Button(ICON_FA_ELLIPSIS_VERTICAL "##Camera", ImVec2(20.0f, 0)))
	{
		popUpOpen = true;
		ImGui::OpenPopup("Component Options");
		ori = ImGui::GetMousePosOnOpeningCurrentPopup();
	}
	else
	{
		ImGui::CloseCurrentPopup();
	}
	if (no_collapsed)
	{
		ImVec2 gameDimensions = ImGui::GetContentRegionAvail();

		float aspect_ratio = 1.0f / app->camera->activeGameCamera->GetAspectRatio();
		ImGui::Image((ImTextureID)app->camera->activeGameCamera->frameBuffer.GetTexture(), ImVec2(gameDimensions.x, gameDimensions.x * aspect_ratio), ImVec2(0, 1), ImVec2(1, 0));
		
		float temp_fov = math::RadToDeg(GetCamera()->GetFOV());
		ImGui::Text("FOV: ");
		ImGui::SameLine();
		if (ImGui::DragFloat("##Fov", &temp_fov, 0.1f)) GetCamera()->SetFOV(temp_fov);
	}
	if (popUpOpen)
	{
		ComponentOptions();
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