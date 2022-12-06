#include "DN_PressKey.h"
#include "Application.h"
#include "ModuleInput.h"

DN_PressKey::DN_PressKey(int id, ImVec2 pos, BluePrint* bp) : CoreNode(id, "Press Key", pos, ImColor(200, 0, 0), bp)
{
	key = SDL_SCANCODE_UNKNOWN;
	int unique_id = 1;
	BP_Pin key_pin(unique_id++, "Key", BP_Pin::TYPE::String, bp, true, BP_Pin::TextType::COMBO);
	inputs.push_back(key_pin);
	BP_Pin send_pin(unique_id++, "", BP_Pin::TYPE::Executable, bp);
	outputs.push_back(send_pin);
}

DN_PressKey::~DN_PressKey()
{

}

void DN_PressKey::Update(float dt)
{
	key = inputs[0].text;
	if (app->input->GetKey(key) == KEY_DOWN) // on press
	{

	}
	else if (app->input->GetKey(key) == KEY_UP) // on release
	{

	}
}