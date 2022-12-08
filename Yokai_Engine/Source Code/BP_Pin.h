#pragma once

#include "Globals.h"
#include "Blueprint.h"
#include "BP_Node.h"

#include "ImGuiNodeEditor/imgui_node_editor.h"
#include "ImGuiNodeEditor/utilities/widgets.h"

#include <string>
#include <vector>

using namespace ax::NodeEditor;
using namespace ax::Widgets;
using namespace ax::Drawing;

class BP_Node;
class BluePrint;
class GameObject;

class BP_Pin
{
public:
    enum class TYPE
    {
        Executable,
        Bool,
        Int,
        Float,
        String,
        Object,
        Function,
        None
    };

    enum class BoxType
    {
        NONE,
        COMBO,
        STRING,
        GAMEOBJECT
    };

    BP_Pin(int id, const char* name, TYPE type, BluePrint* bp, BoxType box_type = BoxType::NONE);
    virtual ~BP_Pin();

    bool IsPinLinked();
    BP_Pin* GetOpositePin();

public:
    PinId id;
    std::string name;
    TYPE type;
    PinKind kind;

    ////// only if kind input
    BoxType box_type;
    std::vector<std::string> combo_box;
    std::string string_box;
    GameObject* go = nullptr;
    ////////

    BP_Node* node = nullptr;
    BluePrint* blue_print = nullptr;
};