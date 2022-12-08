#include "DN_Int.h"
#include "Application.h"
#include "ModuleInput.h"

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

DN_Int::~DN_Int()
{

}