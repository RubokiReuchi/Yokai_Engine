#pragma once

#include "Camera.h"

class EO_Editor;

class SceneCamera : public Camera
{
public:
	SceneCamera();
	~SceneCamera();

	void UpdateCameraInput(float dt);

public:
	EO_Editor* editor = nullptr;

	bool confine_move = false;
	bool confine_pan = false;

private:
	void Focus(const float3& focusPoint);

};