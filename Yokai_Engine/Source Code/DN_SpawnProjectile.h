#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_SpawnProjectile : public BP_Node
{
public:
	DN_SpawnProjectile(ImVec2 pos, BluePrint* bp);
	DN_SpawnProjectile(ImVec2 pos, BluePrint* bp, int ask1_pin_id, int ask2_pin_id, int ask3_pin_id, int ask4_pin_id, int send_pin_id);
	virtual ~DN_SpawnProjectile() override;

	void Activate(float dt);

private:

};