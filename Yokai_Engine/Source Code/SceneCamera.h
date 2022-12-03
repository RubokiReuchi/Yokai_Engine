#pragma once

#include "Camera.h"

class EO_Editor;
class GameObject;

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
	GameObject* GetGoInMouse();

public:
	EO_Editor* editor = nullptr;

	bool confine_move = false;
	bool confine_pan = false;

private:
	void Focus(const float3& objective);

	float speed_multiplier = 3.0f;

};