#include "DN_Multiplication.h"
#include "Application.h"

DN_Multiplication::DN_Multiplication(ImVec2 pos, BluePrint* bp) : BP_Node("Multiply", BP_Node::TYPE::SIMPLE, pos, ImColor(100, 0, 228), bp)
{
	BP_Pin ask1_pin(bp->unique_id++, "First Value", BP_Pin::TYPE::Float, bp);
	ask1_pin.node = this;
	inputs.push_back(ask1_pin);
	BP_Pin ask2_pin(bp->unique_id++, "Second Value", BP_Pin::TYPE::Float, bp);
	ask2_pin.node = this;
	inputs.push_back(ask2_pin);
	BP_Pin send_pin(bp->unique_id++, "Result", BP_Pin::TYPE::Float, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_Multiplication::~DN_Multiplication()
{

}

void DN_Multiplication::SaveInNode()
{
	info_as_number = inputs[0].GetOpositePin()->node->info_as_number * inputs[1].GetOpositePin()->node->info_as_number;
}