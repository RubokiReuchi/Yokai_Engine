#include "DN_Translate.h"
#include "Application.h"

DN_Translate::DN_Translate(ImVec2 pos, BluePrint* bp) : BP_Node("Translate", BP_Node::TYPE::SIMPLE, pos, ImColor(255, 255, 0), bp)
{
	BP_Pin ask_pin1(bp->unique_id++, "Amount", BP_Pin::TYPE::Float, bp);
	ask_pin1.node = this;
	inputs.push_back(ask_pin1);
	BP_Pin ask_pin2(bp->unique_id++, "Direction", BP_Pin::TYPE::Vector3, bp);
	ask_pin2.node = this;
	inputs.push_back(ask_pin2);
	BP_Pin ask_pin3(bp->unique_id++, "Execute", BP_Pin::TYPE::Executable, bp);
	ask_pin3.node = this;
	inputs.push_back(ask_pin3);
}

DN_Translate::DN_Translate(ImVec2 pos, BluePrint* bp, int ask1_pin_id, int ask2_pin_id, int ask3_pin_id) : BP_Node("Translate", BP_Node::TYPE::SIMPLE, pos, ImColor(255, 255, 0), bp)
{
	BP_Pin ask_pin1(ask1_pin_id, "Amount", BP_Pin::TYPE::Float, bp);
	ask_pin1.node = this;
	inputs.push_back(ask_pin1);
	BP_Pin ask_pin2(ask2_pin_id, "Direction", BP_Pin::TYPE::Vector3, bp);
	ask_pin2.node = this;
	inputs.push_back(ask_pin2);
	BP_Pin ask_pin3(ask3_pin_id, "Execute", BP_Pin::TYPE::Executable, bp);
	ask_pin3.node = this;
	inputs.push_back(ask_pin3);
}

DN_Translate::~DN_Translate()
{

}

void DN_Translate::Activate(float dt)
{
	if (inputs[0].IsPinLinked())
	{
		distance = inputs[0].GetOpositePin()->node->info_as_number;
		direction = inputs[1].GetOpositePin()->node->info_as_vector3;

		blue_print->go->transform->Translate(direction * distance);
	}
}