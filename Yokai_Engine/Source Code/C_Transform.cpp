#include "C_Transform.h"
#include "GameObject.h"

C_Transform::C_Transform(GameObject* gameObject) : Component(gameObject, TYPE::TRANSFORM)
{
	if (this->GetGameObject()->GetParent() != NULL)
	{
		C_Transform* parentTransform = dynamic_cast<C_Transform*>(gameObject->GetParent()->GetComponent(Component::TYPE::TRANSFORM));
		parentGlobalTransform = parentTransform->GetGlobalTransform();
	}
}

C_Transform::~C_Transform()
{
}

void C_Transform::OnEditor()
{
	float3 temp_pos = localTransform.position;
	float3 temp_rot = localTransform.rotation;
	float3 temp_scl = localTransform.scale;
	
	ImGui::AlignTextToFramePadding();
	bool no_collapsed = ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap); ImGui::SameLine(ImGui::GetWindowWidth() - 22);
	if (ImGui::Button(ICON_FA_ELLIPSIS_VERTICAL "##Transform", ImVec2(20.0f, 0)))
	{
		popUpOpen = true;
		ImGui::OpenPopup("Component Options");
		ori = ImGui::GetMousePosOnOpeningCurrentPopup();
	}
	else
	{
		ImGui::CloseCurrentPopup();
	}
	if (no_collapsed)
	{
		float offset = ImGui::CalcTextSize("Position: ").x + 16;
		ImGui::Text("Position: ");
		ImGui::SameLine();
		if (ImGui::DragFloat3("##lPos", &temp_pos[0], 0.1f)) SetPosition(temp_pos);


		ImGui::Text("Rotation: ");
		ImGui::SameLine();
		ImGui::SetCursorPosX(offset);
		if (ImGui::DragFloat3("##lRot", &temp_rot[0], 0.1f)) SetRotation(temp_rot);


		ImGui::Text("Scale: ");
		ImGui::SameLine();
		ImGui::SetCursorPosX(offset);
		if (ImGui::DragFloat3("##lScale", &temp_scl[0], 0.1f)) SetScale(temp_scl);
	}
	if (popUpOpen)
	{
		ComponentOptions(true);
	}
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

void C_Transform::SetTransform(float4x4 matrix)
{
	float3 pos, scl, euler_rot;
	Quat rot;

	matrix.Decompose(pos, rot, scl);

	euler_rot = rot.ToEulerXYZ();
	euler_rot.x = math::RadToDeg(euler_rot.x);
	euler_rot.y = math::RadToDeg(euler_rot.y);
	euler_rot.z = math::RadToDeg(euler_rot.z);

	this->localTransform.position = pos;
	this->localTransform.scale = scl;
	this->localTransform.rotation = euler_rot;
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
	if (this->GetGameObject()->GetParent() == NULL)
	{
		return localTransform;
	}

	Transform global_transform;
	global_transform.position = parentGlobalTransform.position + localTransform.position;
	global_transform.rotation = parentGlobalTransform.rotation + localTransform.rotation;
	global_transform.scale.x = parentGlobalTransform.scale.x * localTransform.scale.x;
	global_transform.scale.y = parentGlobalTransform.scale.y * localTransform.scale.y;
	global_transform.scale.z = parentGlobalTransform.scale.z * localTransform.scale.z;

	return global_transform;
}

float3 C_Transform::GetForward()
{
	CalculateGlobalMatrix();
	return globalMatrix.RotatePart().Col(2).Normalized();
}

float3 C_Transform::GetRight()
{
	CalculateGlobalMatrix();
	return globalMatrix.RotatePart().Col(0).Normalized();
}

float3 C_Transform::GetUp()
{
	CalculateGlobalMatrix();
	return globalMatrix.RotatePart().Col(1).Normalized();
}

