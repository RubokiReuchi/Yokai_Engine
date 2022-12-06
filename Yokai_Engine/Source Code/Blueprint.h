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

#include <string>
#include <vector>

using namespace ax::NodeEditor;
using namespace ax::Widgets;
using namespace ax::Drawing;

class BP_Node;
class BP_Link;

class BluePrint
{
    BluePrint(std::string name);
    ~BluePrint();

    void Update(float dt);

public:

    std::string name;

    std::vector<BP_Node*> nodes;
    std::vector<BP_Pin*> pins;
    std::vector<BP_Link*> links;

    friend class C_Blueprint;
};