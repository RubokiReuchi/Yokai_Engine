#include "C_MeshRenderer.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "M_ModelRender.h"
#include "Re_Mesh.h"
#include "TextureImporter.h"

C_MeshRenderer::C_MeshRenderer(GameObject* gameObject) : Component(gameObject, TYPE::MESH_RENDERER)
{
	mesh_id = 0;
	instance_id = 0;
}

C_MeshRenderer::~C_MeshRenderer()
{
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id);
	manager->GetMap().erase(instance_id);
}

void C_MeshRenderer::OnEditor()
{
	ImGui::AlignTextToFramePadding();
	bool no_collapsed = ImGui::CollapsingHeader("Mesh Render", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap); ImGui::SameLine(ImGui::GetWindowWidth() - 22);
	if (ImGui::Button(ICON_FA_ELLIPSIS_VERTICAL "##MeshRender", ImVec2(20.0f, 0)))
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
		ImGui::Text("Visible: "), ImGui::SameLine();
		if (GetMesh().visible) 
			ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.0f, 1.0f), "True");
		else ImGui::TextColored(ImVec4(0.9f, 0.0f, 0.0f, 1.0f), "False");
	}
	if (popUpOpen)
	{
		ComponentOptions(true);
	}
}

void C_MeshRenderer::InitAsInstanciedMesh(uint mesh_id)
{
	this->mesh_id = mesh_id + 2;

	M_Render* manager = app->renderer3D->model_render.GetRenderManager(this->mesh_id);

	Re_Mesh instanceMesh;
	instanceMesh.InitMeshTransform({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });

	instance_id = manager->AddMesh(instanceMesh);
}

void C_MeshRenderer::InitAsNewMesh(std::vector<VertexInfo>& vertices, std::vector<uint>& indices)
{
	Re_Mesh newMesh;
	newMesh.InitMeshInfo(vertices, indices, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });

	mesh_id = app->renderer3D->model_render.GetMapSize() + 2;
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id); // Create a M_Render

	instance_id = manager->InitManageRender(newMesh);
}

void C_MeshRenderer::OnPositionUpdate(float3 position)
{
	GetMesh().SetPosition(position);
}

void C_MeshRenderer::OnScaleUpdate(float3 scale)
{
	GetMesh().SetScale(scale);
}

void C_MeshRenderer::OnRotationUpdate(float3 rotation)
{
	GetMesh().SetRotation(rotation);
}

void C_MeshRenderer::OnTransformUpdate(float3 position, float3 scale, float3 rotation)
{
	GetMesh().SetTransform(position, scale, rotation);
}

Re_Mesh& C_MeshRenderer::GetMesh()
{
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id);

	Re_Mesh& meshReference = manager->GetMap()[instance_id];

	return meshReference;
}
