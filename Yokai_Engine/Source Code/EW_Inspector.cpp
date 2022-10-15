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
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	if (selectGameobject)
	{

	}
	ImGui::End();
}