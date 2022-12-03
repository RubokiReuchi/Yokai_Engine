#include "C_Material.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "M_ModelRender.h"
#include "Re_Mesh.h"
#include "TextureImporter.h"
#include "M_Texture.h"

C_Material::C_Material(GameObject* gameObject) : Component(gameObject, TYPE::MATERIAL)
{
	if (gameObject->GetComponent(Component::TYPE::MESH_RENDERER) == NULL) return;
	renderer = dynamic_cast<C_MeshRenderer*>(gameObject->GetComponent(Component::TYPE::MESH_RENDERER));
}

C_Material::~C_Material()
{
	
}

void C_Material::OnEditor()
{
	ImGui::AlignTextToFramePadding();
	bool no_collapsed = ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap); ImGui::SameLine(ImGui::GetWindowWidth() - 22);
	if (ImGui::Button(ICON_FA_ELLIPSIS_VERTICAL "##Material", ImVec2(20.0f, 0)))
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
					ImGui::Text(loaded_tex.second.name.c_str());
					selected_texture = loaded_tex.second.name;
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

		std::string selected_texture_name;
		if (selected_texture != "Checkers" && selected_texture != "Default")
		{
			size_t npos = selected_texture.find_last_of("/") + 1;
			selected_texture_name = selected_texture;
			selected_texture_name = selected_texture_name.substr(npos);
			npos = selected_texture_name.find_last_of(".");
			selected_texture_name.erase(npos, 9);
		}
		else
		{
			selected_texture_name = selected_texture;
		}
		
		if (ImGui::BeginCombo("Select Texture", selected_texture_name.c_str(), ImGuiComboFlags_HeightSmall))
		{
			bool is_selected = (selected_texture == "Default");
			if (ImGui::Selectable("Default", is_selected))
			{
				selected_texture = "Default";
				if (is_selected) ImGui::SetItemDefaultFocus();
				GetMesh().texture_id = -1;
				GetMesh().GL_id = -1;
			}
			is_selected = (selected_texture == "Checkers");
			if (ImGui::Selectable("Checkers", is_selected))
			{
				selected_texture = "Checkers";
				if (is_selected) ImGui::SetItemDefaultFocus();
				GetMesh().texture_id = (float)TextureImporter::CreateTextureChecker();
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
				is_selected = (selected_texture_name == file_name);
				if (ImGui::Selectable(file_name.c_str(), is_selected))
				{
					selected_texture = texture.second.name;
					if (is_selected) ImGui::SetItemDefaultFocus();
					GetMesh().texture_id = (float)std::stoi(id_names[i]);
				}
				i++;
			}
			ImGui::EndCombo();
		}
		if (M_Texture::texture_image.find(selected_texture) != M_Texture::texture_image.end())
		{
			ImGui::Image((ImTextureID)M_Texture::texture_image[selected_texture], ImVec2(100, 100), ImVec2(0, 1), ImVec2(1, 0));
		}
	}
	if (popUpOpen)
	{
		ComponentOptions(true);
	}
}

void C_Material::SetTexture(std::string texture)
{
	if (texture == "Default")
	{
		renderer->GetMesh().texture_id = -1;
		selected_texture = texture;
	}
	else if(texture == "Checkers")
	{
		renderer->GetMesh().texture_id = (float)TextureImporter::CreateTextureChecker();
		selected_texture = texture;
	}
	else
	{
		renderer->GetMesh().texture_id = (float)TextureImporter::LoadTexture(texture);
		selected_texture = texture;
	}
}

Re_Mesh& C_Material::GetMesh()
{
	return renderer->GetMesh();
}
