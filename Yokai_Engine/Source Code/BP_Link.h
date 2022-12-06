#pragma once

#include "Globals.h"

#include "ImGuiNodeEditor/imgui_node_editor.h"
#include "ImGuiNodeEditor/utilities/widgets.h"

#include <string>
#include <vector>

using namespace ax::NodeEditor;
using namespace ax::Widgets;
using namespace ax::Drawing;

class BluePrint;

class BP_Link
{
public:
    BP_Link(LinkId id, PinId input_id, PinId output_id, ImColor color, BluePrint* bp);
    virtual ~BP_Link();

public:
    LinkId id;
    PinId input_id;
    PinId output_id;
    ImColor color;

    BluePrint* blue_print = nullptr;
};