#include "EW_Performance.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleFile.h"
#include "CycleArray.hpp"

EW_Performance::EW_Performance()
{
	window_name = "Performance";
	enabled = true;

	frames = new CArrayF(60, 0.0f);

	countCPU = SDL_GetCPUCount();

	systemRAM = (float)SDL_GetSystemRAM();

	windowWidth = &app->window->width;

	windowHeight = &app->window->height;

	isVSyncOn = &app->renderer3D->vsync;

	frameLimit = &app->fpsCap;
}

EW_Performance::~EW_Performance()
{
	RELEASE(frames);
}

void EW_Performance::Update()
{
	// Performance
	std::string framerate = "Framerate: " + std::to_string(ImGui::GetIO().Framerate);

	frames->push_back(ImGui::GetIO().Framerate);

	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	if (ImGui::CollapsingHeader("Frame Rate", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PlotHistogram("##Framerate", frames->front(), frames->size(), 0, framerate.c_str(), 0.0f, 160.0f, ImVec2(300, 160));
		if (ImGui::SliderInt("FPS Limit", frameLimit, 30, 120))
		{
			app->SetFPS(*frameLimit);
		}
	}
	if (ImGui::CollapsingHeader("Window", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextWrapped("Window Width:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(*windowWidth).c_str());

		ImGui::TextWrapped("Window Height:"); ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(*windowHeight).c_str()); ImGui::SameLine();
		ImGui::Spacing();
		ImGui::Checkbox("VSync", isVSyncOn);
		app->renderer3D->ToggleVSync(*isVSyncOn);
	}
	if (ImGui::CollapsingHeader("Hardware", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::TextWrapped("CPU Count: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(countCPU).c_str());

		ImGui::TextWrapped("RAM: "); ImGui::SameLine();
		ImGui::TextColored(ImVec4(255, 255, 0, 255), std::to_string(systemRAM).c_str());

		ImGui::TextWrapped("--------OpenGL-------- ");
		ImGui::TextWrapped("Vendor: %s", glGetString(GL_VENDOR));
		ImGui::TextWrapped("Renderer: %s", glGetString(GL_RENDERER));
		ImGui::TextWrapped("OpenGL version supported: %s", glGetString(GL_VERSION));
		ImGui::TextWrapped("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	}
	ImGui::End();
}