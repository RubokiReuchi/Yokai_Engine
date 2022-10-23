#pragma once

#include "Globals.h"
#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <vector>

struct Vertex
{
	Vertex() {}
	Vertex(float3 pos, float3 norm, float2 texC) : position(pos), normals(norm), tex_coords(texC) {}
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

	void InitAsMeshInformation(float3 position, float3 scale);
	void InitAsCube(float3 position, float3 scale);
	void InitAsSphere(float3 position, float3 scale);

	void InitAsMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices, float3 pos, float3 scale);

	void Update();

	void CleanUp();

	void SetPosition(float3 pos);
	void SetScale(float3 s);
	void SetRotation(float3 rot);

	void SetTransform(float3 pos, float3 s, float3 rot);

	float3 position;
	float3 rotation;
	float3 scale;

	std::vector<Vertex>* vertices = nullptr;
	std::vector<uint>* indices = nullptr;
	float4x4 model_matrix = model_matrix.identity;

	float OpenGL_texture_id = -1;
	uint texture_id = -1;

	bool visible = true;
	bool visible_on_editor = true;
private:
	bool update_matrix = true;
};