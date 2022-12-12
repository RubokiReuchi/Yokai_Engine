#include "C_Blueprint.h"
#include "Application.h"
#include "EO_Editor.h"
#include "Serialization.h"

C_Blueprint::C_Blueprint(GameObject* gameObject) : Component(gameObject, TYPE::BLUEPRINT)
{
	blueprint = new BluePrint("Default", gameObject);
}

C_Blueprint::~C_Blueprint()
{
	for (auto& node : blueprint->nodes)
	{
		RELEASE(node);
	}
	for (auto& link : blueprint->links)
	{
		RELEASE(link);
	}
}

void C_Blueprint::UpdateInGame(float dt)
{
	if (app->engine_order->editor->GetGameWindow()->window_hovered) blueprint->Update(dt);
}

void C_Blueprint::OnLoad()
{
	blueprint->OnLoad();
}

void C_Blueprint::OnEditor()
{
	ImGui::AlignTextToFramePadding();
	bool no_collapsed = ImGui::CollapsingHeader("Blueprint", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap); ImGui::SameLine(ImGui::GetWindowWidth() - 22);
	if (ImGui::Button(ICON_FA_ELLIPSIS_VERTICAL "##Blueprint", ImVec2(20.0f, 0)))
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
		ImGuiH::InputText("##Name", &blueprint->name);
	}
	if (popUpOpen)
	{
		ComponentOptions(false);
	}
}

void C_Blueprint::ExportBlueprint()
{
	std::string save_path = "Assets/Scripts/" + blueprint->name + ".ykbp";
	Serialization::SerializeBlueprint(blueprint, save_path);
}

void C_Blueprint::LoadBlueprint(std::string path)
{
	SerializedGO go;
	Serialization::DeSerializeBlueprint(&go, path);

	GetGameObject()->ProcessSerializedBlueprint(go, blueprint);
}