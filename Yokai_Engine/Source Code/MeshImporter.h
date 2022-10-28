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

	static GameObject* LoadMesh(std::string path);

private:
	static const aiScene* GetAiScene(std::string path);

	static void CreateNewNode(aiNode* node, const aiScene* scene, std::string path, GameObject* parent = nullptr);
	static void CreateMesh(aiMesh* mesh, const aiScene* scene, GameObject* parent, aiString node_name, bool create_go = false);
	static void CloneLoadedNode(aiNode* node, const aiScene* scene, uint& firstMeshID, GameObject* parent = nullptr); // create a copy

	static std::map<std::string, MeshInfo> loadedMeshes;
	static Assimp::Importer importer;
	static GameObject* returnGameObject;
};