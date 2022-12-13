#include "DN_MouseMotionY.h"
#include "Application.h"
#include "ModuleInput.h"

DN_MouseMotionY::DN_MouseMotionY(ImVec2 pos, BluePrint* bp) : BP_Node("Mouse Motion Y", BP_Node::TYPE::CORE, pos, ImColor(125, 0, 0), bp)
{
	y_movement = 0.0f;
	BP_Pin send_pin1(bp->unique_id++, "Y Movement", BP_Pin::TYPE::Float, bp);
	send_pin1.node = this;
	outputs.push_back(send_pin1);
	BP_Pin send_pin2(bp->unique_id++, "On Move", BP_Pin::TYPE::Executable, bp);
	send_pin2.node = this;
	outputs.push_back(send_pin2);
}

DN_MouseMotionY::DN_MouseMotionY(ImVec2 pos, BluePrint* bp, int send_pin1_id, int send_pin2_id) : BP_Node("Mouse Motion Y", BP_Node::TYPE::CORE, pos, ImColor(125, 0, 0), bp)
{
	y_movement = 0.0f;
	BP_Pin send_pin1(send_pin1_id, "Y Movement", BP_Pin::TYPE::Float, bp);
	send_pin1.node = this;
	outputs.push_back(send_pin1);
	BP_Pin send_pin2(send_pin2_id, "On Move", BP_Pin::TYPE::Executable, bp);
	send_pin2.node = this;
	outputs.push_back(send_pin2);
}

DN_MouseMotionY::~DN_MouseMotionY()
{

}

void DN_MouseMotionY::Update(float dt)
{
	y_movement = (float)app->input->GetMouseYMotion();

	if ((y_movement != 0.0f) && outputs[1].IsPinLinked())
	{
		for (auto& pin : outputs[1].GetOpositesPins())
		{
			pin->node->Activate(dt);
		}
	}

	info_as_number = y_movement;
}