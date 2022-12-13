#include "DN_Vector3.h"
#include "Application.h"

DN_Vector3::DN_Vector3(ImVec2 pos, BluePrint* bp) : BP_Node("Vector3", BP_Node::TYPE::SIMPLE, pos, ImColor(147, 226, 74), bp)
{
	BP_Pin ask_pin(bp->unique_id++, "", BP_Pin::TYPE::None, bp, BP_Pin::BoxType::NUMBER3);
	ask_pin.vector3_box[0] = "";
	ask_pin.vector3_box[1] = "";
	ask_pin.vector3_box[2] = "";
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(bp->unique_id++, "Vector3", BP_Pin::TYPE::Vector3, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_Vector3::DN_Vector3(ImVec2 pos, BluePrint* bp, int ask_pin_id, int send_pin_id, std::string vector_box0, std::string vector_box1, std::string vector_box2) : BP_Node("Vector3", BP_Node::TYPE::SIMPLE, pos, ImColor(147, 226, 74), bp)
{
	BP_Pin ask_pin(ask_pin_id, "", BP_Pin::TYPE::None, bp, BP_Pin::BoxType::NUMBER3);
	ask_pin.vector3_box[0] = vector_box0;
	ask_pin.vector3_box[1] = vector_box1;
	ask_pin.vector3_box[2] = vector_box2;
	ask_pin.node = this;
	inputs.push_back(ask_pin);
	BP_Pin send_pin(send_pin_id, "Vector3", BP_Pin::TYPE::Vector3, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_Vector3::~DN_Vector3()
{

}

void DN_Vector3::SaveInNode()
{
	info_as_vector3 = float3(std::stof(inputs[0].vector3_box[0]), std::stof(inputs[0].vector3_box[1]), std::stof(inputs[0].vector3_box[2]));
}