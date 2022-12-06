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

    BP_Pin(int id, const char* name, TYPE type, BluePrint* bp);
    virtual ~BP_Pin();

    bool IsPinLinked();

public:
    PinId id;
    std::string name;
    TYPE type;
    PinKind kind;

    BP_Node* node = nullptr;
    BluePrint* blue_print = nullptr;
};