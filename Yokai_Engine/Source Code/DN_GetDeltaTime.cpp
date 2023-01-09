#include "DN_GetDeltaTime.h"
#include "Application.h"

DN_GetDeltaTime::DN_GetDeltaTime(ImVec2 pos, BluePrint* bp) : BP_Node("Get Delta Time", BP_Node::TYPE::PRIORITY, pos, ImColor(228, 125, 0), bp)
{
	BP_Pin send_pin(bp->unique_id++, "Delta Time", BP_Pin::TYPE::Float, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_GetDeltaTime::DN_GetDeltaTime(ImVec2 pos, BluePrint* bp, int send_pin_id) : BP_Node("Get Delta Time", BP_Node::TYPE::PRIORITY, pos, ImColor(228, 125, 0), bp)
{
	BP_Pin send_pin(send_pin_id, "Delta Time", BP_Pin::TYPE::Float, bp);
	send_pin.node = this;
	outputs.push_back(send_pin);
}

DN_GetDeltaTime::~DN_GetDeltaTime()
{

}

void DN_GetDeltaTime::Update(float dt)
{
	info_as_number = dt;
}