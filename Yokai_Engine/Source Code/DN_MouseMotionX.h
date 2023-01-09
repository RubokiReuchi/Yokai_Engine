#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_MouseMotionX : public BP_Node
{
public:
	DN_MouseMotionX(ImVec2 pos, BluePrint* bp);
	DN_MouseMotionX(ImVec2 pos, BluePrint* bp, int send_pin1_id, int send_pin2_id);
	virtual ~DN_MouseMotionX() override;

	void Update(float dt);

private:
	float x_movement;

};