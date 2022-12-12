#pragma once

#include "Globals.h"
#include "BP_Node.h"
#include "BP_Pin.h"
#include "BP_Link.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGuiNodeEditor/imgui_node_editor.h"
#include "ImGuiNodeEditor/utilities/widgets.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <string>
#include <vector>

using namespace ax::NodeEditor;
using namespace ax::Widgets;
using namespace ax::Drawing;

class GameObject;

class BP_Node;
class BP_Link;

class BluePrint
{
public:
    BluePrint(std::string name, GameObject* go);
    ~BluePrint();

    void Update(float dt);
    void OnLoad();

    void CreateNode(std::string node_name, ImVec2 pos);
    void CreateNode(std::string node_name, ImVec2 pos, std::vector<int> inputs_id, std::vector<std::string> input_box, std::vector<int> outputs_id, std::string info_as_name,
        float info_as_number, float3 info_as_vector3, bool info_as_boolean, std::string go_UUID);

    void CreateLink(int id, int input_id, int output_id, float3 color);

public:

    int unique_id = 1;
    std::string name;

    std::vector<BP_Node*> nodes;
    std::vector<BP_Pin*> pins;
    std::vector<BP_Link*> links;

    bool info_saved_in_nodes = false;

    GameObject* go = nullptr;

    friend class C_Blueprint;
};