#pragma once

#include "Globals.h"
#include "Re_Mesh.h"
#include "Re_Shader.h"

#include <map>

class M_Render
{
public:
	M_Render();
	~M_Render();

	uint InitManageRender(Re_Mesh mesh);

	void Draw();

	uint AddMesh(Re_Mesh& mesh);

	std::map<uint, Re_Mesh>& GetMap()
	{
		return meshes;
	}

	std::vector<VertexInfo> GetVertices()
	{
		return total_vertices;
	}

	std::string GetMeshPath()
	{
		return mesh_path;
	}

public:
	bool initialized = false;
	std::string mesh_path;
private:
	void CreateMeshBuffers();

	Re_Shader* basic_shader = nullptr;
	Re_Shader* outline_shader = nullptr;

private:
	// M_Render meshes info
	std::map<uint, Re_Mesh> meshes;
	std::vector<VertexInfo> total_vertices;
	std::vector<uint> total_indices;
	std::vector<float4x4> model_matrices;
	std::vector<float> texture_ids;

	// Bind variables
	uint VAO = 0; // Vertex Array Object
	uint VBO = 0; // Vertex Buffer Object
	uint IBO = 0; // Inices Buffer Object
	uint MBO = 0; // ModelMatrix Buffer Object
	uint TBO = 0; // TextureID Buffer Object

	int id_counter = 0;

	// outlined meshes
	std::vector<uint> selected_total_indices;
	std::vector<float4x4> selected_model_matrices;
};