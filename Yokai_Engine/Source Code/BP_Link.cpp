#include "BP_Link.h"
#include "BlueprintHelpers.h"

BP_Link::BP_Link(int id, int input_id, int output_id, ImColor color, BluePrint* bp) : id_as_int(id), input_id_as_int(input_id), output_id_as_int(output_id), color(color), blue_print(bp)
{
	this->id = id_as_int;
	this->input_id = input_id_as_int;
	this->output_id = output_id_as_int;
}

BP_Link::~BP_Link()
{

}