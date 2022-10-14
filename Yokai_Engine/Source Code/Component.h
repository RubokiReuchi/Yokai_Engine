#pragma once

class GameObject;

class Component
{
public:

	enum class TYPE
	{
		NONE,
		TRANSFORM,
		MESH_RENDERER,
		MATERIAL,
	};

	Component(GameObject* game_object, TYPE type);
	virtual ~Component();

	bool IsEnabled() { return enabled; }
	GameObject* GetGameObject() { return go; }
	TYPE GetType() { return type; }

private:
	bool enabled = true;
	GameObject* go = nullptr;
	TYPE type = TYPE::NONE;

};