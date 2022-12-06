#include "BP_Pin.h"
#include "BP_Link.h"
#include "BlueprintHelpers.h"

BP_Pin::BP_Pin(int id, const char* name, TYPE type, BluePrint* bp) : id(id), name(name), type(type), blue_print(bp)
{
	
}

BP_Pin::~BP_Pin()
{

}

bool BP_Pin::IsPinLinked()
{
	for (auto& link : blue_print->links)
	{
		if (link->input_id == this->id || link->output_id == this->id) return true;
	}

	return false;
}