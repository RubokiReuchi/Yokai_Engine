#include "MeshImporter.h"
#include "Re_Mesh.h"
#include "Application.h"
#include "M_ModelRender.h"
#include "C_MeshRenderer.h"
#include "C_Material.h"
#include "ModuleFile.h"
#include <iostream>
#include <fstream>

#pragma comment (lib, "Assimp/libx86/assimp.lib")

std::map<std::string, MeshInfo> MeshImporter::loadedMeshes;
std::map<std::string, MeshInfo> MeshImporter::loadedCustomMeshes;
std::map<uint, std::string> MeshImporter::loadedPaths;
Assimp::Importer MeshImporter::importer;
GameObject* MeshImporter::returnGameObject = nullptr;

GameObject* MeshImporter::LoadMesh(std::string path)
{
	std::string mesh_path = MESHES_PATH + ModuleFile::FS_GetFileName(path, false) + ".ykmesh";
	std::string model_path = MODELS_PATH + ModuleFile::FS_GetFileName(path, false) + ".ykmodel";
	if (ModuleFile::FS_Exists(mesh_path))
	{
		return LoadMeshFromYK(mesh_path);
	}
	else if (ModuleFile::FS_Exists(model_path))
	{
		return LoadModelFromYK(model_path);
	}
	else
	{
		return LoadMeshFromFBX(path);
	}
}

GameObject* MeshImporter::LoadModelFromYK(std::string path, GameObject* parent)
{
	if (!parent) parent = app->engine_order->rootGameObject;
	GameObject* new_go = new GameObject(parent, ModuleFile::FS_GetFileName(path, false));

	std::vector<std::string> children_paths = app->file->YK_LoadModel(path);
	for (int i = 0; i < std::stoi(children_paths[0]); i++)
	{
		if (app->file->FS_GetExtension(children_paths[i + 1]) == ".ykmesh")
		{
			LoadMeshFromYK(children_paths[i + 1], new_go);
		}
		else if (app->file->FS_GetExtension(children_paths[i + 1]) == ".ykmodel")
		{
			LoadModelFromYK(children_paths[i + 1], new_go);
		}
	}

	// set transform after al child have been added
	new_go->GenerateAABB();
	dynamic_cast<C_Transform*>(new_go->GetComponent(Component::TYPE::TRANSFORM))->SetTransform({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f });

	return new_go;
}

GameObject* MeshImporter::LoadMeshFromYK(std::string path, GameObject* parent)
{
	if (!parent) parent = app->engine_order->rootGameObject;
	returnGameObject = new GameObject(parent, ModuleFile::FS_GetFileName(path, false));

	if (loadedCustomMeshes.find(path) != loadedCustomMeshes.end()) // check if path is loaded
	{
		dynamic_cast<C_MeshRenderer*>(returnGameObject->AddComponent(Component::TYPE::MESH_RENDERER))->InitAsInstanciedMesh(loadedCustomMeshes[path].initialID);
	}
	else
	{
		loadedCustomMeshes[path].initialID = app->renderer3D->model_render.GetMapSize();
		loadedCustomMeshes[path].numOfMeshes = 0;

		SaveMesh aux_mesh;
		aux_mesh.YK_LoadMesh(path.c_str());

		dynamic_cast<C_MeshRenderer*>(returnGameObject->AddComponent(Component::TYPE::MESH_RENDERER))->InitAsNewMesh(aux_mesh.vertices, aux_mesh.indices, path);
	}
	
	returnGameObject->AddComponent(Component::TYPE::MATERIAL);

	returnGameObject->GenerateAABB();
	dynamic_cast<C_Transform*>(returnGameObject->GetComponent(Component::TYPE::TRANSFORM))->SetTransform({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f });

	return returnGameObject;
}

