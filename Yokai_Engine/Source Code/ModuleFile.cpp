#include "Globals.h"
#include "Application.h"
#include "ModuleFile.h"

#include "FileSystem.h"
#include "MeshImporter.h"

ModuleFile::ModuleFile(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleFile::~ModuleFile()
{}

bool ModuleFile::Init()
{
	FileSystem::FSInit();

	return true;
}

bool ModuleFile::Start()
{

	
	return true;
}

bool ModuleFile::CleanUp()
{
	FileSystem::FSDeInit();

	return true;
}
