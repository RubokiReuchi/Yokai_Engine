#include "M_ModelRender.h"
#include "Application.h"
#include "MeshImporter.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"

#pragma comment (lib, "DevIL/libx86/DevIL.lib")
#pragma comment (lib, "DevIL/libx86/ILU.lib")
#pragma comment (lib, "DevIL/libx86/ILUT.lib")

M_ModelRender::M_ModelRender()
{
}

M_ModelRender::~M_ModelRender()
{
	RELEASE(texture_manager);
}

void M_ModelRender::Init()
{
	ilInit();
	ilutInit();
	ilutRenderer(ILUT_OPENGL);
	texture_manager = new M_Texture();
}

M_Render* M_ModelRender::GetRenderManager(uint id)
{
	return &render_map[id];
}

void M_ModelRender::Draw()
{
	for (auto& m_render : render_map)
	{
		m_render.second.Draw();
	}
}

void M_ModelRender::CreatePrimitive(GameObject* parent, PrimitiveType type)
{
	if (parent == nullptr) parent = app->engine_order->rootGameObject;

	switch (type)
	{
		case PrimitiveType::PLANE:
		{
			GameObject* plane = MeshImporter::LoadMesh("Assets/plane.fbx");
			plane->SetParent(parent);
			plane->name = "Plane";
			break;
		}
		case PrimitiveType::CUBE:
		{
			GameObject* cube = MeshImporter::LoadMesh("Assets/cube.fbx");
			cube->SetParent(parent);
			cube->name = "Cube";
			break;
		}
		case PrimitiveType::SPHERE:
		{
			GameObject* sphere = MeshImporter::LoadMesh("Assets/sphere.fbx");
			sphere->SetParent(parent);
			sphere->name = "Sphere";
			break;
		}
		case PrimitiveType::CYLINDER:
		{
			GameObject* cylinder = MeshImporter::LoadMesh("Assets/cylinder.fbx");
			cylinder->SetParent(parent);
			cylinder->name = "Cylinder";
			break;
		}
		case PrimitiveType::CONE:
		{
			GameObject* cone = MeshImporter::LoadMesh("Assets/cone.fbx");
			cone->SetParent(parent);
			cone->name = "Cone";
			break;
		}
	}
}