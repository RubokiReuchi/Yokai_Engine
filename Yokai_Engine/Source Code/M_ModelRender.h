#pragma once

#include "M_Render.h"
#include "M_Texture.h"
#include "Globals.h"

#include <map>

/// <summary>
/// This class contains a colletion of RenderManagers. It dynamically creates and destroys Render Managers tu fullfill the task of having one per Unique mesh.
/// Every Render Manager updates and draws their corresponding Models.
/// </summary>
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