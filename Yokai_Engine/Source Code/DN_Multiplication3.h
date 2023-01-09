#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_Multiplication3 : public BP_Node
{
public:
	DN_Multiplication3(ImVec2 pos, BluePrint* bp);
	DN_Multiplication3(ImVec2 pos, BluePrint* bp, int ask1_pin_id, int ask2_pin_id, int send_pin_id);
	virtual ~DN_Multiplication3() override;

	void SaveInNode();

private:

};