GameObject* MeshImporter::LoadMeshFromFBX(std::string path)
{
	returnGameObject = nullptr;
	
	const aiScene* scene = GetAiScene(path);

	if (loadedMeshes.find(path) != loadedMeshes.end()) // check if path is loaded
	{
		uint first_id = loadedMeshes[path].initialID;
		CloneLoadedNode(scene->mRootNode, scene, first_id);
	}
	else
	{
		loadedMeshes[path].initialID = app->renderer3D->model_render.GetMapSize();
		loadedMeshes[path].numOfMeshes = 0;
		CreateNewNode(scene->mRootNode, scene, path);
	}
	return returnGameObject;
}

const aiScene* MeshImporter::GetAiScene(std::string path)
{
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		LOG("ERROR::ASSIMP::%s", importer.GetErrorString());
	}
	Console::LogInConsole("ASSIMP::Scene loaded from path: " + path);
	return scene;
}

std::vector<std::string> MeshImporter::CreateNewNode(aiNode* node, const aiScene* scene, std::string path, GameObject* parent)
{
	std::string file = "";
	std::string reference_file = path;
	std::vector<std::string> model_paths;
	// save custom format
	if (node->mNumChildren > 1)
	{
		model_paths.push_back(std::to_string(node->mNumChildren));
		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			if (node->mChildren[i]->mNumChildren > 1)
			{
				std::string model_file = MODELS_PATH;
				model_file += node->mChildren[i]->mName.C_Str();
				model_file += ".ykmodel";
				model_paths.push_back(model_file);
			}
		}

		file += MODELS_PATH;
		file += app->file->FS_GetFileName(path, false);
		file += ".ykmodel";

		app->file->YK_SaveModel(file, model_paths);
		app->file->YK_SaveMetaData(file, reference_file);
		path = file;
	}

	if (node->mNumMeshes == 0 && node->mNumChildren == 1)
	{
		std::vector<std::string> aux = CreateNewNode(node->mChildren[0], scene, path, parent);
		return aux;
	}
	
	// Create empty Gameobject 
	GameObject* newParent = nullptr;

	bool necessaryNode = node->mNumChildren > 1;

	if (parent == nullptr)
	{
		if (node->mNumChildren == 0)
		{
			returnGameObject = newParent = new GameObject(app->engine_order->rootGameObject, node->mName.C_Str());
		}
		else
		{
			returnGameObject = newParent = new GameObject(app->engine_order->rootGameObject, "GameObject");
		}
	}
	else
	{
		newParent = new GameObject(parent, node->mName.C_Str());
	}

	// Set new GameObject position with node Transform
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	std::string aux = node->mName.C_Str();
	if (aux == "RootNode") scale = float3(100, 100, 100);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 eulerRot = rot.ToEulerXYZ();
	eulerRot.x = math::RadToDeg(eulerRot.x);
	eulerRot.y = math::RadToDeg(eulerRot.y);
	eulerRot.z = math::RadToDeg(eulerRot.z);

	uint meshNum = node->mNumMeshes;

	//loadedMeshes[path].numOfMeshes += meshNum;
	std::vector<std::string> meshes_paths;

	for (uint i = 0; i < meshNum; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		std::string mesh_path = CreateMesh(mesh, scene, newParent, node->mName, path, (meshNum > 1 || necessaryNode));
		meshes_paths.push_back(mesh_path);
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		std::vector<std::string> aux = CreateNewNode(node->mChildren[i], scene, path, newParent);
		meshes_paths.insert(meshes_paths.end(), aux.begin(), aux.end());
	}

	// set transform after al child have been added
	newParent->GenerateAABB();
	dynamic_cast<C_Transform*>(newParent->GetComponent(Component::TYPE::TRANSFORM))->SetTransform(pos, {1,1,1}/*pos / 100.0f, scale / 100.0f*/, eulerRot);

	// save custom format
	if (node->mNumChildren > 1)
	{
		model_paths.insert(model_paths.end(), meshes_paths.begin(), meshes_paths.end());

		app->file->YK_SaveModel(path, model_paths);
		app->file->YK_SaveMetaData(path, reference_file);
	}

	return meshes_paths;
}

