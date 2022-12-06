#include "Blueprint.h"
#include "BlueprintHelpers.h"

BluePrint::BluePrint(std::string name) : name(name)
{
	
}

BluePrint::~BluePrint()
{
	for (auto& node : nodes) RELEASE(node);
	for (auto& pin : pins) RELEASE(pin);
	for (auto& link : links) RELEASE(link);
}

void BluePrint::Update(float dt)
{

}

//BP_Pin* BluePrint::GetPinByID(PinId id)
//{
//    for (auto& pin : pins)
//    {
//        if (pin->id == id)
//        {
//            return pin;
//        }
//    }
//
//    return NULL;
//}