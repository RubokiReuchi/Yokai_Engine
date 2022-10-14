#include "C_MeshRenderer.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "M_ModelRender.h"
#include "Re_Mesh.h"

C_MeshRenderer::C_MeshRenderer(GameObject* gameObject) : Component(gameObject, TYPE::MESH_RENDERER)
{
}

C_MeshRenderer::~C_MeshRenderer()
{
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id);
	manager->GetMap().erase(instance_id);
}

void C_MeshRenderer::InitAsDefaultCube(float3 position, float3 scale)
{
	mesh_id = 0;
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id);

	if (!manager->initialized)
	{
		Re_Mesh cube;
		cube.InitAsCube(position, scale);
		instance_id = manager->SetMeshInformation(cube);
	}
	else
	{
		Re_Mesh meshInfo;
		meshInfo.InitAsMeshInformation(position, scale);
		instance_id = manager->AddMesh(meshInfo);
	}
}

void C_MeshRenderer::InitAsDefaultSphere(float3 position, float3 scale)
{
	mesh_id = 1;
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id);


	if (!manager->initialized)
	{
		Re_Mesh sphere;
		sphere.InitAsSphere(position, scale);
		instance_id = manager->SetMeshInformation(sphere);
	}
	else
	{
		Re_Mesh meshInfo;
		meshInfo.InitAsMeshInformation(position, scale);
		instance_id = manager->AddMesh(meshInfo);
	}
}

void C_MeshRenderer::InitAsLoadedMesh(uint mesh_id)
{
	this->mesh_id = mesh_id + 2;

	M_Render* manager = app->renderer3D->model_render.GetRenderManager(this->mesh_id);

	Re_Mesh instanceMesh;
	instanceMesh.InitAsMeshInformation({ 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });

	instance_id = manager->AddMesh(instanceMesh);

	//TODO: determine how this funciton will be called. 
	// The body of this funciton should:
	// Acces the ModelRenderManager to check if the given mesh has already been loaded.
	// Create an instance inside the RenderManager in ModelRenderManager that manages this unique Mesh.
	// Get an ID to access that instance, so its attributes can be dynamically changed from this component.
}

void C_MeshRenderer::InitAsNewMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices)
{
	Re_Mesh newMesh;
	newMesh.InitAsMesh(vertices, indices, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });

	mesh_id = app->renderer3D->model_render.GetMapSize() + 2;
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id); // Create a renderManager.

	instance_id = manager->SetMeshInformation(newMesh);

}

Re_Mesh& C_MeshRenderer::GetMesh()
{
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id);

	Re_Mesh& meshReference = manager->GetMap()[instance_id];

	return meshReference;
}
