#pragma once

#include "BP_Node.h"

class CoreNode : public BP_Node
{
public:
	CoreNode(int id, const char* name, ImVec2 pos, ImColor color, BluePrint* bp);
	virtual ~CoreNode() override;

	virtual void Update(float dt);

private:
	
};