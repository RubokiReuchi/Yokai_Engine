#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Console.h"
#include "Module.h"
#include "Serialization.h"
#include "ModuleFile.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleEngineOrder.h"

#include"MathGeoLib/include/Algorithm/Random/LCG.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleEO* engine_order;
	ModuleFile* file;

	std::vector<Module*> list_modules;

private:

	Timer ms_timer;
	float dt;

	std::string scene_loaded_name = "default";

public:

	float fps = 1.0f / 60.0f;
	int fpsCap;
	void SetFPS(int mFPS)
	{
		fps = 1.0f / (float)mFPS;
	}

	bool restart = false;

	void SetSceneName(std::string scene_name)
	{
		scene_loaded_name = scene_name;
	}
	std::string GetSceneName()
	{
		return scene_loaded_name;
	}

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* app;