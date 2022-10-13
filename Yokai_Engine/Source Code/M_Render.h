#pragma once

#include "Globals.h"
#include "Re_Mesh.h"
#include "Re_Shader.h"

#include <map>

/// <summary>
/// This class manages the rendering process of a single MeshObject.
/// A MeshObject is a colection of Meshes that follow the same vertices/indices pattern.
/// TLDR: This class instance renders a MeshObject.
/// </summary>
class M_Render
{
public:
	M_Render();
	~M_Render();

	uint SetMeshInformation(Re_Mesh& mesh);

	void Draw();

	uint AddMesh(Re_Mesh& mesh);

	std::map<uint, Re_Mesh>& GetMap() { return meshes; };

public:
	bool initialized = false;
private:
	void CreateBuffers();
	Re_Shader* basic_shader = nullptr;

private:
	std::map<uint, Re_Mesh> meshes;
	std::vector<Vertex> total_vertices;
	std::vector<uint> total_indices;
	std::vector<float4x4> model_matrices;
	std::vector<float> texture_ids;

	uint VAO = 0; // Vertex Array
	uint VBO = 0; // Vertex buffer
	uint IBO = 0; // Elements buffer object
	uint MBO = 0; // ModelMatrix buffer object
	uint TBO = 0; // TextureID buffer object 

	int id_counter = 0;
};