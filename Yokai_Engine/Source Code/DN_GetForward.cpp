#include "DN_GetForward.h"
#include "Application.h"

DN_GetForward::DN_GetForward(ImVec2 pos, BluePrint* bp) : BP_Node("Get Forward", BP_Node::TYPE::PRIORITY, pos, ImColor(0, 125, 125), bp)
{
	BP_Pin ask_pin(bp->unique_id++, "Game Object", BP_Pin::TYPE::Object, bp, BP_Pin::BoxType::GAMEOBJECT);
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(bp->unique_id++, "Forward Vector", BP_Pin::TYPE::Vector3, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_GetForward::DN_GetForward(ImVec2 pos, BluePrint* bp, int ask_pin_id, int send_pin_id, int go_ID) : BP_Node("Get Forward", BP_Node::TYPE::PRIORITY, pos, ImColor(0, 125, 125), bp)
{
	BP_Pin ask_pin(ask_pin_id, "Game Object", BP_Pin::TYPE::Object, bp, BP_Pin::BoxType::GAMEOBJECT);
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(send_pin_id, "Forward Vector", BP_Pin::TYPE::Vector3, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);

	serialized_go_ID = go_ID;
}

DN_GetForward::~DN_GetForward()
{

}

void DN_GetForward::Update(float dt)
{
	if (inputs[0].IsPinLinked())
	{
		info_as_vector3 = inputs[0].GetOpositePin()->node->info_as_go->transform->GetForward();
	}
	else
	{
		info_as_vector3 = inputs[0].go_box->transform->GetForward();
	}
}

void DN_GetForward::OnLoad()
{
	if (serialized_go_ID == -1) return;
	inputs[0].go_box = app->engine_order->game_objects[serialized_go_ID];
	serialized_go_ID = -1;
}