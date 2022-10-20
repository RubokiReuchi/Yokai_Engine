#pragma once

#include "Component.h"
#include "Camera.h"

class C_Camera : public Component
{
public:
	C_Camera(GameObject* gameObject);
	~C_Camera();

	void OnEditor();

	void OnPositionUpdate(float3 pos) override;
	void OnRotationUpdate(float3 rotation) override;
	void OnTransformUpdate(float3 pos, float3 scale, float3 rotation) override;

	Camera* GetCamera();

private:
	uint cameraID = 0;
};