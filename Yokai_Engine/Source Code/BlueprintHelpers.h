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

namespace NH
{
	// BP
	void BeginColumn();
	void NextColumn();
	void EndColumn();

	bool CanLink(BP_Pin a, BP_Pin b);

	// WG
	ImColor GetIconColor(PinType type);
	void PinIcon(BP_Pin& pin, bool connected, int alpha);
}