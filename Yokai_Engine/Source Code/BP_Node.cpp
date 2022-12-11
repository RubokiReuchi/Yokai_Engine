#include "BP_Node.h"
#include "EO_Editor.h"

BP_Node::BP_Node(const char* name, TYPE type, ImVec2 pos, ImColor color, BluePrint* bp)
	: name(name), type(type), position(pos), color(color), blue_print(bp)
{
	id_as_int = blue_print->unique_id++;
	id = id_as_int;
}

BP_Node::BP_Node(const char* name, TYPE type, ImVec2 pos, ImColor color, BluePrint* bp, std::string info_as_name, float info_as_number, float3 info_as_vector3, bool info_as_boolean, GameObject* info_as_go)
	: name(name), type(type), position(pos), color(color), blue_print(bp), info_as_name(info_as_name), info_as_number(info_as_number), info_as_vector3(info_as_vector3), info_as_boolean(info_as_boolean), info_as_go(info_as_go)
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

void BP_Node::SaveInNode()
{

}