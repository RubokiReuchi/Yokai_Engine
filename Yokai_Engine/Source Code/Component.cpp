#include "Component.h"
#include "GameObject.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "EO_Editor.h"
#include "FileExplorer.h"

Component::Component(GameObject* game_object, TYPE type) : go(game_object), type(type)
{
}

Component::~Component()
{
}

void Component::ComponentOptions(bool undestructable)
{
	ImGui::SetNextWindowSize(ImVec2(200.0f, 100.0f));
	if (ImGui::BeginPopup("Component Options"))
	{
		ImGui::Text("Component Options");
		ImGui::Separator();
		if (this->GetType() == Component::TYPE::BLUEPRINT)
		{
			C_Blueprint* c_bp = dynamic_cast<C_Blueprint*>(this);
			if (c_bp->IsBlueprintEmpty())
			{
				if (ImGui::Selectable("Import Blueprint"))
				{
					app->engine_order->editor->file_explorer.OpenExplorer("Import As", Explorer::TYPE::LOAD, SCRIPTS_PATH);
				}
			}
			else
			{
				if (ImGui::Selectable("Reset Blueprint"))
				{
					c_bp->ResetBlueprint();
				}
				if (ImGui::Selectable("Export Blueprint"))
				{
					app->engine_order->editor->file_explorer.OpenExplorer("Export As", Explorer::TYPE::SAVE, SCRIPTS_PATH);
				}
			}
		}
		if (!undestructable)
		{
			if (ImGui::Selectable("Destroy Component"))
			{
				GetGameObject()->RemoveComponent(this);
			}
		}
		ImGui::EndPopup();
	}
	if (!ImGuiH::CheckMouseInPopUp(ImVec2(app->window->width - 200.0f, ori.y), ImVec2(200.0f, 100.0f)))
	{
		popUpOpen = false;
		ImGui::CloseCurrentPopup();
	}
}