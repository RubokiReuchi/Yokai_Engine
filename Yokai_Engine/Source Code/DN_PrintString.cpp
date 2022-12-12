#include "DN_PrintString.h"
#include "Application.h"
#include "Console.h"

DN_PrintString::DN_PrintString(ImVec2 pos, BluePrint* bp) : BP_Node("Print String", BP_Node::TYPE::SIMPLE, pos, ImColor(0, 125, 125), bp)
{
	BP_Pin ask_pin1(bp->unique_id++, "Text", BP_Pin::TYPE::String, bp, BP_Pin::BoxType::STRING);
	ask_pin1.string_box = "";
	ask_pin1.node = this;
	inputs.push_back(ask_pin1);
	BP_Pin ask_pin2(bp->unique_id++, "Execute", BP_Pin::TYPE::Executable, bp);
	ask_pin2.node = this;
	inputs.push_back(ask_pin2);
}

DN_PrintString::DN_PrintString(ImVec2 pos, BluePrint* bp, int ask1_pin_id, int ask2_pin_id, std::string box) : BP_Node("Print String", BP_Node::TYPE::SIMPLE, pos, ImColor(0, 125, 125), bp)
{
	BP_Pin ask_pin1(ask1_pin_id, "Text", BP_Pin::TYPE::String, bp, BP_Pin::BoxType::STRING);
	ask_pin1.string_box = box;
	ask_pin1.node = this;
	inputs.push_back(ask_pin1);
	BP_Pin ask_pin2(ask2_pin_id, "Execute", BP_Pin::TYPE::Executable, bp);
	ask_pin2.node = this;
	inputs.push_back(ask_pin2);
}

DN_PrintString::~DN_PrintString()
{

}

void DN_PrintString::Activate(float dt)
{
	if (inputs[0].IsPinLinked())
	{
		Console::LogInConsole(inputs[0].GetOpositePin()->node->info_as_name);
	}
	else
	{
		Console::LogInConsole(inputs[0].string_box);
	}
}