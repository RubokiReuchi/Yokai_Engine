#include "C_Transform.h"
#include "GameObject.h"

C_Transform::C_Transform(GameObject* gameObject) : Component(gameObject, TYPE::TRANSFORM)
{
	// If this is not the root node, we need the parents global transform.
	if (this->GetGameObject()->GetParent() != NULL)
	{
		C_Transform* parentTransform = dynamic_cast<C_Transform*>(gameObject->GetParent()->GetComponent(Component::TYPE::TRANSFORM));
		parentGlobalTransform = parentTransform->GetGlobalTransform();
	}
}

C_Transform::~C_Transform()
{
}

void C_Transform::SetPosition(float3 pos)
{
	this->localTransform.position = pos;
	UpdatePosition();
}

void C_Transform::SetScale(float3 scl)
{
	this->localTransform.scale = scl;
	UpdateScale();
}

void C_Transform::SetRotation(float3 rot)
{
	this->localTransform.rotation = rot;
	UpdateRotation();
}

void C_Transform::SetTransform(float3 pos, float3 scl, float3 rot)
{
	this->localTransform.position = pos;
	this->localTransform.scale = scl;
	this->localTransform.rotation = rot;
	UpdateTransform();
}

void C_Transform::Translate(float3 translation)
{
	this->localTransform.position += translation;
	UpdatePosition();
}

void C_Transform::Scale(float3 scale)
{
	this->localTransform.scale += scale;
	UpdateScale();
}

void C_Transform::Rotate(float3 rotate)
{
	this->localTransform.rotation += rotate;
	UpdateRotation();
}

void C_Transform::OnPositionUpdate(float3 pos)
{
	parentGlobalTransform.position = pos;
	UpdatePosition();
}

void C_Transform::OnRotationUpdate(float3 rot)
{
	parentGlobalTransform.rotation = rot;
	UpdateRotation();
}

void C_Transform::OnScaleUpdate(float3 scale)
{
	parentGlobalTransform.scale = scale;
	UpdateScale();
}

void C_Transform::OnTransformUpdate(float3 pos, float3 scale, float3 rot)
{
	parentGlobalTransform.position = pos;
	parentGlobalTransform.rotation = rot;
	parentGlobalTransform.scale = scale;
	UpdateTransform();
}

Transform C_Transform::GetGlobalTransform()
{
	// If we have no parent, we are the root gameObject. which means our local transform is also our global.
	if (this->GetGameObject()->GetParent() == NULL) return localTransform;

	Transform global_transform;
	global_transform.position = parentGlobalTransform.position + localTransform.position;
	global_transform.rotation = parentGlobalTransform.rotation + localTransform.rotation;
	global_transform.scale = parentGlobalTransform.scale + localTransform.scale;

	return global_transform;
}

void C_Transform::UpdatePosition()
{
	float3 globalPosition = parentGlobalTransform.position + localTransform.position;
	// Give current position change to the transform component of every child of this transform's gameobject.
	for (int i = 0; i < this->GetGameObject()->GetChilds().size(); i++)
	{
		C_Transform* child_transform = dynamic_cast<C_Transform*>(this->GetGameObject()->GetChilds().at(i)->GetComponent(Component::TYPE::TRANSFORM));
		child_transform->OnPositionUpdate(globalPosition);
	}

	// Give current position change to every component linked to this transform's gameobject.
	for (int i = 1; i < this->GetGameObject()->GetComponentList().size(); i++)
	{
		this->GetGameObject()->GetComponentList().at(i)->OnPositionUpdate(globalPosition);
	}
}

void C_Transform::UpdateRotation()
{
	float3 totalRotation = parentGlobalTransform.rotation + localTransform.rotation;
	// Give current rotation change to the transform component of every child of this transform's gameobject.
	for (int i = 0; i < this->GetGameObject()->GetChilds().size(); i++)
	{
		C_Transform* child_transform = dynamic_cast<C_Transform*>(this->GetGameObject()->GetChilds().at(i)->GetComponent(Component::TYPE::TRANSFORM));
		child_transform->OnRotationUpdate(totalRotation);
	}

	// Give current rotation change to every component linked to this transform's gameobject.
	for (int i = 1; i < this->GetGameObject()->GetComponentList().size(); i++)
	{
		this->GetGameObject()->GetComponentList().at(i)->OnRotationUpdate(totalRotation);
	}
}

void C_Transform::UpdateScale()
{
	float3 totalScale = parentGlobalTransform.scale + localTransform.scale;
	// Give current scale change to the transform component of every child of this transform's gameobject.
	for (int i = 0; i < this->GetGameObject()->GetChilds().size(); i++)
	{
		C_Transform* child_transform = dynamic_cast<C_Transform*>(this->GetGameObject()->GetChilds().at(i)->GetComponent(Component::TYPE::TRANSFORM));
		child_transform->OnScaleUpdate(totalScale);
	}

	// Give current scale change to every component linked to this transform's gameobject.
	for (int i = 1; i < this->GetGameObject()->GetComponentList().size(); i++)
	{
		this->GetGameObject()->GetComponentList().at(i)->OnScaleUpdate(totalScale);
	}
}

void C_Transform::UpdateTransform()
{
	Transform globalTransform = GetGlobalTransform();
	// Give current position change to the transform component of every child of this transform's gameobject.
	for (int i = 0; i < this->GetGameObject()->GetChilds().size(); i++)
	{
		C_Transform* child_transform = dynamic_cast<C_Transform*>(this->GetGameObject()->GetChilds().at(i)->GetComponent(Component::TYPE::TRANSFORM));
		child_transform->OnTransformUpdate(globalTransform.position, globalTransform.scale, globalTransform.rotation);
	}

	// Give current position change to every component linked to this transform's gameobject. i = 1 so we skip this transform instance
	for (int i = 1; i < this->GetGameObject()->GetComponentList().size(); i++)
	{
		this->GetGameObject()->GetComponentList().at(i)->OnTransformUpdate(globalTransform.position, globalTransform.scale, globalTransform.rotation);
	}
}