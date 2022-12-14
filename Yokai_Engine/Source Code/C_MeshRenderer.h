#pragma once

#include "Re_Mesh.h"
#include "Component.h"

class C_MeshRenderer : public Component
{
public:
	C_MeshRenderer(GameObject* gameObject);
	virtual ~C_MeshRenderer() override;

	void Update();

	void OnEditor();

	void InitAsInstanciedMesh(uint mesh_id);
	void InitAsNewMesh(std::vector<VertexInfo>& vertices, std::vector<uint>& indices, std::string mesh_path);

	Re_Mesh& GetMesh();
	uint GetMeshID()
	{
		return mesh_id;
	}

	std::string GetMeshPath();
	std::string GetTexture(float texture_id);

private:
	uint mesh_id;
	uint instance_id;

	bool show_aabb = false;
	bool show_obb = false;
	std::vector<float3> PointsToLines_AABB(float3 points[8]);

	friend class Serialization;
};