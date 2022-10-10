#pragma once

#include "Globals.h"
#include "Color.h"
#include "MathGeoLib/include/MathGeoLib.h"

#include <vector>

struct Vertex
{
	Vertex() {}
	Vertex(float3 pos, float3 norm, float2 texC) : position(pos), normals(norm), tex_coords(texC) {}
	float3 position;
	float3 normals;
	float2 tex_coords;
	float texture_id;
};

class Mesh
{
public:
	Mesh();
	~Mesh();

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

	void SetUpdateTrue() { update_matrix = true; } // Debugging function! Should be deleted.

	float3 rotation;
	float3 scale;
	float3 position;

	std::vector<Vertex>* vertices = nullptr;
	std::vector<uint>* indices = nullptr;
	float4x4 model_matrix = model_matrix.identity;

	float OpenGL_texture_id = -1;
	float texture_id = -1;
private:
	bool update_matrix = false;
};