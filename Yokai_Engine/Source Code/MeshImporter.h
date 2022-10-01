#pragma once
#include "Globals.h"
#include "OpenGL.h"

class aiMesh;

struct StoredMesh
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
};

namespace MeshImporter
{
	void EnableDebugMode();
	void DisableDebugMode();

	void LoadMesh(char* path_file, StoredMesh* myMesh);

	void CreateMesh(StoredMesh myMesh);
	void RenderMesh(StoredMesh myMesh);
}