#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"

#include"MathGeoLib/include/Geometry/AABB.h"
#include"MathGeoLib/include/Geometry/OBB.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

public:

	bool vsync, wireframe, multi_sample, depth_test, cull_face, lighting, color_material, texture_2d;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};

/*static const float cube_vertices[] =
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
};*/