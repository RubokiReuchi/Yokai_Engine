#pragma once

#include "Globals.h"
#include "ModuleFile.h"
#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <vector>

struct VertexInfo // contain all vertex info
{
	VertexInfo() {}
	VertexInfo(float3 pos, float3 norm, float2 texC) : position(pos), normals(norm), tex_coords(texC) {}
	float3 position; // 0, 1, 2
	float3 normals; // 3, 4, 5
	float2 tex_coords; // 6, 7
	float texture_id;
};

class Re_Mesh
{
public:
	Re_Mesh();
	~Re_Mesh();

	void InitMeshTransform(float3 position, float3 scale);
	void InitMeshVertex(std::vector<VertexInfo>* vertices, std::vector<uint>* indices);
	void InitMeshInfo(std::vector<VertexInfo>& vertices, std::vector<uint>& indices, float3 pos, float3 scale);

	void Update();

	void CleanUp();

	void SetPosition(float3 pos);
	void SetScale(float3 s);
	void SetRotation(float3 rot);
	void SetTransform(float3 pos, float3 s, float3 rot);

	float3 position;
	float3 rotation;
	float3 scale;

	std::vector<VertexInfo>* vertices = nullptr;
	std::vector<uint>* indices = nullptr;
	float4x4 model_matrix = model_matrix.identity;
	float4x4 outline_matrix = outline_matrix.identity;

	float GL_id = -1;
	float texture_id = -1;

	// render attributes
	bool visible = true;
	bool visible_on_editor = true;
	bool in_camera = true;

	bool is_outlined = false;
	bool is_outlined_child = false;

private:
	bool update_matrix = true;
};