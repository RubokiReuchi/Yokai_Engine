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
	GetGameObject()->GenerateFixedAABB();
}

C_Camera::~C_Camera()
{
}

void C_Camera::Update()
{
	float3 points[8];
	GetGameObject()->global_aabb.GetCornerPoints(points);

	std::vector<float3> lines = PointsToLines_AABB(points);

	app->renderer3D->AddLines(lines, float4(0.905f, 0.851f, 0.0f, 1.0f));
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

std::vector<float3> C_Camera::PointsToLines_AABB(float3 points[8])
{
	std::vector<float3> lines;

	// face 1
	lines.push_back(points[0]); // line 1
	lines.push_back(points[1]);
	lines.push_back(points[0]); // line 2
	lines.push_back(points[4]);
	lines.push_back(points[1]); // line 3
	lines.push_back(points[5]);
	lines.push_back(points[4]); // line 4
	lines.push_back(points[5]);

	// face 2
	lines.push_back(points[2]); // line 5
	lines.push_back(points[3]);
	lines.push_back(points[2]); // line 6
	lines.push_back(points[6]);
	lines.push_back(points[6]); // line 7
	lines.push_back(points[7]);
	lines.push_back(points[3]); // line 8
	lines.push_back(points[7]);

	// joints
	lines.push_back(points[0]); // line 9
	lines.push_back(points[2]);
	lines.push_back(points[1]); // line 10
	lines.push_back(points[3]);
	lines.push_back(points[4]); // line 11
	lines.push_back(points[6]);
	lines.push_back(points[5]); // line 12
	lines.push_back(points[7]);

	return lines;
}