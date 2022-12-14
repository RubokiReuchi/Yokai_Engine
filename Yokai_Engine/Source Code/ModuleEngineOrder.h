#pragma once

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"
#include "EO_Base.h"
#include "Serialization.h"

#include <map>
#include <random>

class EO_Editor;
class EO_Game;

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
	void LoadSerializedGO();
	GameObject* GetGameObjectByUUID(std::string uuid);
	void NewScene();

	std::string GenerateUUID();
	uint seed; // used to generate random numbers

public:
	EO_Base* engine_order[(uint)EO_NUM::NUM_EO_TYPE] = { nullptr };
	EO_Editor* editor = nullptr;
	EO_Game* game = nullptr;
	ImVec2 scene_pos;
	ImVec2 scene_size;

	uint id_counter = 1;

	GameObject* rootGameObject = nullptr;

	std::map<uint, GameObject*> game_objects;
	std::vector<GameObject*> delete_queu;
	bool new_scene = false;
	bool load_scene = false;

	std::vector<SerializedGO> serialized_go;
	std::map<std::string, uint> loadedSerializedMeshes;
};