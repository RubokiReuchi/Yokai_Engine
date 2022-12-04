#pragma once

#include "Globals.h"
#include "BlueprintDefines.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
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

	bool IsPinLinked(PinId id, ImVector<LinkInfo> links);
	Pin GetPinByID(PinId id);
	bool CanLink(Pin a, Pin b);

	// WG
	ImColor GetIconColor(PinType type);
	void PinIcon(Pin& pin, bool connected, int alpha);
}