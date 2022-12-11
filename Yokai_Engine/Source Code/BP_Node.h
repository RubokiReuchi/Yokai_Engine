#pragma once

#include "Globals.h"
#include "BP_Pin.h"

#include "ImGuiNodeEditor/imgui_node_editor.h"
#include "ImGuiNodeEditor/utilities/widgets.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <string>
#include <vector>

using namespace ax::NodeEditor;
using namespace ax::Widgets;
using namespace ax::Drawing;

class GameObject;
class BluePrint;

class BP_Node
{
public:
    enum class TYPE
    {
        CORE, // activate the worm
        SIMPLE,
        CONVERSOR, // variable change
        PRIORITY,
    };

    BP_Node(const char* name, TYPE type, ImVec2 pos, ImColor color, BluePrint* bp);
    BP_Node(const char* name, TYPE type, ImVec2 pos, ImColor color, BluePrint* bp, std::string info_as_name, float info_as_number, float3 info_as_vector3, bool info_as_boolean, GameObject* info_as_go);
    virtual ~BP_Node();

    virtual void Update(float dt);

    virtual void Activate(float dt);

    virtual void SaveInNode();

    // variables depends of the node content type
    std::string info_as_name = "";
    float info_as_number = 0;
    float3 info_as_vector3 = float3(0, 0, 0);
    bool info_as_boolean = true;
    GameObject* info_as_go = nullptr;

public:
    bool init = false;
    int id_as_int;
    NodeId id;
    std::string name;
    std::vector<BP_Pin> inputs;
    std::vector<BP_Pin> outputs;
    ImColor color;
    TYPE type;

    ImVec2 position;

    BluePrint* blue_print = nullptr;
};