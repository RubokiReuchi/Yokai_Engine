#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_Translate : public BP_Node
{
public:
	DN_Translate(ImVec2 pos, BluePrint* bp);
	virtual ~DN_Translate() override;

	void Activate(float dt);

private:

	float3 direction = float3(0, 0, 0);
	float distance = 0.0f;

};