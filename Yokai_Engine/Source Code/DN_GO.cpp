#include "DN_GO.h"
#include "Application.h"

DN_GO::DN_GO(ImVec2 pos, BluePrint* bp) : BP_Node("Game Object", BP_Node::TYPE::SIMPLE, pos, ImColor(51, 150, 215), bp)
{
	BP_Pin ask_pin(bp->unique_id++, "", BP_Pin::TYPE::None, bp, BP_Pin::BoxType::GAMEOBJECT);
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(bp->unique_id++, "GameObject", BP_Pin::TYPE::Object, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_GO::DN_GO(ImVec2 pos, BluePrint* bp, int ask_pin_id, int send_pin_id, std::string go_UUID) : BP_Node("Game Object", BP_Node::TYPE::SIMPLE, pos, ImColor(51, 150, 215), bp)
{
	BP_Pin ask_pin(ask_pin_id, "", BP_Pin::TYPE::None, bp, BP_Pin::BoxType::GAMEOBJECT);
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(send_pin_id, "GameObject", BP_Pin::TYPE::Object, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);

	serialized_go_UUID = go_UUID;
}

DN_GO::~DN_GO()
{

}

void DN_GO::SaveInNode()
{
	info_as_go = inputs[0].go_box;
}

void DN_GO::OnLoad()
{
	inputs[0].go_box = app->engine_order->GetGameObjectByUUID(serialized_go_UUID);
	serialized_go_UUID = "";
}