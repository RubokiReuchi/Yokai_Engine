#pragma once

#include "Component.h"
#include "Camera.h"

class C_Camera : public Component
{
public:
	C_Camera(GameObject* gameObject);
	~C_Camera();

	void Update();

	void OnEditor();

	void OnPositionUpdate(float3 pos) override;
	void OnRotationUpdate(float3 rotation) override;
	void OnTransformUpdate(float3 pos, float3 scale, float3 rotation) override;

	Camera* GetCamera();

private:
	uint cameraID = 0;
	Quat original_lookingDir;
	Quat lookingDir;

	std::vector<float3> PointsToLines_OBB(float3 points[8]);
	void DrawView();

	friend class Serialization;
};