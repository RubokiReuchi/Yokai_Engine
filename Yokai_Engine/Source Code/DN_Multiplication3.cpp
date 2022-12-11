#include "DN_Multiplication3.h"
#include "Application.h"

DN_Multiplication3::DN_Multiplication3(ImVec2 pos, BluePrint* bp) : BP_Node("Multiply Vector3", BP_Node::TYPE::SIMPLE, pos, ImColor(100, 0, 228), bp)
{
	BP_Pin ask1_pin(bp->unique_id++, "Vector Value", BP_Pin::TYPE::Vector3, bp);
	ask1_pin.node = this;
	inputs.push_back(ask1_pin);
	BP_Pin ask2_pin(bp->unique_id++, "Float Value", BP_Pin::TYPE::Float, bp);
	ask2_pin.node = this;
	inputs.push_back(ask2_pin);
	BP_Pin send_pin(bp->unique_id++, "Result", BP_Pin::TYPE::Vector3, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_Multiplication3::~DN_Multiplication3()
{

}

void DN_Multiplication3::SaveInNode()
{
	info_as_vector3 = inputs[0].GetOpositePin()->node->info_as_vector3 * inputs[1].GetOpositePin()->node->info_as_number;
}