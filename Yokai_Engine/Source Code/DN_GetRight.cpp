#include "DN_GetRight.h"
#include "Application.h"

DN_GetRight::DN_GetRight(ImVec2 pos, BluePrint* bp) : BP_Node("Get Right", BP_Node::TYPE::SIMPLE, pos, ImColor(0, 125, 125), bp)
{
	BP_Pin ask_pin(bp->unique_id++, "Game Object", BP_Pin::TYPE::Object, bp, BP_Pin::BoxType::GAMEOBJECT);
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(bp->unique_id++, "Right Vector", BP_Pin::TYPE::Vector3, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_GetRight::~DN_GetRight()
{

}

void DN_GetRight::SaveInNode()
{
	if (inputs[0].IsPinLinked())
	{
		info_as_vector3 = inputs[0].GetOpositePin()->node->info_as_go->transform->GetRight();
	}
	else
	{
		info_as_vector3 = inputs[0].go_box->transform->GetRight();
	}
}