#pragma once

#include "Globals.h"
#include "BP_Pin.h"

#include "ImGuiNodeEditor/imgui_node_editor.h"
#include "ImGuiNodeEditor/utilities/widgets.h"

#include <string>
#include <vector>

using namespace ax::NodeEditor;
using namespace ax::Widgets;
using namespace ax::Drawing;

class BluePrint;

class BP_Node
{
public:
    enum class TYPE
    {
        CORE, // activate the worm
        SIMPLE,
        CONVERSOR, // variable change
    };

    BP_Node(int id, const char* name, TYPE type, ImColor color = ImColor(255, 255, 255));
    virtual ~BP_Node();

public:
    NodeId id;
    std::string name;
    std::vector<BP_Pin> inputs;
    std::vector<BP_Pin> outputs;
    ImColor color;
    TYPE type;

    BluePrint* blue_print = nullptr;
};