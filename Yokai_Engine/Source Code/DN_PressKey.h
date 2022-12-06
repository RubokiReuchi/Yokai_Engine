#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_PressKey : public BP_Node
{
public:
	DN_PressKey(int id, ImVec2 pos, BluePrint* bp);
	virtual ~DN_PressKey() override;

	void Update(float dt);

private:
	SDL_Scancode key;
};