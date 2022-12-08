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

    BP_Node(const char* name, TYPE type, ImVec2 pos, ImColor color, BluePrint* bp);
    virtual ~BP_Node();

    virtual void Update(float dt);

    virtual void Activate(float dt);

    // variables depends of the node content type
    std::string info_as_name;


public:
    bool init = false;
    NodeId id;
    std::string name;
    std::vector<BP_Pin> inputs;
    std::vector<BP_Pin> outputs;
    ImColor color;
    TYPE type;

    ImVec2 position;

    BluePrint* blue_print = nullptr;
};