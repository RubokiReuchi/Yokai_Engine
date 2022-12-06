#include "Blueprint.h"
#include "CoreNode.h"
#include "BlueprintHelpers.h"

BluePrint::BluePrint(std::string name) : name(name)
{
	
}

BluePrint::~BluePrint()
{
	for (auto& node : nodes) RELEASE(node);
	for (auto& pin : pins) RELEASE(pin);
	for (auto& link : links) RELEASE(link);
}

void BluePrint::Update(float dt)
{
	for (auto& node : nodes)
	{
		if (node->type != BP_Node::TYPE::CORE) continue;

		dynamic_cast<CoreNode*>(node)->Update(dt);
	}
}