#include "Blueprint.h"
#include "DN_Includes.h"
#include "BlueprintHelpers.h"

BluePrint::BluePrint(std::string name, GameObject* go) : name(name), go(go)
{
	
}

BluePrint::~BluePrint()
{
	for (auto& node : nodes) RELEASE(node);
	nodes.clear();
	for (auto& link : links) RELEASE(link);
	links.clear();
}

void BluePrint::Update(float dt)
{
	if (!info_saved_in_nodes)
	{
		for (auto& node : nodes)
		{
			node->SaveInNode();
		}
		info_saved_in_nodes = true;
	}

	for (auto& node : nodes)
	{
		if (node->type == BP_Node::TYPE::PRIORITY)  node->Update(dt);
	}

	for (auto& node : nodes)
	{
		if (node->type == BP_Node::TYPE::CORE)  node->Update(dt);
	}
}

void BluePrint::OnLoad()
{
	for (auto& node : nodes)
	{
		node->OnLoad();
	}
}

void BluePrint::CreateNode(std::string node_name, ImVec2 pos)
{
	BP_Node* new_node = NULL;

	// input
	if (node_name == "Press Key") new_node = new DN_PressKey(pos, this);

	// variable
	if (node_name == "String") new_node = new DN_String(pos, this);
	if (node_name == "Boolean") new_node = new DN_Bool(pos, this);
	if (node_name == "Float") new_node = new DN_Float(pos, this);
	if (node_name == "Integer") new_node = new DN_Int(pos, this);
	if (node_name == "Game Object") new_node = new DN_GO(pos, this);

	// action
	if (node_name == "Print String") new_node = new DN_PrintString(pos, this);

	// modify
	if (node_name == "Translate") new_node = new DN_Translate(pos, this);

	// get value
	if (node_name == "Get Delta Time") new_node = new DN_GetDeltaTime(pos, this);
	if (node_name == "Get Forward") new_node = new DN_GetForward(pos, this);
	if (node_name == "Get Up") new_node = new DN_GetUp(pos, this);
	if (node_name == "Get Right") new_node = new DN_GetRight(pos, this);

	// operation
	if (node_name == "Multiply") new_node = new DN_Multiplication(pos, this);
	if (node_name == "Multiply Vector3") new_node = new DN_Multiplication3(pos, this);

	if (new_node != NULL)
	{
		for (auto& pin : new_node->inputs)
		{
			this->pins.push_back(&pin);
		}
		for (auto& pin : new_node->outputs)
		{
			this->pins.push_back(&pin);
		}
		this->nodes.push_back(new_node);
	}
}

void BluePrint::CreateNode(std::string node_name, ImVec2 pos, std::vector<int> inputs_id, std::vector<std::string> input_box, std::vector<std::string> input_go_UUID, std::vector<int> outputs_id)
{
	BP_Node* new_node = NULL;

	// input
	if (node_name == "Press Key") new_node = new DN_PressKey(pos, this, inputs_id[0], outputs_id[0], outputs_id[1], outputs_id[2], input_box[0]);

	// variable
	else if (node_name == "String") new_node = new DN_String(pos, this, inputs_id[0], outputs_id[0], input_box[0]);
	else if (node_name == "Boolean") new_node = new DN_Bool(pos, this, inputs_id[0], outputs_id[0], input_box[0]);
	else if (node_name == "Float") new_node = new DN_Float(pos, this, inputs_id[0], outputs_id[0], input_box[0]);
	else if (node_name == "Integer") new_node = new DN_Int(pos, this, inputs_id[0], outputs_id[0], input_box[0]);
	else if (node_name == "Game Object") new_node = new DN_GO(pos, this, inputs_id[0], outputs_id[0], input_go_UUID[0]);

	// action
	else if (node_name == "Print String") new_node = new DN_PrintString(pos, this, inputs_id[0], inputs_id[1], input_box[0]);

	// modify
	else if (node_name == "Translate") new_node = new DN_Translate(pos, this, inputs_id[0], inputs_id[1], inputs_id[2]);

	// get value
	else if (node_name == "Get Delta Time") new_node = new DN_GetDeltaTime(pos, this, outputs_id[0]);
	else if (node_name == "Get Forward") new_node = new DN_GetForward(pos, this, inputs_id[0], outputs_id[0], input_go_UUID[0]);
	else if (node_name == "Get Up") new_node = new DN_GetUp(pos, this, inputs_id[0], outputs_id[0], input_go_UUID[0]);
	else if (node_name == "Get Right") new_node = new DN_GetRight(pos, this, inputs_id[0], outputs_id[0], input_go_UUID[0]);

	// operation
	else if (node_name == "Multiply") new_node = new DN_Multiplication(pos, this, inputs_id[0], inputs_id[1], outputs_id[0]);
	else if (node_name == "Multiply Vector3") new_node = new DN_Multiplication3(pos, this, inputs_id[0], inputs_id[1], outputs_id[0]);

	if (new_node != NULL)
	{
		for (auto& pin : new_node->inputs)
		{
			this->pins.push_back(&pin);
		}
		for (auto& pin : new_node->outputs)
		{
			this->pins.push_back(&pin);
		}
		this->nodes.push_back(new_node);
	}
}

void BluePrint::CreateLink(int id, int input_id, int output_id, float3 color)
{
	BP_Link* new_link = new BP_Link(id, input_id, output_id, ImColor(color.x, color.y, color.z), this);
	this->links.push_back(new_link);
}