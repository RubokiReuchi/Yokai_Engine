#include "DN_PressKey.h"
#include "Application.h"
#include "ModuleInput.h"

DN_PressKey::DN_PressKey(ImVec2 pos, BluePrint* bp) : BP_Node("Press Key", BP_Node::TYPE::CORE, pos, ImColor(125, 0, 0), bp)
{
	key = SDL_SCANCODE_UNKNOWN;
	BP_Pin key_pin(bp->unique_id++, "Key", BP_Pin::TYPE::String, bp, BP_Pin::BoxType::COMBO);
	std::string combo_options[36] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	key_pin.combo_box.insert(key_pin.combo_box.begin(), std::begin(combo_options), std::end(combo_options));
	key_pin.string_box = "A";
	key_pin.node = this;
	inputs.push_back(key_pin);
	BP_Pin send_pin1(bp->unique_id++, "Press", BP_Pin::TYPE::Executable, bp);
	send_pin1.node = this;
	outputs.push_back(send_pin1);
	BP_Pin send_pin2(bp->unique_id++, "Hold", BP_Pin::TYPE::Executable, bp);
	send_pin2.node = this;
	outputs.push_back(send_pin2);
	BP_Pin send_pin3(bp->unique_id++, "Release", BP_Pin::TYPE::Executable, bp);
	send_pin3.node = this;
	outputs.push_back(send_pin3);
}

DN_PressKey::DN_PressKey(ImVec2 pos, BluePrint* bp, int key_pin_id, int send_pin1_id, int send_pin2_id, int send_pin3_id, std::string box) : BP_Node("Press Key", BP_Node::TYPE::CORE, pos, ImColor(125, 0, 0), bp)
{
	key = SDL_SCANCODE_UNKNOWN;
	BP_Pin key_pin(key_pin_id, "Key", BP_Pin::TYPE::String, bp, BP_Pin::BoxType::COMBO);
	std::string combo_options[36] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
		"0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
	key_pin.combo_box.insert(key_pin.combo_box.begin(), std::begin(combo_options), std::end(combo_options));
	key_pin.string_box = box;
	key_pin.node = this;
	inputs.push_back(key_pin);
	BP_Pin send_pin1(send_pin1_id, "Press", BP_Pin::TYPE::Executable, bp);
	send_pin1.node = this;
	outputs.push_back(send_pin1);
	BP_Pin send_pin2(send_pin2_id, "Hold", BP_Pin::TYPE::Executable, bp);
	send_pin2.node = this;
	outputs.push_back(send_pin2);
	BP_Pin send_pin3(send_pin3_id, "Release", BP_Pin::TYPE::Executable, bp);
	send_pin3.node = this;
	outputs.push_back(send_pin3);
}

DN_PressKey::~DN_PressKey()
{

}

void DN_PressKey::Update(float dt)
{
	if (inputs[0].IsPinLinked())
	{
		std::string aux = inputs[0].GetOpositePin()->node->info_as_name;
		key = SDL_GetScancodeFromName(aux.c_str());
	}
	else
	{
		key = SDL_GetScancodeFromName(inputs[0].string_box.c_str());
	}

	std::vector<BP_Pin*> aux_pins;
	if (app->input->GetKey(key) == KEY_DOWN && outputs[0].IsPinLinked()) // on press
	{
		aux_pins = outputs[0].GetOpositesPins();
		for (auto& pin : aux_pins)
		{
			pin->node->Activate(dt);
		}
	}
	else if (app->input->GetKey(key) == KEY_REPEAT && outputs[1].IsPinLinked()) // on hold
	{
		aux_pins = outputs[1].GetOpositesPins();
		for (auto& pin : aux_pins)
		{
			pin->node->Activate(dt);
		}
	}
	else if (app->input->GetKey(key) == KEY_UP && outputs[2].IsPinLinked()) // on release
	{
		aux_pins = outputs[2].GetOpositesPins();
		for (auto& pin : aux_pins)
		{
			pin->node->Activate(dt);
		}
	}
}