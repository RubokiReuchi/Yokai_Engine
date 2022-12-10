#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_GetRight : public BP_Node
{
public:
	DN_GetRight(ImVec2 pos, BluePrint* bp);
	virtual ~DN_GetRight() override;

	void SaveInNode();

private:

};