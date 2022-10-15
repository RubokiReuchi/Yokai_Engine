#include "EW_Inspector.h"

EW_Inspector::EW_Inspector()
{
	window_name = "Inspector";
	enabled = true;
}

EW_Inspector::~EW_Inspector()
{
}

void EW_Inspector::Update()
{
	// Inspector
	if (ImGui::Begin(window_name.c_str(), &enabled))
	{
		if (selectGameobject)
		{

		}
		ImGui::End();
	}
}