#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_GetDeltaTime : public BP_Node
{
public:
	DN_GetDeltaTime(ImVec2 pos, BluePrint* bp);
	DN_GetDeltaTime(ImVec2 pos, BluePrint* bp, int send_pin_id);
	virtual ~DN_GetDeltaTime() override;

	void Update(float dt);

private:

};