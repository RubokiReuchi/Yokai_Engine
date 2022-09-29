#include "Globals.h"
#include "OpenGL.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

static const float cube_vertices[] =
{
	1.0f, 1.0f, 0.0f, // v0
	-1.0f, 1.0f, 0.0f, // v1
	-1.0f, -1.0f, 0.0f, // v2
	1.0f, -1.0f, 0.0f, // v3
	1.0f, -1.0f, -2.0f, // v4
	1.0f, 1.0f, -2.0f, // v5
	-1.0f, 1.0f, -2.0f, // v6
	-1.0f, -1.0f, -2.0f, // v7
};

static const uint cube_indices[] =
{
	0, 1, 2,
	2, 3, 0,

	0, 3, 4,
	4, 5, 0,

	0, 5, 6,
	6, 1, 0,

	1, 6, 7,
	7, 2, 1,

	6, 5, 4,
	4, 7, 6,

	3, 2, 7,
	7, 4, 3,
};

void CreateCube()
{
	uint num_vertices = 8;
	uint my_id = 0;
	glGenBuffers(1, (GLuint*)&(my_id));
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, cube_vertices, GL_STATIC_DRAW);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	uint num_indices = 36;
	uint my_indices = 0;
	glGenBuffers(1, (GLuint*)&(my_indices));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, cube_indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

	glDisableClientState(GL_VERTEX_ARRAY);
}