#include "DN_Int.h"
#include "Application.h"

DN_Int::DN_Int(ImVec2 pos, BluePrint* bp) : BP_Node("Integer", BP_Node::TYPE::SIMPLE, pos, ImColor(68, 201, 156), bp)
{
	BP_Pin ask_pin(bp->unique_id++, "", BP_Pin::TYPE::None, bp, BP_Pin::BoxType::NUMBER);
	ask_pin.string_box = "";
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(bp->unique_id++, "Int", BP_Pin::TYPE::Int, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_Int::DN_Int(ImVec2 pos, BluePrint* bp, int ask_pin_id, int send_pin_id, std::string box) : BP_Node("Integer", BP_Node::TYPE::SIMPLE, pos, ImColor(68, 201, 156), bp)
{
	BP_Pin ask_pin(ask_pin_id, "", BP_Pin::TYPE::None, bp, BP_Pin::BoxType::NUMBER);
	ask_pin.string_box = box;
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(send_pin_id, "Int", BP_Pin::TYPE::Int, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_Int::~DN_Int()
{

}

void DN_Int::SaveInNode()
{
	info_as_number = (float)std::stoi(inputs[0].string_box);
}