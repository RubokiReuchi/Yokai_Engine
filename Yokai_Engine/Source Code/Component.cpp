#include "Component.h"
#include"GameObject.h"

Component::Component(GameObject* game_object) : go(game_object)
{
}

Component::~Component()
{
}