#include "TempGameObject.h"

TempGameObject::TempGameObject(GameObject* parent, TempGoType temp_type, float life_time, float speed, std::string name, std::string tag, bool is_camera) : GameObject(parent, name, tag, is_camera)
{
	this->temp_type = temp_type;
	this->speed = speed;
	this->life_time = life_time;

	transform->SetPosition(parent->transform->GetGlobalTransform().position);
	transform->SetRotation(parent->transform->GetGlobalTransform().rotation);

	direction = transform->GetForward();
}

TempGameObject::~TempGameObject()
{

}

void TempGameObject::UpdateInGame(float dt)
{
	switch (temp_type)
	{
	case TempGoType::PROJECTIL:
		life_time -= dt;
		if (life_time <= 0)
		{
			//DeleteGameObject();
			Disable();
		}
		else
		{
			transform->Translate(direction * speed);
		}
		break;
	default:
		break;
	}
}