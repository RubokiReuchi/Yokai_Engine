#pragma once

#include "EditorWindow.h"
#include "SceneCamera.h"

#include "ImGuizmo/ImGuizmo.h"

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

	ImGuizmo::OPERATION guizmo_operation;
	ImGuizmo::MODE guizmo_mode;
	bool guizmo_hide;

private:

	SceneCamera* scene_camera = nullptr;
	int scene_width, scene_height;

	bool popUp = false;
	int popUp_cd = 0;

	float4x4 prev_transform;
	GameObject* prev_go;
	bool once = false;

};