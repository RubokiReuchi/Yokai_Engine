#include "DN_String.h"
#include "Application.h"
#include "ModuleInput.h"

DN_String::DN_String(ImVec2 pos, BluePrint* bp) : BP_Node("String", BP_Node::TYPE::SIMPLE, pos, ImColor(124, 21, 153), bp)
{
	BP_Pin key_pin(bp->unique_id++, "", BP_Pin::TYPE::Bool, bp, BP_Pin::BoxType::STRING);
	key_pin.string_box = "";
	key_pin.node = this;
	inputs.push_back(key_pin);
	BP_Pin send_pin(bp->unique_id++, "String", BP_Pin::TYPE::String, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_String::~DN_String()
{

}