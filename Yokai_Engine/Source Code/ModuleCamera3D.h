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

private:
	Camera* frameBufferRegenCamera = nullptr;
	int newBufferWidth, newBufferHeight;
};