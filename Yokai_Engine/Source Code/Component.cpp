#include "Component.h"
#include"GameObject.h"

Component::Component(GameObject* game_object, TYPE type) : go(game_object), type(type)
{
}

Component::~Component()
{
}