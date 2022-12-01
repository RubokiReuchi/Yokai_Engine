#pragma once

#include "EO_Base.h"
#include "ModuleEngineOrder.h"
#include "GameTimer.h"

class GameObject;

struct GameObjectInfo
{
	GameObject* parent = nullptr;
	std::string name;
	bool enabled;
	bool visible;
	Transform transform;
};

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

private:
	std::map<uint, GameObjectInfo> go_before_play;
	void SaveGameObject();
	void ReturnGameObject();
};