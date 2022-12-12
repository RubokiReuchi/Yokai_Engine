#pragma once

#include "Component.h"
#include "BluePrint.h"

class C_Blueprint : public Component
{
public:
	C_Blueprint(GameObject* gameObject);
	virtual ~C_Blueprint() override;

	void UpdateInGame(float dt);
	void OnLoad();

	void OnEditor();

	BluePrint* GetBluePrint()
	{
		return blueprint;
	}

private:
	
	BluePrint* blueprint = nullptr;

	friend class Serialization;
};