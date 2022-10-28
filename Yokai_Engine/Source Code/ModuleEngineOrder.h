#pragma once

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "EO_Base.h"

#include <map>

class EO_Editor;

enum class EO_NUM
{
	GAME,
	EDITOR, // editor should always be the last layer
	NUM_EO_TYPE, // number of EngineOrder types
};

class ModuleEO : public Module
{
public:
	ModuleEO(bool start_enabled = true);
	~ModuleEO();

	bool Init();
	bool Start();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void DrawEO();
	void DrawEO_Editor();

	bool CleanUp();

	uint AddGameObject(GameObject* go);

public:
	EO_Base* engine_order[(uint)EO_NUM::NUM_EO_TYPE] = { nullptr };
	EO_Editor* editor = nullptr;

	uint id_counter = 1;

	GameObject* rootGameObject = nullptr;

	std::map<uint, GameObject*> game_objects;
};