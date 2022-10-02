#pragma once
#include "Module.h"
#include "Globals.h"

class ModuleFile : public Module
{
public:
	ModuleFile(Application* app, bool start_enabled = true);
	~ModuleFile();

	bool Init() override;
	bool Start() override;

	bool CleanUp() override;
};