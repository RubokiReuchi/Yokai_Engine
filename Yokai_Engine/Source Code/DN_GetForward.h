#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_GetForward : public BP_Node
{
public:
	DN_GetForward(ImVec2 pos, BluePrint* bp);
	virtual ~DN_GetForward() override;

	void SaveInNode();

private:

};