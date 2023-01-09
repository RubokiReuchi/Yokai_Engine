#include "TempGameObject.h"

TempGameObject::TempGameObject(GameObject* parent, Transform tranform, TempGoType temp_type, float life_time, float speed, std::string name, std::string tag, bool is_camera) : GameObject(parent, name, tag, is_camera)
{
	this->temp_type = temp_type;
	this->speed = speed;
	this->life_time = life_time;

	transform->SetTransform(tranform.position, tranform.scale, tranform.rotation);
}

TempGameObject::~TempGameObject()
{

}

void TempGameObject::UpdateInGame(float dt)
{
	if (!enabled) return;

	switch (temp_type)
	{
	case TempGoType::PROJECTIL:
		life_time -= dt;
		if (life_time <= 0)
		{
			Disable();
		}
		else
		{
			transform->Translate(transform->GetForward() * speed);
		}
		break;
	default:
		break;
	}
}