#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_Bool : public BP_Node
{
public:
	DN_Bool(ImVec2 pos, BluePrint* bp);
	DN_Bool(ImVec2 pos, BluePrint* bp, int ask_pin_id, int send_pin_id, std::string box);
	virtual ~DN_Bool() override;

	void SaveInNode();

private:

};