std::string MeshImporter::CreateMesh(aiMesh* mesh, const aiScene* scene, GameObject* parent, aiString node_name, std::string parent_path, bool create_go)
{
	std::vector<VertexInfo> vertices;
	std::vector<uint> indices;
	
	// Get Mesh information
	for (uint i = 0; i < mesh->mNumVertices; i++)
	{
		VertexInfo vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		if (mesh->HasNormals())
		{
			vertex.normals.x = mesh->mNormals[i].x;
			vertex.normals.y = mesh->mNormals[i].y;
			vertex.normals.z = mesh->mNormals[i].z;
		}


		if (mesh->HasTextureCoords(0))
		{
			vertex.tex_coords.x = mesh->mTextureCoords[0][i].x;
			vertex.tex_coords.y = mesh->mTextureCoords[0][i].y;
		}
		else
		{
			vertex.tex_coords = { 0.0f,0.0f };
		}

		vertices.push_back(vertex);
	}

	indices.resize(mesh->mNumFaces * 3);

	if (mesh->HasFaces())
	{
		for (uint i = 0; i < mesh->mNumFaces; ++i)
		{
			memcpy(&indices[i * 3], mesh->mFaces[i].mIndices, 3 * sizeof(uint));
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	}

	std::string compare = CheckSameMeshLoaded(vertices, indices);
	if (compare != "no loaded")
	{
		dynamic_cast<C_MeshRenderer*>(parent->AddComponent(Component::TYPE::MESH_RENDERER))->InitAsInstanciedMesh(loadedCustomMeshes[compare].initialID);
		dynamic_cast<C_Material*>(parent->AddComponent(Component::TYPE::MATERIAL));
		return compare;
	}

	// save custom format
	std::string file = MESHES_PATH;
	file += node_name.C_Str();
	file += ".ykmesh";

	uint size = 0;
	char* buffer = (char*)app->file->YK_SaveMesh(size, vertices, indices);

	app->file->FS_Save(file.c_str(), buffer, size, false);
	RELEASE_ARRAY(buffer);

	// save metadata
	app->file->YK_SaveMetaData(file, parent_path);

	// Load into a GameObject and set the mesh render
	if (create_go)
	{
		GameObject* newGameObject = new GameObject(parent, node_name.C_Str());
	}
	else
	{
		dynamic_cast<C_MeshRenderer*>(parent->AddComponent(Component::TYPE::MESH_RENDERER))->InitAsNewMesh(vertices, indices, file);
		dynamic_cast<C_Material*>(parent->AddComponent(Component::TYPE::MATERIAL));
	}

	loadedCustomMeshes[file].initialID = loadedCustomMeshes.size();
	loadedCustomMeshes[file].numOfMeshes = -1;
	return file;
}

void MeshImporter::CloneLoadedNode(aiNode* node, const aiScene* scene, uint& firstMeshID, GameObject* parent)
{
	if (node->mNumMeshes == 0 && node->mNumChildren == 1)
	{
		CloneLoadedNode(node->mChildren[0], scene, firstMeshID, parent);
		return;
	}

	GameObject* newParent = nullptr;

	bool necessaryNode = node->mNumChildren > 1;

	if (parent == nullptr)
	{
		if (node->mNumChildren == 0) returnGameObject = newParent = new GameObject(app->engine_order->rootGameObject, node->mName.C_Str());
		else returnGameObject = newParent = new GameObject(app->engine_order->rootGameObject, "GameObject");
	}
	else newParent = new GameObject(parent, node->mName.C_Str());

	// Set new GameObject position with node Transform
	aiVector3D translation, scaling;
	aiQuaternion rotation;

	node->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z);
	float3 scale(scaling.x, scaling.y, scaling.z);
	std::string aux = node->mName.C_Str();
	if (aux == "RootNode") scale = float3(100, 100, 100);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 eulerRot = rot.ToEulerXYZ();
	eulerRot.x = math::RadToDeg(eulerRot.x);
	eulerRot.y = math::RadToDeg(eulerRot.y);
	eulerRot.z = math::RadToDeg(eulerRot.z);
	
	dynamic_cast<C_Transform*>(newParent->GetComponent(Component::TYPE::TRANSFORM))->SetTransform(pos / 100.0f, scale / 100.0f, eulerRot);

	M_ModelRender* renderManager = &app->renderer3D->model_render;

	uint meshNum = node->mNumMeshes;

	if ((meshNum > 1 || necessaryNode))
	{
		for (uint i = 0; i < meshNum; i++)
		{
			GameObject* newGameObject = new GameObject(app->engine_order->rootGameObject, node->mName.C_Str());
			dynamic_cast<C_MeshRenderer*>(newGameObject->AddComponent(Component::TYPE::MESH_RENDERER))->InitAsInstanciedMesh(firstMeshID++);
		}
	}
	else if (meshNum != 0)
	{
		dynamic_cast<C_MeshRenderer*>(newParent->AddComponent(Component::TYPE::MESH_RENDERER))->InitAsInstanciedMesh(firstMeshID++);
	}


	for (uint i = 0; i < node->mNumChildren; i++)
	{
		CloneLoadedNode(node->mChildren[i], scene, firstMeshID, newParent);
	}

	// set transform after al child have been added
	dynamic_cast<C_Transform*>(newParent->GetComponent(Component::TYPE::TRANSFORM))->SetTransform(pos, { 1.0f, 1.0f, 1.0f }, eulerRot);
}

