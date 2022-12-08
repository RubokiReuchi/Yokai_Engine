#include "BP_Pin.h"
#include "BP_Link.h"
#include "BlueprintHelpers.h"

BP_Pin::BP_Pin(int id, const char* name, TYPE type, BluePrint* bp, BoxType box_type) : id_as_int(id), name(name), type(type), blue_print(bp), box_type(box_type)
{
	id = id_as_int;
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

BP_Pin* BP_Pin::GetOpositePin()
{
	for (auto& link : blue_print->links)
	{
		if (this->kind == PinKind::Input)
		{
			if (this->id == link->input_id)
			{
				return NH::GetPinByID(link->output_id, blue_print);
			}
		}
		else
		{
			if (this->id == link->output_id)
			{
				return NH::GetPinByID(link->input_id, blue_print);
			}
		}
	}

	return NULL;
}