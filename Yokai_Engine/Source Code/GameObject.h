#pragma once

#include "Component.h"
#include "C_Transform.h"
#include "C_MeshRenderer.h"
#include "C_Material.h"
#include "C_Camera.h"
#include "C_Blueprint.h"
#include "Globals.h"

#include <vector>
#include <string>

class C_Transform;
struct SerializedGO;
struct SerializedNode;
struct SerializedLink;

enum class TempGoType
{
	NONE,
	PROJECTIL,
};

class GameObject
{
public:
	GameObject(GameObject* parent, std::string name = "Default", std::string tag = "Default", bool is_camera = false);
	GameObject(SerializedGO go);
	virtual ~GameObject();

	void Update();
	virtual void UpdateInGame(float dt);
	void OnLoad();

	void DeleteGameObject();

	void AddSerializedNode(SerializedNode node, BluePrint* bp);
	void AddSerializedLink(SerializedLink link, BluePrint* bp);
	void ProcessSerializedBlueprint(SerializedGO go, BluePrint* bp);

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
			new_component = new C_Material(this);
			break;
		case Component::TYPE::CAMERA:
			new_component = new C_Camera(this);
			break;
		case Component::TYPE::BLUEPRINT:
			if (!GetComponent(Component::TYPE::BLUEPRINT))
			{
				new_component = new C_Blueprint(this);
			}
			else
			{
				return NULL;
			}
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
	void RemoveComponent(Component* component)
	{
		int pos_in_array = 0;
		for (size_t i = 0; i < components.size(); i++)
		{
			if (components.at(i) == component)
			{
				components.erase(components.begin() + pos_in_array);
				return;
			}
			else
			{
				pos_in_array++;
			}
		}
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
	void RemoveChild(GameObject* child);

	std::string name;
	bool enabled = true;
	bool visible = true;
	bool visible_on_editor = true;
	std::string tag;
	C_Transform* transform = nullptr;

	bool aabb_init = false;
	AABB aabb; // local
	void GenerateAABB();
	std::vector<float3> GetAllVerticesPositions(GameObject* go);
	void GenerateFixedAABB();

	OBB global_obb;
	AABB global_aabb;

	bool is_camera;

	void Disable();

	void Rename(std::string new_name)
	{
		name = new_name;
	}

public:
	std::vector<Component*> components;
	std::vector<GameObject*> children;

	GameObject* parent = nullptr;

	TempGoType temp_type = TempGoType::NONE;

	uint id = 0; // id = 0 is an invalid id, id = 1 is the world(root)
	std::string UUID = "-1";

	friend class EW_Hierarchy;
};