#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

#include "MathGeoLib/include/MathGeoLib.h"

class DN_Rotate : public BP_Node
{
public:
	DN_Rotate(ImVec2 pos, BluePrint* bp);
	DN_Rotate(ImVec2 pos, BluePrint* bp, int ask1_pin_id, int ask2_pin_id, int ask3_pin_id);
	virtual ~DN_Rotate() override;

	void Activate(float dt);

private:

	float3 axis = float3(0, 0, 0);
	float angle = 0.0f;

};