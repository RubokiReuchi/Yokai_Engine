#pragma once

#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "M_ModelRender.h"
#include "MeshImporter.h"

#include"MathGeoLib/include/Geometry/AABB.h"
#include"MathGeoLib/include/Geometry/OBB.h"

#define MAX_LIGHTS 8
#define MAX_LINES 999

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void ToggleVSync(bool is_on);

	void AddLines(std::vector<float3> lines, float4 color);

private:

	void DrawLines(float4 color, size_t iteration);

	void InitAABB_Buffer();
	void UpdateAABB_Buffer(std::vector<float3> lines);
	Re_Shader* line_shader = nullptr;
	uint linesVAO = 0;
	uint linesVBO = 0;

	std::map<uint, std::vector<float3>> drawing_lines;
	std::map<uint, float4> lines_colors;

public:

	bool vsync, wireframe, cull_face, lighting, color_material, texture_2d;

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat4x4 ModelMatrix, ViewMatrix;

	M_ModelRender model_render;
	bool drawing_scene = true;

	// trigger exit
	bool exit;

	// FOR second deliver
	bool view_frustrum_culling = false;
};