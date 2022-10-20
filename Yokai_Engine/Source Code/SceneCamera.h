#pragma once

#include "Camera.h"

class EO_Editor;

class SceneCamera : public Camera
{
public:
	SceneCamera();
	~SceneCamera();

	void UpdateInput();

public:
	EO_Editor* editor = nullptr;

private:
	void Focus(const float3& focusPoint);

};