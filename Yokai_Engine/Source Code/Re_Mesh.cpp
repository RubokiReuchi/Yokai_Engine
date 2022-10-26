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
	this->rotation = float3(0.0f, 0.0f, 0.0f);
	this->scale = scale;
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