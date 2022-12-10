#pragma once

#include "Globals.h"
#include "BP_Node.h"
#include "BP_Pin.h"
#include "BP_Link.h"

#include "ImGuiNodeEditor/imgui_node_editor.h"
#include "ImGuiNodeEditor/utilities/widgets.h"

#include <string>

using namespace ax::NodeEditor;
using namespace ax::Widgets;
using namespace ax::Drawing;

class BP_Pin;

namespace NH
{
	// BP
	void BeginColumn();
	void NextColumn();
	void EndColumn();

	void HelpText(std::string text);

	bool CanLink(BP_Pin* a, BP_Pin* b);
	BP_Pin* GetPinByID(PinId id, BluePrint* bp);

	// WG
	ImColor GetIconColor(BP_Pin::TYPE type);
	void PinIcon(BP_Pin& pin, bool connected, float alpha = 255.0f);
}