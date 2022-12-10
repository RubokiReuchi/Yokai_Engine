#include "DN_GetUp.h"
#include "Application.h"

DN_GetUp::DN_GetUp(ImVec2 pos, BluePrint* bp) : BP_Node("Get Up", BP_Node::TYPE::SIMPLE, pos, ImColor(0, 125, 125), bp)
{
	BP_Pin ask_pin(bp->unique_id++, "Game Object", BP_Pin::TYPE::Object, bp, BP_Pin::BoxType::GAMEOBJECT);
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(bp->unique_id++, "Up Vector", BP_Pin::TYPE::Vector3, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_GetUp::~DN_GetUp()
{

}

void DN_GetUp::SaveInNode()
{
	if (inputs[0].IsPinLinked())
	{
		info_as_vector3 = inputs[0].GetOpositePin()->node->info_as_go->transform->GetUp();
	}
	else
	{
		info_as_vector3 = inputs[0].go_box->transform->GetUp();
	}
}