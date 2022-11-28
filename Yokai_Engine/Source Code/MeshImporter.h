#pragma once

#include "OpenGL.h"
#include "GameObject.h"
#include "Globals.h"

#include "Assimp/include/Importer.hpp"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#include <map>

class GameObject;
struct VertexInfo;

struct MeshInfo
{
	uint numOfMeshes;
	uint initialID;
};

struct SaveMesh
{
	std::vector<VertexInfo> vertices;
	std::vector<uint> indices;

	void YK_LoadMesh(const char* path);
};

class MeshImporter
{
public:

	static GameObject* LoadMesh(std::string path);

private:
	static GameObject* LoadModelFromYK(std::string path, GameObject* parent = nullptr);
	static GameObject* LoadMeshFromYK(std::string path, GameObject* parent = nullptr);
	static GameObject* LoadMeshFromFBX(std::string path);

	static const aiScene* GetAiScene(std::string path);

	static void CreateNewNode(aiNode* node, const aiScene* scene, std::string path, GameObject* parent = nullptr);
	static void CreateMesh(aiMesh* mesh, const aiScene* scene, GameObject* parent, aiString node_name, std::string parent_path, bool create_go = false);
	static void CloneLoadedNode(aiNode* node, const aiScene* scene, uint& firstMeshID, GameObject* parent = nullptr); // create a copy

	static std::map<std::string, MeshInfo> loadedMeshes;
	static std::map<uint, std::string> loadedPaths;
	static Assimp::Importer importer;
	static GameObject* returnGameObject;
};