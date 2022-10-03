#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "FrameBuffer.h"
#include "GeometricForms.h"

#include "MeshImporter.h"

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

	FrameBuffer frameBuffer;

	SphereForm sphere;
	CubeForm cube;
	PyramidForm pyramid;
};