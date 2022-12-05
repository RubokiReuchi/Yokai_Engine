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
};