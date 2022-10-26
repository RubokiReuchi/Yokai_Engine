#pragma once

#include "EditorWindow.h"
#include "SceneCamera.h"

class EW_Scene : public EditorWindow
{
public:
	EW_Scene();

	~EW_Scene();

	void Update() override;

	void SetSceneCamera(bool active)
	{
		scene_camera->active = active;
	}

private:

	SceneCamera* scene_camera = nullptr;
	int scene_width, scene_height;

	bool popUp = false;
	int popUp_cd = 0;

};