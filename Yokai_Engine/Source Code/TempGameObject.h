#pragma once

#include "GameObject.h"

class TempGameObject : public GameObject
{
public:
	TempGameObject(GameObject* parent, TempGoType temp_type, float life_time, float speed, std::string name = "Default", std::string tag = "Default", bool is_camera = false);
	virtual ~TempGameObject() override;

	void UpdateInGame(float dt);

private:
	float life_time;

	float3 direction;
	float speed;

};