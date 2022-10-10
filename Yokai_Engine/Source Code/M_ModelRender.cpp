#include "M_ModelRender.h"
#include "DevIL/include/il.h"
#include "DevIL/include/ilut.h"

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
	for (auto& obj : render_map)
	{
		obj.second.Draw();
	}
}