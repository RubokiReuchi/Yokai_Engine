#include "MeshImporter.h"
#include "Re_Mesh.h"
#include "Application.h"
#include "M_ModelRender.h"
#include "C_MeshRenderer.h"
#include "C_Material.h"
#include "ModuleFile.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

std::map<std::string, MeshInfo> MeshImporter::loadedMeshes;
Assimp::Importer MeshImporter::importer;
GameObject* MeshImporter::returnGameObject = nullptr;

GameObject* MeshImporter::LoadMesh(std::string path)
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

void MeshImporter::CreateNewNode(aiNode* node, const aiScene* scene, std::string path, GameObject* parent)
{
	if (node->mNumMeshes == 0 && node->mNumChildren == 1)
	{
		CreateNewNode(node->mChildren[0], scene, path, parent);
		return;
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
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 eulerRot = rot.ToEulerXYZ();
	eulerRot.x = math::RadToDeg(eulerRot.x);
	eulerRot.y = math::RadToDeg(eulerRot.y);
	eulerRot.z = math::RadToDeg(eulerRot.z);

	uint meshNum = node->mNumMeshes;

	loadedMeshes[path].numOfMeshes += meshNum;

	for (uint i = 0; i < meshNum; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		CreateMesh(mesh, scene, newParent, node->mName, (meshNum > 1 || necessaryNode));
	}

	for (uint i = 0; i < node->mNumChildren; i++)
	{
		CreateNewNode(node->mChildren[i], scene, path, newParent);
	}

	// set transform after al child have been added
	newParent->GenerateAABB();
	dynamic_cast<C_Transform*>(newParent->GetComponent(Component::TYPE::TRANSFORM))->SetTransform(pos, { 1.0f, 1.0f, 1.0f }, eulerRot);
}

void MeshImporter::CreateMesh(aiMesh* mesh, const aiScene* scene, GameObject* parent, aiString node_name, bool create_go)
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

	// save custom format
	std::string file = MESHES_PATH;
	file += node_name.C_Str();
	file += ".ykmesh";

	uint size = 0;
	char* buffer = (char*)app->file->YK_SaveMesh(size, vertices, indices);

	app->file->FS_Save(file.c_str(), buffer, size, false);
	RELEASE_ARRAY(buffer);

	// Load into a GameObject and set the mesh render
	if (create_go)
	{
		GameObject* newGameObject = new GameObject(parent, node_name.C_Str());
	}
	else
	{
		dynamic_cast<C_MeshRenderer*>(parent->AddComponent(Component::TYPE::MESH_RENDERER))->InitAsNewMesh(vertices, indices);
		dynamic_cast<C_Material*>(parent->AddComponent(Component::TYPE::MATERIAL));
	}
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
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 eulerRot = rot.ToEulerXYZ();
	eulerRot.x = math::RadToDeg(eulerRot.x);
	eulerRot.y = math::RadToDeg(eulerRot.y);
	eulerRot.z = math::RadToDeg(eulerRot.z);
	
	dynamic_cast<C_Transform*>(newParent->GetComponent(Component::TYPE::TRANSFORM))->SetTransform(pos, { 1.0f, 1.0f, 1.0f }, eulerRot);

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

void SaveMesh::YK_LoadMesh(const char* path)
{
	char* buffer = NULL;
	if (ModuleFile::FS_Load(path, &buffer) == 0) return;
	char* cursor = buffer;

	uint ranges[5];
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

GameObject* MeshImporter::LoadMeshFromYK(std::string path)
{
	returnGameObject = new GameObject(app->engine_order->rootGameObject, ModuleFile::FS_GetFileName(path, false));

	SaveMesh aux_mesh;
	aux_mesh.YK_LoadMesh(path.c_str());

	dynamic_cast<C_MeshRenderer*>(returnGameObject->AddComponent(Component::TYPE::MESH_RENDERER))->InitAsNewMesh(aux_mesh.vertices, aux_mesh.indices);
	dynamic_cast<C_Material*>(returnGameObject->AddComponent(Component::TYPE::MATERIAL));

	returnGameObject->GenerateAABB();
	dynamic_cast<C_Transform*>(returnGameObject->GetComponent(Component::TYPE::TRANSFORM))->SetTransform({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f });

	return returnGameObject;
}