#include "EW_Hierarchy.h"

EW_Hierarchy::EW_Hierarchy()
{
	window_name = "Hierarchy";
	enabled = true;
}

EW_Hierarchy::~EW_Hierarchy()
{
}

void EW_Hierarchy::Update()
{
	// Hierarchy
	if (ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse))
	{
        // example
		if (ImGui::TreeNode("Root"))
        {
            for (int i = 0; i < 8; i++)
            {
                if (i == 0)  ImGui::SetNextItemOpen(true, ImGuiCond_Once);

                if (ImGui::TreeNode((void*)(intptr_t)i, "GameObject %d", i))
                {
                    ImGui::TreePop();
                }
            }
            ImGui::TreePop();
        }
		ImGui::End();
	}
}