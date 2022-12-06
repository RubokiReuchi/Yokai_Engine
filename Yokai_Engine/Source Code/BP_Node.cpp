#include "BP_Node.h"
#include "EO_Editor.h"
#include "EW_Blueprint.h"

BP_Node::BP_Node(int id, const char* name, TYPE type, ImVec2 pos, ImColor color, BluePrint* bp)
	: id(id), name(name), type(type), position(pos), color(color), blue_print(bp)
{
	// add to node list
	app->engine_order->editor->GetBlueprintWindow()->GetNodeList()[name] = this;
}

BP_Node::~BP_Node()
{

}

void BP_Node::Update(float dt)
{

}