#pragma once

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Camera.h"
#include "SceneCamera.h"

#include"MathGeoLib/include/Geometry/Frustum.h"

#include <map>

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void RequestFrameBufferRegen(Camera* camera, int width, int height);

public:
	std::map<uint, Camera> game_cameras;
	Camera* activeGameCamera = nullptr;
	SceneCamera sceneCamera;

	Camera* currentDrawingCamera = nullptr;

	bool updateSceneCamera = false;
	bool updateGameCamera = false;
	uint click_state = 0; // 0 --> idle, 1 --> single click, 2 --> double click

private:
	Camera* frameBufferRegenCamera = nullptr;
	int newBufferWidth, newBufferHeight;
};