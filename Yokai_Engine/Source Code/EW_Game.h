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

	bool window_hovered = false;

private:

	Camera* currentGameCamera = nullptr;
	float game_width, game_height;

	RECT window_rect;
};