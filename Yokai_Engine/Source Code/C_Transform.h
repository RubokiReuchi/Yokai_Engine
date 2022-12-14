#pragma once

#include "Component.h"

struct Transform
{
	float3 position = float3(0.0f, 0.0f, 0.0f);
	float3 scale = float3(1.0f, 1.0f, 1.0f);
	float3 rotation = float3(0.0f, 0.0f, 0.0f);
};

class C_Transform : public Component
{
public:
	C_Transform(GameObject* gameObject);
	~C_Transform();

	void Update();

	void OnEditor();

	void SetPosition(float3 pos);
	void SetRotation(float3 rot);
	void SetScale(float3 scl);
	void SetTransform(float3 pos, float3 scl, float3 rot);
	void SetTransform(float4x4 matrix);

	Transform GetGlobalTransform();
	float4x4 GetGlobalMatrix()
	{
		return globalMatrix;
	}

	void UpdateBB();

	Transform GetLocalTransform()
	{
		return localTransform;
	}

	void Translate(float3 translation);
	void Scale(float3 scale);
	void Rotate(float3 rotate);

	float3 GetForward();
	float3 GetRight();
	float3 GetUp();

private:
	Transform localTransform;

	void CalculateGlobalMatrix();

private:
	float4x4 globalMatrix;

	Transform parentGlobalTransform;
	friend class GameObject;
};