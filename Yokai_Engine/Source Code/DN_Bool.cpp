#include "DN_Bool.h"
#include "Application.h"

DN_Bool::DN_Bool(ImVec2 pos, BluePrint* bp) : BP_Node("Boolean", BP_Node::TYPE::SIMPLE, pos, ImColor(220, 48, 48), bp)
{
	BP_Pin ask_pin(bp->unique_id++, "", BP_Pin::TYPE::None, bp, BP_Pin::BoxType::COMBO);
	std::string combo_options[2] = { "True", "False" };
	ask_pin.combo_box.insert(ask_pin.combo_box.begin(), std::begin(combo_options), std::end(combo_options));
	ask_pin.string_box = "True";
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(bp->unique_id++, "Bool", BP_Pin::TYPE::Bool, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_Bool::~DN_Bool()
{

}

void DN_Bool::SaveInNode()
{
	if (inputs[0].string_box == "True")
	{
		info_as_boolean = true;
	}
	else
	{
		info_as_boolean = false;;
	}
}