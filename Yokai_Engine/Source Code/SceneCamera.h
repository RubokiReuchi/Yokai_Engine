#pragma once

#include "Camera.h"

class EO_Editor;

struct RayCastHit
{
	bool hit = false;
	float distance = 0.0f;
	float3 hit_point = float3(0, 0, 0);
};

class SceneCamera : public Camera
{
public:
	SceneCamera();
	~SceneCamera();

	void UpdateCameraInput(float dt);
	void CalculateMousePicking();

public:
	EO_Editor* editor = nullptr;

	bool confine_move = false;
	bool confine_pan = false;

private:
	void Focus(const float3& focusPoint);
	void Confine(float& value, float min_value, float max_value);

	bool fast_move = false;

};