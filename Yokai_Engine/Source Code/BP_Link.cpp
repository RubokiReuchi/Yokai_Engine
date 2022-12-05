#include "BP_Link.h"
#include "BlueprintHelpers.h"

BP_Link::BP_Link(LinkId id, PinId input_id, PinId output_id/*, BP_Pin::TYPE type*/) : id(id), input_id(input_id), output_id(output_id)
{
	//color = NH::GetIconColor(type);
}

BP_Link::~BP_Link()
{

}