#include "MeshImporter.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

void MeshImporter::EnableDebugMode()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

void MeshImporter::DisableDebugMode()
{
	aiDetachAllLogStreams();
}

void MeshImporter::LoadMesh(char* file_path, StoredMesh* myMesh)
{
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		aiReleaseImport(scene);
	}
	else
	{
		LOG("Error loading scene % s", file_path);
	}

	for (size_t i = 0; i < scene->mNumMeshes; i++)
	{
		myMesh->num_vertex = scene->mMeshes[i]->mNumVertices;
		myMesh->vertex = new float[myMesh->num_vertex * 3];
		memcpy(myMesh->vertex, scene->mMeshes[i]->mVertices, sizeof(float) * myMesh->num_vertex * 3);
		LOG("New mesh with %d vertices", myMesh->num_vertex);

		if (scene->mMeshes[i]->HasFaces())
		{
			myMesh->num_index = scene->mMeshes[i]->mNumFaces * 3;
			myMesh->index = new uint[myMesh->num_index]; // assume each face is a triangle

			for (size_t j = 0; j < scene->mMeshes[i]->mNumFaces; j++)
			{
				if (scene->mMeshes[i]->mFaces[j].mNumIndices != 3)
				{
					LOG("WARNING, geometry face with != 3 indices!");
				}
				else
				{
					memcpy(&myMesh->index[j * 3], scene->mMeshes[i]->mFaces[j].mIndices, 3 * sizeof(uint));
				}
			}
		}
	}
}

void MeshImporter::CreateMesh(StoredMesh myMesh)
{
	glGenBuffers(1, (GLuint*)&(myMesh.id_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, myMesh.id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * myMesh.num_vertex * 3, myMesh.vertex, GL_STATIC_DRAW);

	glGenBuffers(1, (GLuint*)&(myMesh.id_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myMesh.id_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * myMesh.num_index, myMesh.index, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myMesh.id_index);
}

void MeshImporter::RenderMesh(StoredMesh myMesh)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawElements(GL_TRIANGLES, myMesh.num_vertex, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
}