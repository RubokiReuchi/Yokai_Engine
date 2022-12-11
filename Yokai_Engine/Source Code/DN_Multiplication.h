#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_Multiplication : public BP_Node
{
public:
	DN_Multiplication(ImVec2 pos, BluePrint* bp);
	virtual ~DN_Multiplication() override;

	void SaveInNode();

private:

};