std::string MeshImporter::CheckSameMeshLoaded(std::vector<VertexInfo> vertex, std::vector<uint> indices)
{
	for (auto& mesh : loadedCustomMeshes)
	{
		SaveMesh aux;
		if (mesh.second.numOfMeshes == -1)
		{
			aux.YK_LoadMesh(mesh.first.c_str());
			if (aux.vertices.size() != vertex.size()) continue;
			if (aux.indices.size() != indices.size()) continue;
			for (size_t i = 0; i < indices.size(); i++)
			{
				if (indices[i] != aux.indices[i]) break;
			}
			for (size_t i = 0; i < vertex.size(); i++)
			{
				if (vertex[i].position.x != aux.vertices[i].position.x) break;
				if (vertex[i].position.y != aux.vertices[i].position.y) break;
				if (vertex[i].position.z != aux.vertices[i].position.z) break;
				if (vertex[i].normals.x != aux.vertices[i].normals.x) break;
				if (vertex[i].normals.y != aux.vertices[i].normals.y) break;
				if (vertex[i].normals.z != aux.vertices[i].normals.z) break;
				if (vertex[i].tex_coords.x != aux.vertices[i].tex_coords.x) break;
				if (vertex[i].tex_coords.y != aux.vertices[i].tex_coords.y) break;
				if (vertex[i].texture_id != aux.vertices[i].texture_id) break;
			}

			return mesh.first;
		}
	}

	return "no loaded";
}

void SaveMesh::YK_LoadMesh(const char* path)
{
	char* buffer = NULL;
	if (ModuleFile::FS_Load(path, &buffer) == 0) return;
	char* cursor = buffer;

	uint ranges[2];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);
	cursor += bytes;

	size_t num_indices = ranges[0];
	size_t num_vertices = ranges[1];
	indices.resize(num_indices);
	vertices.resize(num_vertices);

	bytes = sizeof(uint) * num_indices;
	memcpy(&indices[0], cursor, bytes);
	cursor += bytes;

	bytes = sizeof(VertexInfo) * num_vertices;
	memcpy(&vertices[0], cursor, bytes);
	cursor += bytes;
}