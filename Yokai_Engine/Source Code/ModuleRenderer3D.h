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

static const float cube_vertices[] =
{
	1.0f, 1.0f, 0.0f, // v0-v1-v2
	-1.0f, 1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f,

	-1.0f, -1.0f, 0.0f, // v2-v3-v0
	1.0f, -1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	// right face
	1.0f, 1.0f, 0.0f, // v0-v3-v4
	1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, -2.0f,

	1.0f, -1.0f, -2.0f, // v4-v5-v0
	1.0f, 1.0f, -2.0f,
	1.0f, 1.0f, 0.0f,

	// top face
	1.0f, 1.0f, 0.0f, // v0-v5-v6
	1.0f, 1.0f, -2.0f,
	-1.0f, 1.0f, -2.0f,

	-1.0f, 1.0f, -2.0f, // v6-v1-v0
	-1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,

	// left face
	-1.0f, 1.0f, 0.0f, // v1-v6-v7
	-1.0f, 1.0f, -2.0f,
	-1.0f, -1.0f, -2.0f,

	-1.0f, -1.0f, -2.0f, // v7-v2-v1
	-1.0f, -1.0f, 0.0f,
	-1.0f, 1.0f, 0.0f,

	// back face
	-1.0f, 1.0f, -2.0f, // v6-v5-v4
	1.0f, 1.0f, -2.0f,
	1.0f, -1.0f, -2.0f,

	1.0f, -1.0f, -2.0f, // v4-v7-v6
	-1.0f, -1.0f, -2.0f,
	-1.0f, 1.0f, -2.0f,

	// down face
	1.0f, -1.0f, 0.0f, // v3-v2-v7
	-1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, -2.0f,

	-1.0f, -1.0f, -2.0f, // v7-v4-v3
	1.0f, -1.0f, -2.0f,
	1.0f, -1.0f, 0.0f,
};