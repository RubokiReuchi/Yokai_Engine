#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_GetUp : public BP_Node
{
public:
	DN_GetUp(ImVec2 pos, BluePrint* bp);
	virtual ~DN_GetUp() override;

	void SaveInNode();

private:

};