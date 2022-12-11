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

DN_GO::~DN_GO()
{

}

void DN_GO::SaveInNode()
{
	info_as_go = inputs[0].go_box;
}