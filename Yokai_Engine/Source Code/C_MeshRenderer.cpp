#include "C_MeshRenderer.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "M_ModelRender.h"
#include "Re_Mesh.h"

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
		std::vector<std::string> id_names;
		for (auto& loaded_tex : M_Texture::loaded_textures)
		{
			id_names.push_back(std::to_string(loaded_tex.first));
		}
		
		if (ImGui::BeginCombo("Select Texture", selected_texture.c_str(), ImGuiComboFlags_HeightSmall))
		{
			bool is_selected = (selected_texture == "Default");
			if (ImGui::Selectable("Default", is_selected))
			{
				selected_texture = "Default";
				if (is_selected) ImGui::SetItemDefaultFocus();
				GetMesh().texture_id = -1;
				GetMesh().OpenGL_texture_id = -1;
			}
			int i = 0;
			for (auto& texture : M_Texture::loaded_textures)
			{
				size_t npos = texture.second.name.find_last_of("/") + 1;
				std::string file_name = texture.second.name;
				file_name = file_name.substr(npos);
				npos = file_name.find_last_of(".");
				file_name.erase(npos, 9);
				is_selected = (selected_texture == file_name);
				if (ImGui::Selectable(file_name.c_str(), is_selected))
				{
					selected_texture = file_name;
					if (is_selected) ImGui::SetItemDefaultFocus();
					GetMesh().texture_id = std::stoi(id_names[i]);
				}
				i++;
			}
			ImGui::EndCombo();
		}
	}
	if (popUpOpen)
	{
		ComponentOptions(true);
	}
}

void C_MeshRenderer::InitAsLoadedMesh(uint mesh_id)
{
	this->mesh_id = mesh_id + 2;

	M_Render* manager = app->renderer3D->model_render.GetRenderManager(this->mesh_id);

	Re_Mesh instanceMesh;
	instanceMesh.InitAsMeshInformation({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });

	instance_id = manager->AddMesh(instanceMesh);
}

void C_MeshRenderer::InitAsNewMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
	Re_Mesh newMesh;
	newMesh.InitAsMesh(vertices, indices, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });

	mesh_id = app->renderer3D->model_render.GetMapSize() + 2;
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id); // Create a M_Render

	instance_id = manager->SetMeshInformation(newMesh);
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
