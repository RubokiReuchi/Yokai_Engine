#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_Float : public BP_Node
{
public:
	DN_Float(ImVec2 pos, BluePrint* bp);
	DN_Float(ImVec2 pos, BluePrint* bp, int ask_pin_id, int send_pin_id, std::string box);
	virtual ~DN_Float() override;

	void SaveInNode();

private:

};