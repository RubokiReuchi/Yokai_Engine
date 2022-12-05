#pragma once

#include "Globals.h"

#include "ImGuiNodeEditor/imgui_node_editor.h"
#include "ImGuiNodeEditor/utilities/widgets.h"

#include <string>
#include <vector>

using namespace ax::NodeEditor;
using namespace ax::Widgets;
using namespace ax::Drawing;

class BP_Pin;
class BluePrint;

enum class NodeType
{
    CORE, // activate the worm
    SIMPLE,
    CONVERSOR, // variable change
};

class BP_Node
{
public:
    BP_Node(int id, const char* name, NodeType type, ImColor color = ImColor(255, 255, 255));
    virtual ~BP_Node();

public:
    NodeId id;
    std::string name;
    std::vector<BP_Pin> inputs;
    std::vector<BP_Pin> outputs;
    ImColor color;
    NodeType type;

    BluePrint* blue_print = nullptr;
};