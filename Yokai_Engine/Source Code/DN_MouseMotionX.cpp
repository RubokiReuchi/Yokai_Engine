#include "DN_MouseMotionX.h"
#include "Application.h"
#include "ModuleInput.h"

DN_MouseMotionX::DN_MouseMotionX(ImVec2 pos, BluePrint* bp) : BP_Node("Mouse Motion X", BP_Node::TYPE::CORE, pos, ImColor(125, 0, 0), bp)
{
	x_movement = 0.0f;
	BP_Pin send_pin1(bp->unique_id++, "X Movement", BP_Pin::TYPE::Float, bp);
	send_pin1.node = this;
	outputs.push_back(send_pin1);
	BP_Pin send_pin2(bp->unique_id++, "On Move", BP_Pin::TYPE::Executable, bp);
	send_pin2.node = this;
	outputs.push_back(send_pin2);
}

DN_MouseMotionX::DN_MouseMotionX(ImVec2 pos, BluePrint* bp, int send_pin1_id, int send_pin2_id) : BP_Node("Mouse Motion X", BP_Node::TYPE::CORE, pos, ImColor(125, 0, 0), bp)
{
	x_movement = 0.0f;
	BP_Pin send_pin1(send_pin1_id, "X Movement", BP_Pin::TYPE::Float, bp);
	send_pin1.node = this;
	outputs.push_back(send_pin1);
	BP_Pin send_pin2(send_pin2_id, "On Move", BP_Pin::TYPE::Executable, bp);
	send_pin2.node = this;
	outputs.push_back(send_pin2);
}

DN_MouseMotionX::~DN_MouseMotionX()
{

}

void DN_MouseMotionX::Update(float dt)
{
	x_movement = (float)app->input->GetMouseXMotion();

	if ((x_movement != 0.0f) && outputs[1].IsPinLinked())
	{
		for (auto& pin : outputs[1].GetOpositesPins())
		{
			pin->node->Activate(dt);
		}
	}

	info_as_number = x_movement;
}