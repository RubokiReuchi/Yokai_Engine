#include "Blueprint.h"
#include "BlueprintHelpers.h"

BluePrint::BluePrint(std::string name, GameObject* go) : name(name), go(go)
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
	if (!info_saved_in_nodes)
	{
		for (auto& node : nodes)
		{
			node->SaveInNode();
		}
		info_saved_in_nodes = true;
	}

	for (auto& node : nodes)
	{
		if (node->type == BP_Node::TYPE::CORE)  node->Update(dt);
	}
}