#pragma once

#include "GameObject.h"
#include "Globals.h"
#include "OpenGL.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#include <map>

struct MeshInfo
{
	uint numOfMeshes;
	uint initialID;
};

class MeshImporter
{
public:

	static void LoadMesh(std::string path); // This function loads all the information in the mesh and creates all necessary Gameobjects in the process

private:
	static const aiScene* GetAiScene(std::string path);

	// ProcessNew creates a new RenderManger for each Mesh
	static void ProcessNewNode(aiNode* node, const aiScene* scene, std::string path, GameObject* parent = nullptr);
	static void ProcessNewMesh(aiMesh* mesh, const aiScene* scene, GameObject* parent);

	//ProcessLoaded creates a new Instance inside a RenderManager for each Mesh
	static void ProcessLoadedNode(aiNode* node, const aiScene* scene, uint firstMeshID, GameObject* parent = nullptr);

	static std::map<std::string, MeshInfo> loadedMeshes;
	static Assimp::Importer importer;
};