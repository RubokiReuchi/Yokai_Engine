#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_PrintString : public BP_Node
{
public:
	DN_PrintString(ImVec2 pos, BluePrint* bp);
	DN_PrintString(ImVec2 pos, BluePrint* bp, int ask1_pin_id, int ask2_pin_id, std::string box);
	virtual ~DN_PrintString() override;

	void Activate(float dt);

private:

};