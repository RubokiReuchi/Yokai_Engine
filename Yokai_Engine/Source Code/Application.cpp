#include "Application.h"

Application* app = NULL;

Application::Application() : fpsCap(60)
{
}

Application::~Application()
{
	for (int i = list_modules.size() - 1; i >= 0; --i)
	{
		delete list_modules[i];
		list_modules[i] = nullptr;
	}

	list_modules.clear();
}

bool Application::Init()
{
	window = new ModuleWindow(true);
	file = new ModuleFile(true);
	input = new ModuleInput(true);
	camera = new ModuleCamera3D(true);
	renderer3D = new ModuleRenderer3D(true);
	engine_order = new ModuleEO(true);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(file);
	AddModule(camera);
	AddModule(input);
	AddModule(engine_order);

	// Renderer last!
	AddModule(renderer3D);

	fpsCap = 60;
	SetFPS(fpsCap);

	bool ret = true;

	// Call Init() in all modules
	for (unsigned int i = 0; i < list_modules.size() && ret == true; i++)
	{
		ret = list_modules[i]->Init();
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");

	// Call Init() in all modules
	for (unsigned int i = 0; i < list_modules.size() && ret == true; i++)
	{
		ret = list_modules[i]->Start();
	}
	
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	/*if (renderer3D->vsync)
	{
		Uint32 last_frame_ms = ms_timer.Read();
		float wait_time = (1000.f / (float)fpsCap) - (float)last_frame_ms;
		SDL_Delay(static_cast<Uint32>(fabs(wait_time)));
	}*/
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	for (unsigned int i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PreUpdate(dt);
	}
	for (unsigned int i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->Update(dt);
	}
	for (unsigned int i = 0; i < list_modules.size() && ret == UPDATE_CONTINUE; i++)
	{
		ret = list_modules[i]->PostUpdate(dt);
	}

	dt = ms_timer.getDeltaTime();

	if (dt < fps)
	{
		float sleepTime = (fps - dt) * 1000;
		Sleep(sleepTime);
	}

	ms_timer.Reset();

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	
	for (int i = list_modules.size() - 1; i >= 0 && ret == true; --i)
	{
		ret = list_modules[i]->CleanUp();
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}