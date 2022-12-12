#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_GetForward : public BP_Node
{
public:
	DN_GetForward(ImVec2 pos, BluePrint* bp);
	DN_GetForward(ImVec2 pos, BluePrint* bp, int ask_pin_id, int send_pin_id, std::string go_UUID);
	virtual ~DN_GetForward() override;

	void SaveInNode();
	void OnLoad();

private:
	std::string serialized_go_UUID = "";
};