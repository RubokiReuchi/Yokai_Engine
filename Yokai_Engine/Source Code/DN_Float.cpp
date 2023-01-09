#include "DN_Float.h"
#include "Application.h"

DN_Float::DN_Float(ImVec2 pos, BluePrint* bp) : BP_Node("Float", BP_Node::TYPE::SIMPLE, pos, ImColor(147, 226, 74), bp)
{
	BP_Pin ask_pin(bp->unique_id++, "", BP_Pin::TYPE::None, bp, BP_Pin::BoxType::NUMBER);
	ask_pin.string_box = "";
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(bp->unique_id++, "Float", BP_Pin::TYPE::Float, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_Float::DN_Float(ImVec2 pos, BluePrint* bp, int ask_pin_id, int send_pin_id, std::string box) : BP_Node("Float", BP_Node::TYPE::SIMPLE, pos, ImColor(147, 226, 74), bp)
{
	BP_Pin ask_pin(ask_pin_id, "", BP_Pin::TYPE::None, bp, BP_Pin::BoxType::NUMBER);
	ask_pin.string_box = box;
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(send_pin_id, "Float", BP_Pin::TYPE::Float, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_Float::~DN_Float()
{

}

void DN_Float::SaveInNode()
{
	info_as_number = std::stof(inputs[0].string_box);
}