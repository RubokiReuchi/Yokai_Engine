#pragma once

class GameObject;

class Component
{
public:

	enum class Type
	{
		NONE,
		TRANSFORM,
		MESH_RENDERER,
		MATERIAL,
	};

	Component(GameObject* game_object);
	virtual ~Component();

	bool IsEnabled() { return enabled; }
	GameObject* GetGameObject() { return go; }
	Type GetType() { return type; }

private:
	bool enabled = true;
	GameObject* go = nullptr;
	Type type = Type::NONE;

};