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

#ifndef STANDALONE
	MeshImporter::EnableDebugMode();
#endif // !STANDALONE

	return true;
}

bool ModuleFile::Start()
{

	
	return true;
}

bool ModuleFile::CleanUp()
{
	FileSystem::FSDeInit();

#ifndef STANDALONE
	MeshImporter::DisableDebugMode();
#endif // !STANDALONE

	return true;
}
