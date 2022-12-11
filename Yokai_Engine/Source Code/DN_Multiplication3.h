#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_Multiplication3 : public BP_Node
{
public:
	DN_Multiplication3(ImVec2 pos, BluePrint* bp);
	virtual ~DN_Multiplication3() override;

	void SaveInNode();

private:

};