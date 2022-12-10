#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_String : public BP_Node
{
public:
	DN_String(ImVec2 pos, BluePrint* bp);
	virtual ~DN_String() override;

	void SaveInNode();

private:

};