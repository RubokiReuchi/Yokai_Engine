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
		ImGui::Text("Texture Path: "); ImGui::SameLine();
		std::vector<std::string> id_names;
		for (auto& loaded_tex : M_Texture::loaded_textures)
		{
			id_names.push_back(std::to_string(loaded_tex.first));
			if (GetMesh().texture_id > 0 && GetMesh().texture_id == loaded_tex.first)
			{
				if (loaded_tex.second.name != "Checkers")
				{
					size_t npos = loaded_tex.second.name.find_last_of("/") + 1;
					std::string file_name = loaded_tex.second.name;
					ImGui::Text(loaded_tex.second.name.c_str());
					file_name = file_name.substr(npos);
					npos = file_name.find_last_of(".");
					file_name.erase(npos, 9);
					selected_texture = file_name;
				}
				else
				{
					ImGui::Text("NULL");
					selected_texture = "Checkers";
				}
			}
		}
		if (GetMesh().texture_id == -1)
		{
			ImGui::Text("NULL");
			selected_texture = "Default";

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
			is_selected = (selected_texture == "Checkers");
			if (ImGui::Selectable("Checkers", is_selected))
			{
				selected_texture = "Checkers";
				if (is_selected) ImGui::SetItemDefaultFocus();
				GetMesh().texture_id = (float)TextureImporter::CheckerImage();
			}
			int i = 0;
			for (auto& texture : M_Texture::loaded_textures)
			{
				if (texture.second.name == "Checkers") { i++; continue; }
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
					GetMesh().texture_id = (float)std::stoi(id_names[i]);
				}
				i++;
			}
			ImGui::EndCombo();
		}

		ImGui::Text("Visible: "), ImGui::SameLine();
		if (GetMesh().visible) ImGui::Text("True");
		else ImGui::Text("False");

		std::string display_options[] = {"None", "Vertex Normals", "Face Normals"};
		if (ImGui::BeginCombo("Display Normals", display_options[selected_normal].c_str(), ImGuiComboFlags_HeightSmall))
		{
			for (size_t i = 0; i < 3; i++)
			{
				bool is_selected = (selected_normal == i);
				if (ImGui::Selectable(display_options[i].c_str(), is_selected))
				{
					selected_normal = i;
					if (is_selected) ImGui::SetItemDefaultFocus();
					GetMesh().show_normals = selected_normal;
				}
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
