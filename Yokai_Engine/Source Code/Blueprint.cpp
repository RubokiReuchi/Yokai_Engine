#include "Blueprint.h"
#include "BlueprintHelpers.h"

BluePrint::BluePrint(std::string name) : name(name)
{
	
}

BluePrint::~BluePrint()
{
	
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