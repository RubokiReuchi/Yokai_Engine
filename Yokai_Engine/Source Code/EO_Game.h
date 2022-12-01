#pragma once

#include "EO_Base.h"
#include "GameTimer.h"

class EO_Game : public EO_Base
{
public:
	EO_Game();
	~EO_Game();

	void Start();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void CleanUp();

	bool in_game = false;
	bool paused = false;
	bool tick = false;

	GameTimer* timer = nullptr;

	void PlayGame();
	void PauseGame();
	void ContinueGame();
	void StopGame();
};