#include "Globals.h"
#include "OpenGL.h"

#include <vector>
#include <cmath>

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

class CubeForm
{
protected:
	uint num_vertices = 8;
	uint num_indices = 36;

	uint my_id = 0;
	uint my_indices = 0;

public:
	void CreateCube()
	{
		glGenBuffers(1, (GLuint*)&(my_id));
		glBindBuffer(GL_ARRAY_BUFFER, my_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, cube_vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, (GLuint*)&(my_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, cube_indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void DrawCube()
	{
		glBindBuffer(GL_ARRAY_BUFFER, my_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glPushMatrix();
		glTranslatef(2.0f, 0.0f, 0.0f);

		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

		glPopMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
	}
};

static const float pyramid_vertices[] =
{
	1.0f, -1.0f, 0.0f, // v0
	-1.0f, -1.0f, 0.0f, // v1
	0.0f, -1.0f, -2.0f, // v2
	0.0f, 2.0f, -1.0f, // v3
};

static const uint pyramid_indices[] =
{
	0, 1, 2,
	0, 3, 1,

	0, 2, 3,
	1, 3, 2,
};

class PyramidForm
{
protected:
	uint num_vertices = 4;
	uint num_indices = 12;

	uint my_id = 0;
	uint my_indices = 0;

public:
	void CreatePyramid()
	{
		glGenBuffers(1, (GLuint*)&(my_id));
		glBindBuffer(GL_ARRAY_BUFFER, my_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices * 3, pyramid_vertices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, (GLuint*)&(my_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * num_indices, pyramid_indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void DrawPyramid()
	{
		glBindBuffer(GL_ARRAY_BUFFER, my_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glPushMatrix();
		glTranslatef(-2.0f, 0.0f, 0.0f);

		glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_INT, NULL);

		glPopMatrix();

		glDisableClientState(GL_VERTEX_ARRAY);
	}
};

class SphereForm
{
protected:
	std::vector<GLfloat> sphere_vertices;
	std::vector<GLushort> sphere_indices;

	uint my_id = 0;
	uint my_indices = 0;

public:
	void CreateSphere(float radius, unsigned int rings, unsigned int sectors)
	{
		std::vector<GLfloat>().swap(sphere_vertices);

		float const R = 1. / (float)(rings - 1);
		float const S = 1. / (float)(sectors - 1);
		int r, s;

		sphere_vertices.resize(rings * sectors * 3);
		std::vector<GLfloat>::iterator v = sphere_vertices.begin();
		for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
			float const y = sin(-M_PI_2 + M_PI * r * R);
			float const x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
			float const z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);

			*v++ = x * radius;
			*v++ = y * radius;
			*v++ = z * radius;
		}

		sphere_indices.resize(rings * sectors * 4);
		std::vector<GLushort>::iterator i = sphere_indices.begin();
		for (r = 0; r < rings; r++) for (s = 0; s < sectors; s++) {
			*i++ = r * sectors + s;
			*i++ = r * sectors + (s + 1);
			*i++ = (r + 1) * sectors + (s + 1);
			*i++ = (r + 1) * sectors + s;
		}

		glGenBuffers(1, (GLuint*)&(my_id));
		glBindBuffer(GL_ARRAY_BUFFER, my_id);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * sphere_vertices.size(), sphere_vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, (GLuint*)&(my_indices));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * sphere_indices.size(), sphere_indices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	void DrawSphere()
	{
		glBindBuffer(GL_ARRAY_BUFFER, my_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);

		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		glDrawElements(GL_QUADS, sphere_indices.size(), GL_UNSIGNED_SHORT, NULL);

		glDisableClientState(GL_VERTEX_ARRAY);
	}
};