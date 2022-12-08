#include "BP_Node.h"
#include "EO_Editor.h"

BP_Node::BP_Node(const char* name, TYPE type, ImVec2 pos, ImColor color, BluePrint* bp)
	: name(name), type(type), position(pos), color(color), blue_print(bp)
{
	id_as_int = blue_print->unique_id++;
	id = id_as_int;
}

BP_Node::~BP_Node()
{

}

void BP_Node::Update(float dt)
{

}

void BP_Node::Activate(float dt)
{

}