#pragma once

#include "OpenGL.h"
#include "GameObject.h"
#include "Globals.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#include <map>

class GameObject;

struct MeshInfo
{
	uint numOfMeshes;
	uint initialID;
};

class MeshImporter
{
public:

	static GameObject* LoadMesh(std::string path); // This function loads all the information in the mesh and creates all necessary Gameobjects in the process

private:
	static const aiScene* GetAiScene(std::string path);

	// ProcessNew creates a new RenderManger for each Mesh
	static void ProcessNewNode(aiNode* node, const aiScene* scene, std::string path, GameObject* parent = nullptr);
	static void ProcessNewMesh(aiMesh* mesh, const aiScene* scene, GameObject* parent, aiString node_name, bool create_go = false);

	//ProcessLoaded creates a new Instance inside a M_Render for each Mesh
	static void ProcessLoadedNode(aiNode* node, const aiScene* scene, uint& firstMeshID, GameObject* parent = nullptr);

	static std::map<std::string, MeshInfo> loadedMeshes;
	static Assimp::Importer importer;
	static GameObject* returnGameObject;
};