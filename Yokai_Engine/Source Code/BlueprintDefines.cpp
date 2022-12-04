#include "BlueprintDefines.h"
#include "BlueprintHelpers.h"

ImVector<Pin> BluePrint::pins;

Pin::Pin(int id, const char* name, PinType type) : id(id), node(nullptr), name(name), type(type), kind(PinKind::Input)
{
	BluePrint::pins.push_back(*this);
}

LinkInfo::LinkInfo(LinkId id, PinId input_id, PinId output_id, PinType type) : id(id), input_id(input_id), output_id(output_id)
{
	color = NodeEditorH::GetIconColor(type);
}