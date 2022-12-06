#include "CoreNode.h"

CoreNode::CoreNode(int id, const char* name, ImVec2 pos, ImColor color, BluePrint* bp) : BP_Node(id, name, BP_Node::TYPE::CORE, pos, color, bp)
{

}

CoreNode::~CoreNode()
{

}

void CoreNode::Update(float dt)
{

}