void C_Transform::UpdateBB()
{
	GameObject* go = GetGameObject();
	if (!go->aabb_init) return;

	CalculateGlobalMatrix();

	go->global_obb = go->aabb;
	go->global_obb.Transform(globalMatrix);

	go->global_aabb.SetNegativeInfinity();
	go->global_aabb.Enclose(go->global_obb);
}

void C_Transform::UpdatePosition()
{
	float3 globalPosition = parentGlobalTransform.position + localTransform.position;

	// For each child
	for (size_t i = 0; i < GetGameObject()->GetChilds().size(); i++)
	{
		C_Transform* child_transform = dynamic_cast<C_Transform*>(this->GetGameObject()->GetChilds().at(i)->GetComponent(Component::TYPE::TRANSFORM));
		child_transform->OnPositionUpdate(globalPosition);
	}

	// For each component, exclude transform component
	for (size_t i = 1; i < GetGameObject()->GetComponentList().size(); i++)
	{
		GetGameObject()->GetComponentList().at(i)->OnPositionUpdate(globalPosition);
	}

	UpdateBB();
}

void C_Transform::UpdateRotation()
{
	float3 globalRotation = parentGlobalTransform.rotation + localTransform.rotation;

	// For each child
	for (size_t i = 0; i < GetGameObject()->GetChilds().size(); i++)
	{
		C_Transform* child_transform = dynamic_cast<C_Transform*>(this->GetGameObject()->GetChilds().at(i)->GetComponent(Component::TYPE::TRANSFORM));
		child_transform->OnRotationUpdate(globalRotation);
	}

	// For each component, exclude transform component
	for (size_t i = 1; i < GetGameObject()->GetComponentList().size(); i++)
	{
		GetGameObject()->GetComponentList().at(i)->OnRotationUpdate(globalRotation);
	}

	UpdateBB();
}

void C_Transform::UpdateScale()
{
	float3 globalScale;
	globalScale.x = parentGlobalTransform.scale.x * localTransform.scale.x;
	globalScale.y = parentGlobalTransform.scale.y * localTransform.scale.y;
	globalScale.z = parentGlobalTransform.scale.z * localTransform.scale.z;

	// For each child
	for (size_t i = 0; i < GetGameObject()->GetChilds().size(); i++)
	{
		C_Transform* child_transform = dynamic_cast<C_Transform*>(GetGameObject()->GetChilds().at(i)->GetComponent(Component::TYPE::TRANSFORM));
		child_transform->OnScaleUpdate(globalScale);
	}

	// For each component, exclude transform component
	for (size_t i = 1; i < GetGameObject()->GetComponentList().size(); i++)
	{
		GetGameObject()->GetComponentList().at(i)->OnScaleUpdate(globalScale);
	}

	UpdateBB();
}

void C_Transform::UpdateTransform()
{
	Transform globalTransform = GetGlobalTransform();

	// For each child
	for (size_t i = 0; i < GetGameObject()->GetChilds().size(); i++)
	{
		C_Transform* child_transform = dynamic_cast<C_Transform*>(GetGameObject()->GetChilds().at(i)->GetComponent(Component::TYPE::TRANSFORM));
		child_transform->OnTransformUpdate(globalTransform.position, globalTransform.scale, globalTransform.rotation);
	}

	// For each component, exclude transform component
	for (size_t i = 1; i < GetGameObject()->GetComponentList().size(); i++)
	{
		GetGameObject()->GetComponentList().at(i)->OnTransformUpdate(globalTransform.position, globalTransform.scale, globalTransform.rotation);
	}

	UpdateBB();
}

void C_Transform::CalculateGlobalMatrix()
{
	Transform globalTransform = GetGlobalTransform();

	float aux_x = math::DegToRad(globalTransform.rotation.x);
	float aux_y = math::DegToRad(globalTransform.rotation.y);
	float aux_z = math::DegToRad(globalTransform.rotation.z);

	math::Quat rotation = Quat::FromEulerXYZ(aux_x, aux_y, aux_z);
	globalMatrix = float4x4::FromTRS(globalTransform.position, rotation, globalTransform.scale);
}