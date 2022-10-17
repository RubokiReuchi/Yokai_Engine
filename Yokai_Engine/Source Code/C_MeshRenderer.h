#pragma once

#include "Re_Mesh.h"
#include "Component.h"

class C_MeshRenderer : public Component
{
public:
	C_MeshRenderer(GameObject* gameObject);
	virtual ~C_MeshRenderer() override;

	void InitAsDefaultCube(float3 position = { 0.0f,0.0f,0.0f }, float3 scale = { 1.0f,1.0f,1.0f });
	void InitAsDefaultSphere(float3 position = { 0.0f,0.0f,0.0f }, float3 scale = { 1.0f,1.0f,1.0f });
	void InitAsLoadedMesh(uint mesh_id);
	void InitAsNewMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices);

	void OnPositionUpdate(float3 position) override;
	void OnScaleUpdate(float3 scale) override;
	void OnRotationUpdate(float3 rotation) override;
	void OnTransformUpdate(float3 position, float3 scale, float3 rotation) override;

	Re_Mesh& GetMesh();

private:
	uint mesh_id;
	uint instance_id;
};