#include "DN_PressKey.h"
#include "Application.h"
#include "ModuleInput.h"

DN_PressKey::DN_PressKey(int id, ImVec2 pos, BluePrint* bp) : BP_Node(id, "Press Key", BP_Node::TYPE::CORE, pos, ImColor(200, 0, 0), bp)
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
	BP_Pin send_pin2(bp->unique_id++, "Release", BP_Pin::TYPE::Executable, bp);
	send_pin2.node = this;
	outputs.push_back(send_pin2);
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

	if (app->input->GetKey(key) == KEY_DOWN && outputs[0].IsPinLinked()) // on press
	{
		outputs[0].GetOpositePin()->node->Activate(dt);
	}
	else if (app->input->GetKey(key) == KEY_UP && outputs[1].IsPinLinked()) // on release
	{
		outputs[1].GetOpositePin()->node->Activate(dt);
	}
}