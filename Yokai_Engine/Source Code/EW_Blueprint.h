#pragma once

#include "EditorWindow.h"
#include "BluePrint.h"

#include "ImGuiNodeEditor/imgui_node_editor.h"
#include "ImGui/imgui_internal.h"
#include "ImGuiNodeEditor/utilities/widgets.h"

namespace BP = ax::NodeEditor;

class GameObject;

class EW_Blueprint : public EditorWindow
{
public:
	EW_Blueprint();

	~EW_Blueprint();

	void Update() override;

private:

	BluePrint* current_blueprint = nullptr;
	bool blueprint_blocked = false;
	BP::EditorContext* context;
	int nextLinkId = 100;

	bool popUpOpen = false; //create node popUp
	ImVec2 ori;

	ImGuiTextFilter filter;
	void DisplayNodes();

	void FillNodeList();
	std::vector<std::string> node_list;
};