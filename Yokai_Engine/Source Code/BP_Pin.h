#pragma once

#include "Globals.h"
#include "Blueprint.h"

#include "ImGuiNodeEditor/imgui_node_editor.h"
#include "ImGuiNodeEditor/utilities/widgets.h"

#include <string>
#include <vector>

using namespace ax::NodeEditor;
using namespace ax::Widgets;
using namespace ax::Drawing;

enum class PinType
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

class BP_Pin
{
public:
    BP_Pin(int id, const char* name, PinType type, BluePrint* bp);
    virtual ~BP_Pin();

    bool IsPinLinked(PinId id);

public:
    PinId id;
    std::string name;
    PinType type;
    PinKind kind;

    BP_Node* node = nullptr;
    BluePrint* blue_print = nullptr;
};