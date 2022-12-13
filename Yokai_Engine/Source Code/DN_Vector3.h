#pragma once

#include "BP_Node.h"
#include "SDL/include/SDL.h"

class DN_Vector3 : public BP_Node
{
public:
	DN_Vector3(ImVec2 pos, BluePrint* bp);
	DN_Vector3(ImVec2 pos, BluePrint* bp, int ask_pin_id, int send_pin_id, std::string vector_box0, std::string vector_box1, std::string vector_box2);
	virtual ~DN_Vector3() override;

	void SaveInNode();

private:

};