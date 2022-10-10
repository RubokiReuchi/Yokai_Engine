#pragma once

#include "Component.h"
#include "Globals.h"

#include <vector>
#include <string>

class GameObject
{
public:
	GameObject(GameObject* parent, std::string name = "Default", std::string tag = "Default");
	GameObject(GameObject* parent, std::string& name, std::string& tag);
	~GameObject();

	//TODO: Could do with an array of functions that creates a specific component
	template <class T>
	T* AddComponent()
	{
		T* newComponent = new T(this);
		components.push_back(newComponent);
		return newComponent;
	}

	template<class T>
	T* GetComponent(uint index = 0)
	{
		uint counter = 0;
		for (const auto& component : components)
		{
			if (typeid(*component) != typeid(T)) continue;
	
			if (counter == index) return (T*)component;
			else counter++;
		}
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