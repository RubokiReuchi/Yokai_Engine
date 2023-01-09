#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_GO : public BP_Node
{
public:
	DN_GO(ImVec2 pos, BluePrint* bp);
	DN_GO(ImVec2 pos, BluePrint* bp, int ask_pin_id, int send_pin_id, int go_ID);
	virtual ~DN_GO() override;

	void SaveInNode();
	void OnLoad();

private:
	int serialized_go_ID = -1;
};