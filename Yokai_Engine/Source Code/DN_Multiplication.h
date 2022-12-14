#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_Multiplication : public BP_Node
{
public:
	DN_Multiplication(ImVec2 pos, BluePrint* bp);
	DN_Multiplication(ImVec2 pos, BluePrint* bp, int ask1_pin_id, int ask2_pin_id, int send_pin_id);
	virtual ~DN_Multiplication() override;

	void Update(float dt);

private:

};