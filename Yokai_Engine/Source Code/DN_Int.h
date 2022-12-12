#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_Int : public BP_Node
{
public:
	DN_Int(ImVec2 pos, BluePrint* bp);
	DN_Int(ImVec2 pos, BluePrint* bp, int ask_pin_id, int send_pin_id, std::string box);
	virtual ~DN_Int() override;

	void SaveInNode();

private:

};