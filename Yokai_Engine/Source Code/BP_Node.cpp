#include "BP_Node.h"

BP_Node::BP_Node(int id, const char* name, TYPE type, ImVec2 pos, ImColor color, BluePrint* bp) : id(id), name(name), type(type), color(color), position(pos), blue_print(bp)
{
	
}

BP_Node::~BP_Node()
{

}