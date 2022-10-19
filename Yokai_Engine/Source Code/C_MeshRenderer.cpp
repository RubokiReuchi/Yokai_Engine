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

void C_MeshRenderer::OnEditor()
{
	if (ImGui::CollapsingHeader("Mesh Render", ImGuiTreeNodeFlags_DefaultOpen))
	{
		if (ImGui::Checkbox("Visible", &visible))
		{
			GetMesh().visible = this->visible;
			for (auto& childs : GetGameObject()->GetChilds())
			{
				C_MeshRenderer* mr = dynamic_cast<C_MeshRenderer*>(childs->GetComponent(Component::TYPE::MESH_RENDERER));
				if (mr->visible)
				{
					mr->GetMesh().visible = this->visible;
				}
			}
		}
	}
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

void C_MeshRenderer::OnPositionUpdate(float3 position)
{
	GetMesh().SetPosition(position);
}

void C_MeshRenderer::OnScaleUpdate(float3 scale)
{
	GetMesh().SetScale(scale);
}

void C_MeshRenderer::OnRotationUpdate(float3 rotation)
{
	GetMesh().SetRotation(rotation);
}

void C_MeshRenderer::OnTransformUpdate(float3 position, float3 scale, float3 rotation)
{
	GetMesh().SetTransform(position, scale, rotation);
}

Re_Mesh& C_MeshRenderer::GetMesh()
{
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id);

	Re_Mesh& meshReference = manager->GetMap()[instance_id];

	return meshReference;
}
