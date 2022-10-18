#include "Re_Mesh.h"
#include "M_Texture.h"

#define _USE_MATH_DEFINES

#include <math.h>

inline void push_indices(std::vector<uint>& indices, int sectors, int r, int s)
{
	int curRow = r * sectors;
	int nextRow = (r + 1) * sectors;
	int nextS = (s + 1) % sectors;

	indices.push_back(curRow + s);
	indices.push_back(nextRow + s);
	indices.push_back(nextRow + nextS);

	indices.push_back(curRow + s);
	indices.push_back(nextRow + nextS);
	indices.push_back(curRow + nextS);
}


Re_Mesh::Re_Mesh()
{
	model_matrix.SetIdentity();
}

Re_Mesh::~Re_Mesh()
{
}

void Re_Mesh::InitAsMeshInformation(float3 position, float3 scale)
{
	this->position = position;
	this->scale = scale;
}

void Re_Mesh::InitAsCube(float3 position, float3 scale)
{
	this->position = position;
	this->scale = scale;

	vertices = new std::vector<Vertex>(8);

	vertices->at(0).position = float3(-0.5, 0.5, 0.5);
	vertices->at(1).position = float3(-0.5, -0.5, 0.5);
	vertices->at(2).position = float3(0.5, 0.5, 0.5);
	vertices->at(3).position = float3(0.5, -0.5, 0.5);
	vertices->at(4).position = float3(-0.5, 0.5, -0.5);
	vertices->at(5).position = float3(-0.5, -0.5, -0.5);
	vertices->at(6).position = float3(0.5, 0.5, -0.5);
	vertices->at(7).position = float3(0.5, -0.5, -0.5);

	indices = new std::vector<uint>
	{
	  0, 2, 3, 0, 3, 1,
	  2, 6, 7, 2, 7, 3,
	  6, 4, 5, 6, 5, 7,
	  4, 0, 1, 4, 1, 5,
	  0, 4, 6, 0, 6, 2,
	  1, 5, 7, 1, 7, 3,
	};
}

void Re_Mesh::InitAsSphere(float3 position, float3 scale)
{
	this->position = position;
	this->scale = scale;

	int rings = 12;
	int sectors = 24;
	int radius = 1;

	float const R = 1. / (float)(rings - 1);
	float const S = 1. / (float)(sectors - 1);

	vertices = new std::vector<Vertex>(rings * sectors);
	indices = new std::vector<uint>(rings * sectors * 4);

	int counter = 0;

	for (int r = 0; r < rings; r++) for (int s = 0; s < sectors; s++)
	{
		float const y = sin(-M_PI_2 + M_PI * r * R);
		float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

		vertices->at(counter).position.x = x * radius;
		vertices->at(counter).position.y = y * radius;
		vertices->at(counter).position.z = z * radius;
		counter++;
		if (r < rings - 1) push_indices(*indices, sectors, r, s);
	}
}

void Re_Mesh::Update()
{
	if (M_Texture::loaded_textures.find(texture_id) != M_Texture::loaded_textures.end()) // check if texture is loaded
	{
		OpenGL_texture_id = M_Texture::BindTexture(texture_id);
	}

	if (!update_matrix) return;

	// Update Model matrix. This information will be used later by the RenderManager.
	model_matrix.SetIdentity();

	math::Quat rot = rot.FromEulerXYZ(math::DegToRad(rotation.x), math::DegToRad(rotation.y), math::DegToRad(rotation.z));
	rot.Normalize();

	model_matrix = float4x4::FromTRS(position, rot, scale);

	model_matrix.Transpose();

	update_matrix = false;
}

void Re_Mesh::InitAsMesh(std::vector<Vertex>& vertices, std::vector<uint>& indices, float3 pos, float3 scale)
{
	this->position = pos;
	this->scale = scale;

	this->vertices = new std::vector<Vertex>(vertices);
	this->indices = new std::vector<uint>(indices);

}

void Re_Mesh::CleanUp()
{
	RELEASE(vertices);
	RELEASE(indices);
}

void Re_Mesh::SetPosition(float3 pos)
{
	this->position = pos;
	update_matrix = true;
}

void Re_Mesh::SetScale(float3 s)
{
	this->scale = s;
	update_matrix = true;
}

void Re_Mesh::SetRotation(float3 rot)
{
	this->rotation = rot;
	update_matrix = true;
}

void Re_Mesh::SetTransform(float3 pos, float3 s, float3 rot)
{
	this->position = pos;
	this->scale = s;
	this->rotation = rot;
	update_matrix = true;
}