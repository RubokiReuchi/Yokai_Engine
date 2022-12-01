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

	ModelYK children_info = app->file->YK_LoadModel(path);
	for (size_t i = 0; i < children_info.children_paths.size(); i++)
	{
		if (app->file->FS_GetExtension(children_info.children_paths[i]) == ".ykmesh")
		{
			LoadMeshFromYK(children_info.children_paths[i], children_info.positions[i], children_info.scales[i], new_go);
		}
		else if (app->file->FS_GetExtension(children_info.children_paths[i]) == ".ykmodel")
		{
			LoadModelFromYK(children_info.children_paths[i], new_go);
		}
	}

	// set transform after al child have been added
	//new_go->GenerateAABB();
	dynamic_cast<C_Transform*>(new_go->GetComponent(Component::TYPE::TRANSFORM))->SetTransform({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f });

	return new_go;
}

GameObject* MeshImporter::LoadMeshFromYK(std::string path, float3 position, float3 scale, GameObject* parent)
{
	if (!parent) parent = app->engine_order->rootGameObject;
	returnGameObject = new GameObject(parent, ModuleFile::FS_GetFileName(path, false));

	C_MeshRenderer* mr = NULL;
	if (loadedCustomMeshes.find(path) != loadedCustomMeshes.end()) // check if path is loaded
	{
		mr = dynamic_cast<C_MeshRenderer*>(returnGameObject->AddComponent(Component::TYPE::MESH_RENDERER));
		mr->InitAsInstanciedMesh(loadedCustomMeshes[path].initialID);
	}
	else
	{
		loadedCustomMeshes[path].initialID = app->renderer3D->model_render.GetMapSize();
		loadedCustomMeshes[path].numOfMeshes = 0;

		SaveMesh aux_mesh;
		aux_mesh.YK_LoadMesh(path.c_str());

		mr = dynamic_cast<C_MeshRenderer*>(returnGameObject->AddComponent(Component::TYPE::MESH_RENDERER));
		mr->InitAsNewMesh(aux_mesh.vertices, aux_mesh.indices, path);
	}
	
	returnGameObject->AddComponent(Component::TYPE::MATERIAL);

	returnGameObject->GenerateAABB();
	dynamic_cast<C_Transform*>(returnGameObject->GetComponent(Component::TYPE::TRANSFORM))->SetTransform(position, scale, { 0.0f, 0.0f, 0.0f });

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

ModelYK MeshImporter::CreateNewNode(aiNode* node, const aiScene* scene, std::string path, GameObject* parent)
{
	if (node->mNumMeshes == 0 && node->mNumChildren == 1)
	{
		ModelYK aux = CreateNewNode(node->mChildren[0], scene, path, parent);
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

	std::string file = "";
	std::string reference_file = path;
	ModelYK model_info;
	// save custom format
	if (node->mNumChildren > 1)
	{
		model_info.children_paths.push_back(std::to_string(node->mNumChildren));
		for (size_t i = 0; i < node->mNumChildren; i++)
		{
			if (node->mChildren[i]->mNumChildren > 1)
			{
				std::string model_file = MODELS_PATH;
				model_file += node->mChildren[i]->mName.C_Str();
				model_file += ".ykmodel";
				model_info.children_paths.push_back(model_file);
				model_info.positions.push_back(float3(0, 0, 0));
				model_info.scales.push_back(float3(1, 1, 1));
			}
		}

		file += MODELS_PATH;
		file += app->file->FS_GetFileName(path, false);
		file += ".ykmodel";

		app->file->YK_SaveModel(file, model_info.children_paths, model_info.positions, model_info.scales);
		app->file->YK_SaveMetaData(file, reference_file);
		path = file;
	}

	ModelYK meshes_info;

	for (uint i = 0; i < meshNum; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		std::string mesh_path = CreateMesh(mesh, scene, newParent, node->mName, path, (meshNum > 1 || necessaryNode));
		meshes_info.children_paths.push_back(mesh_path);
		meshes_info.positions.push_back(pos / 100.0f);
		meshes_info.scales.push_back(scale / 100.0f);
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		ModelYK aux = CreateNewNode(node->mChildren[i], scene, path, newParent);
		meshes_info.children_paths.insert(meshes_info.children_paths.end(), aux.children_paths.begin(), aux.children_paths.end());
		meshes_info.positions.insert(meshes_info.positions.end(), aux.positions.begin(), aux.positions.end());
		meshes_info.scales.insert(meshes_info.scales.end(), aux.scales.begin(), aux.scales.end());
	}

	// set transform after al child have been added
	dynamic_cast<C_Transform*>(newParent->GetComponent(Component::TYPE::TRANSFORM))->SetTransform(pos / 100.0f, scale / 100.0f, eulerRot);

	// save custom format
	if (node->mNumChildren > 1)
	{
		model_info.children_paths.insert(model_info.children_paths.end(), meshes_info.children_paths.begin(), meshes_info.children_paths.end());
		model_info.positions.insert(model_info.positions.end(), meshes_info.positions.begin(), meshes_info.positions.end());
		model_info.scales.insert(model_info.scales.end(), meshes_info.scales.begin(), meshes_info.scales.end());

		app->file->YK_SaveModel(path, model_info.children_paths, model_info.positions, model_info.scales);
		app->file->YK_SaveMetaData(path, reference_file);
	}

	return meshes_info;
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
		parent->Rename(app->file->FS_GetFileName(compare, false));
		dynamic_cast<C_MeshRenderer*>(parent->AddComponent(Component::TYPE::MESH_RENDERER))->InitAsInstanciedMesh(loadedCustomMeshes[compare].initialID);
		dynamic_cast<C_Material*>(parent->AddComponent(Component::TYPE::MATERIAL));
		parent->GenerateAABB();
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
		parent->GenerateAABB();
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

	delete[] buffer;
	buffer = nullptr;
}