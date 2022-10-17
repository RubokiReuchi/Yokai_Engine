#pragma once

#include "Component.h"
#include "C_Transform.h"
#include "C_MeshRenderer.h"
#include "Globals.h"

#include <vector>
#include <string>

class C_Transform;

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
			new_component = new C_Transform(this);
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
	std::vector<Component*> GetComponentList()
	{
		return components;
	}

	bool AddChild(GameObject* child);
	std::vector<GameObject*> GetChilds()
	{
		return children;
	}

	bool SetParent(GameObject* parent);
	GameObject* GetParent()
	{
		return parent;
	}

	std::string name;
	std::string tag;
	C_Transform* transform = nullptr;

private:
	void RemoveChild(GameObject* child);

	std::vector<Component*> components;
	std::vector<GameObject*> children;

	GameObject* parent = nullptr;

	bool active = true;
	bool destroyed = false;

	uint id = 0; // ID =  0 is an invalid ID. First ID is 1

	//friend class C_Transform;
	//friend class EW_Hierarchy;
};