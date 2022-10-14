#pragma once

#include "M_Render.h"
#include "M_Texture.h"
#include "Globals.h"

#include <map>

class M_ModelRender
{
public:
	M_ModelRender();
	~M_ModelRender();

	void Init();

	M_Render* GetRenderManager(uint id);
	uint GetMapSize() { return render_map.size(); };

	void Draw();

private:
	std::map<uint, M_Render> render_map;
	M_Texture* texture_manager = nullptr;
};