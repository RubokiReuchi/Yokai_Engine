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

	uint SetMeshInformation(Re_Mesh& mesh);

	void Draw();

	uint AddMesh(Re_Mesh& mesh);

	std::map<uint, Re_Mesh>& GetMap() { return meshes; };

public:
	bool initialized = false;
private:
	void CreateBuffers();
	void CreateNormalsDisplayBuffer();

	void DrawVertexNormals(int modelMatrixIndex);
	void DrawFaceNormals(int modelMatrixIndex);

	Re_Shader* basic_shader = nullptr;
	Re_Shader* line_shader = nullptr;

private:
	std::map<uint, Re_Mesh> meshes;
	std::vector<Vertex> total_vertices;
	std::vector<uint> total_indices;
	std::vector<float4x4> model_matrices;
	std::vector<float> texture_ids;

	std::vector<float3> vertexNormalsDisplay;
	std::vector<float3> faceNormalsDisplay;

	uint VAO = 0; // Vertex Array
	uint VBO = 0; // Vertex buffer
	uint IBO = 0; // Elements buffer object
	uint MBO = 0; // ModelMatrix buffer object
	uint TBO = 0; // TextureID buffer object 

	uint VertexLineVAO = 0; // Lines to display Vertex Normals
	uint VertexLineVBO = 0;

	uint FaceLineVAO = 0; // Lines to display Face Normals
	uint FaceLineVBO = 0;

	bool drawVertexNormals = false;
	bool drawFaceNormals = true;

	int id_counter = 0;
};