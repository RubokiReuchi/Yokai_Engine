#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_PressKey : public BP_Node
{
public:
	DN_PressKey(ImVec2 pos, BluePrint* bp);
	DN_PressKey(ImVec2 pos, BluePrint* bp, int key_pin_id, int send_pin1_id, int send_pin2_id, int send_pin3_id, std::string box);
	virtual ~DN_PressKey() override;

	void Update(float dt);

private:
	SDL_Scancode key;


};