#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_GO : public BP_Node
{
public:
	DN_GO(ImVec2 pos, BluePrint* bp);
	virtual ~DN_GO() override;

	void SaveInNode();

private:

};