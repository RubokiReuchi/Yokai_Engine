#include "GameObject.h"
#include "Application.h"

GameObject::GameObject(GameObject* parent, std::string name, std::string tag) : name(name), tag(tag)
{
	id = app->engine_order->AddGameObject(this);
	transform = dynamic_cast<C_Transform*>(AddComponent(Component::TYPE::TRANSFORM));
	if (parent != nullptr) parent->AddChild(this);
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

bool GameObject::AddChild(GameObject* child)
{
	if (!child) return false;
	if (child->parent == this) return false;

	GameObject* p = parent;

	while (p)
	{
		if (p == child) return false;

		p = p->parent;
	}

	children.push_back(child);

	if (child->parent) child->parent->RemoveChild(child);

	child->parent = this;

	child->transform->parentGlobalTransform = transform->GetGlobalTransform();

	return true;
}

bool GameObject::SetParent(GameObject* parent)
{
	return parent->AddChild(this);
}

void GameObject::RemoveChild(GameObject* child)
{
	if (!child) return;

	for (size_t i = 0; i < children.size(); ++i)
	{
		if (children[i] == child) children.erase(children.begin() + i);
	}
	child->parent = nullptr;
}