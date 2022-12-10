#include "DN_String.h"
#include "Application.h"

DN_String::DN_String(ImVec2 pos, BluePrint* bp) : BP_Node("String", BP_Node::TYPE::SIMPLE, pos, ImColor(124, 21, 153), bp)
{
	BP_Pin ask_pin(bp->unique_id++, "", BP_Pin::TYPE::None, bp, BP_Pin::BoxType::STRING);
	ask_pin.string_box = "";
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(bp->unique_id++, "String", BP_Pin::TYPE::String, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_String::~DN_String()
{

}

void DN_String::SaveInNode()
{
	info_as_name = inputs[0].string_box;
}