#include "GameObject.h"
#include "Application.h"
#include "EO_Editor.h"

GameObject::GameObject(GameObject* parent, std::string name, std::string tag, bool is_camera) : name(name), tag(tag), is_camera(is_camera)
{
	id = app->engine_order->AddGameObject(this);
	transform = dynamic_cast<C_Transform*>(AddComponent(Component::TYPE::TRANSFORM));
	if (parent != NULL) parent->AddChild(this);
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		RELEASE(components[i]);
	}
	components.clear();

	for (size_t i = 0; i < children.size(); i++)
	{
		RELEASE(children[i]);
	}
	children.clear();
}

void GameObject::DeleteGameObject()
{
	app->engine_order->editor->SetSelectedGameObject(NULL);
	parent->RemoveChild(this);
	app->engine_order->game_objects[id] = NULL;
	app->engine_order->delete_qeue.push_back(this);
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->DeleteGameObject();
	}
}

bool GameObject::AddChild(GameObject* child)
{
	if (child->parent == this)
	{
		return false;
	}

	GameObject* aux_parent = parent;
	while (aux_parent)
	{
		if (aux_parent == child)
		{
			return false;
		}

		aux_parent = aux_parent->parent;
	}

	children.push_back(child);

	if (child->parent)
	{
		child->parent->RemoveChild(child);
	}

	child->parent = this;
	child->transform->parentGlobalTransform = transform->GetGlobalTransform();
	child->transform->UpdateTransform();

	return true;
}

bool GameObject::SetParent(GameObject* parent)
{
	return parent->AddChild(this);
}

void GameObject::RemoveChild(GameObject* child)
{
	for (size_t i = 0; i < children.size(); ++i)
	{
		if (children[i] == child)
		{
			children.erase(children.begin() + i);
		}
	}
	child->parent = NULL;
}

void GameObject::GenerateAABB()
{
	this->aabb;
	C_MeshRenderer* mr = dynamic_cast<C_MeshRenderer*>(GetComponent(Component::TYPE::MESH_RENDERER));

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)mr->GetMesh().vertices, mr->GetMesh().vertices->size());
}