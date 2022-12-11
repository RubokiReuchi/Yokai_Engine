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
    BP_Link(int id, int input_id, int output_id, ImColor color, BluePrint* bp);
    virtual ~BP_Link();

public:
    int id_as_int;
    LinkId id;
    int input_id_as_int;
    PinId input_id;
    int output_id_as_int;
    PinId output_id;
    ImColor color;

    BluePrint* blue_print = nullptr;
};