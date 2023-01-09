#pragma once

#include "EditorWindow.h"

class Camera;

class EW_Game : public EditorWindow
{
public:
	EW_Game();
	~EW_Game();

	void Update() override;

	void SetCurrentGameCamera(bool active)
	{
		currentGameCamera->active = active;
	}
	void SetCurrentGameCamera(Camera* cam)
	{
		currentGameCamera = cam;
	}

	bool window_hovered = false;
	bool mouse_confined = false;

private:

	Camera* currentGameCamera = nullptr;
	float game_width, game_height;
};