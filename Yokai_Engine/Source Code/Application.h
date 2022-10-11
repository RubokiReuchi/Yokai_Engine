#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
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

public:

	int fpsCap;

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