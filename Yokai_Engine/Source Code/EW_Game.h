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

private:

	Camera* currentGameCamera = nullptr;
	int game_width, game_height;
};