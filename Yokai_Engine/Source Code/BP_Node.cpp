#include "BP_Node.h"

BP_Node::BP_Node(int id, const char* name, TYPE type, ImVec2 pos, ImColor color) : id(id), name(name), type(type), color(color), position(pos)
{
	
}

BP_Node::~BP_Node()
{

}