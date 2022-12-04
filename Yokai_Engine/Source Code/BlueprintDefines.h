#pragma once

#include "ImGuiNodeEditor/imgui_node_editor.h"
#include "ImGuiNodeEditor/utilities/widgets.h"

#include <string>
#include <vector>

using namespace ax::NodeEditor;
using namespace ax::Widgets;
using namespace ax::Drawing;

enum class PinType
{
    Flow,
    Bool,
    Int,
    Float,
    String,
    Object,
    Function,
    Delegate,
    None
};

enum class NodeType
{
    Blueprint,
    Simple,
    Tree,
    Comment,
    Houdini
};

struct Pin;
struct Node;

class BluePrint
{
public:
    static ImVector<Pin*> pins;

};

struct Pin
{
    PinId id;
    Node* node; // node where pin is inside
    std::string name;
    PinType type;
    PinKind kind;

    Pin(int id, const char* name, PinType type);
};

struct Node
{
    NodeId id;
    std::string name;
    std::vector<Pin> inputs;
    std::vector<Pin> outputs;
    ImColor color;
    NodeType type;
    ImVec2 size;

    std::string state;
    std::string savedState;

    Node(int id, const char* name, ImColor color = ImColor(255, 255, 255)) : id(id), name(name), color(color), type(NodeType::Blueprint), size(0, 0) {}
};

struct LinkInfo
{
    LinkId id;
    PinId input_id;
    PinId output_id;

    ImColor color;

    LinkInfo(LinkId id, PinId input_id, PinId output_id, PinType type);
};