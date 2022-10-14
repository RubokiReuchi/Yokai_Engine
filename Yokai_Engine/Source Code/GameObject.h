#pragma once

#include "Component.h"
#include "C_MeshRenderer.h"
#include "Globals.h"

#include <vector>
#include <string>

class GameObject
{
public:
	GameObject(GameObject* parent, std::string name = "Default", std::string tag = "Default");
	~GameObject();

	//TODO: Could do with an array of functions that creates a specific component
	Component* AddComponent(Component::TYPE type)
	{
		Component* new_component;
		switch (type)
		{
		case Component::TYPE::TRANSFORM:
			break;
		case Component::TYPE::MESH_RENDERER:
			new_component = new C_MeshRenderer(this);
			break;
		case Component::TYPE::MATERIAL:
			break;
		default:
			LOG("component type error");
			break;
		}
		components.push_back(new_component);
		return new_component;
	}

	Component* GetComponent(Component::TYPE type)
	{
		for (auto component : components) // check all components
		{
			if (component->GetType() == type)
			{
				return component;
			}
		}
		return NULL; // component not find
	}

	bool AddChild(GameObject* child);

	bool SetParent(GameObject* parent);

public:
	std::string name;
	std::string tag;

private:
	void RemoveChild(GameObject* child);

private:
	std::vector<Component*> components;
	std::vector<GameObject*> children;

	GameObject* parent = nullptr;

	bool active = true;
	bool destroyed = false;
};