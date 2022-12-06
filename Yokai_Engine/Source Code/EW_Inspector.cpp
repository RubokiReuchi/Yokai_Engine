#include "EW_Inspector.h"
#include "Application.h"
#include "EO_Editor.h"
#include "C_Camera.h"
#include "C_Transform.h"
#include "C_MeshRenderer.h"

EW_Inspector::EW_Inspector()
{
	window_name = "Inspector";
	enabled = true;

	editor = dynamic_cast<EO_Editor*>(app->engine_order->engine_order[(uint)EO_NUM::EDITOR]);
}

EW_Inspector::~EW_Inspector()
{
}

void EW_Inspector::Update()
{
	selectGameobject = editor->GetSelectedGameObject();
	// Inspector
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoCollapse);
	if (selectGameobject == NULL)
	{
		ImGui::End();
		return;
	}
	ImGui::Checkbox("##Enable", &selectGameobject->enabled);
	ImGui::SameLine();  ImGuiH::InputText("##Name", &selectGameobject->name);
	if (!selectGameobject->is_camera)
	{
		ImGui::SameLine();
		if (ImGui::Checkbox("Visible", &selectGameobject->visible))
		{
			SetVisible(selectGameobject, selectGameobject, selectGameobject->visible, true);
		}
	}

	if (editor->GetSelectedGameObject() != NULL)
	{
		if (editor->GetSelectedGameObject()->GetComponent(Component::TYPE::CAMERA) != NULL)
		{
			dynamic_cast<C_Camera*>(editor->GetSelectedGameObject()->GetComponent(Component::TYPE::CAMERA))->OnEditor();
		}
		if (editor->GetSelectedGameObject()->GetComponent(Component::TYPE::TRANSFORM) != NULL)
		{
			dynamic_cast<C_Transform*>(editor->GetSelectedGameObject()->GetComponent(Component::TYPE::TRANSFORM))->OnEditor();
		}
		if (editor->GetSelectedGameObject()->GetComponent(Component::TYPE::MESH_RENDERER) != NULL)
		{
			dynamic_cast<C_MeshRenderer*>(editor->GetSelectedGameObject()->GetComponent(Component::TYPE::MESH_RENDERER))->OnEditor();
		}
		if (editor->GetSelectedGameObject()->GetComponent(Component::TYPE::MATERIAL) != NULL)
		{
			dynamic_cast<C_Material*>(editor->GetSelectedGameObject()->GetComponent(Component::TYPE::MATERIAL))->OnEditor();
		}

		ImGui::Separator();
		if (ImGuiH::ButtonAlignOnLine("Add Component"))
		{
			popUpOpen = !popUpOpen;
			if (popUpOpen)
			{
				ImGui::OpenPopup("New Component");
				ori = ImGui::GetMousePosOnOpeningCurrentPopup();
			}
			else
			{
				ImGui::CloseCurrentPopup();
			}
		}

		if (popUpOpen)
		{
			ImGui::SetNextWindowSize(ImVec2(200.0f, 250.0f));
			if (ImGui::BeginPopup("New Component"))
			{
				DisplayComponents();
				ImGui::EndPopup();
			}
			if (!ImGuiH::CheckMouseInPopUp(ori, ImVec2(200.0f, 250.0f)))
			{
				popUpOpen = false;
				ImGui::CloseCurrentPopup();
			}
		}
	}
	ImGui::End();
}

void EW_Inspector::SetVisible(GameObject* selected_game_object, GameObject* game_object, bool visible, bool it_one)
{
	C_MeshRenderer* mr = dynamic_cast<C_MeshRenderer*>(game_object->GetComponent(Component::TYPE::MESH_RENDERER));
	if (mr != NULL && selected_game_object->GetParent()->visible)
	{
		if (game_object->visible || it_one) mr->GetMesh().visible = visible;
	}
	for (auto& childs : game_object->GetChilds())
	{
		SetVisible(selected_game_object, childs, visible, false);
	}
}

void EW_Inspector::DisplayComponents()
{
	ImGui::AlignTextToFramePadding();
	ImGui::Text(ICON_FA_MAGNIFYING_GLASS); ImGui::SameLine();
	filter.Draw("##Filter");
	std::string componentNames[numComponents - 3] = { "Camera", "Blueprint" };
	for (int i = 0; i < (numComponents - 3); i++)
	{
		std::string name = componentNames[i];
		if (filter.PassFilter(name.c_str()))
		{
			if (ImGui::Selectable(name.c_str()))
			{
				switch (i)
				{
				case 0: selectGameobject->AddComponent(Component::TYPE::CAMERA); break;
				case 1: selectGameobject->AddComponent(Component::TYPE::BLUEPRINT); break;
				}
			}
		}
	}
}