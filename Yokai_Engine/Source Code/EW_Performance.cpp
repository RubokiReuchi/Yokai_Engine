#include "EW_Performance.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "ModuleInput.h"
#include "ModuleFile.h"

EW_Performance::EW_Performance()
{
	window_name = "Performance";
	enabled = true;

	for (size_t i = 0; i < 60; i++) frame_array.push_back(0.0f);
	frames = &frame_array[0];

	countCPU = SDL_GetCPUCount();

	systemRAM = (float)SDL_GetSystemRAM();

	windowWidth = &app->window->width;

	windowHeight = &app->window->height;

	isVSyncOn = &app->renderer3D->vsync;

	frameLimit = &app->fpsCap;
}

EW_Performance::~EW_Performance()
{
	
}

void EW_Performance::Update()
{
	// Performance
	std::string framerate = "Framerate: " + std::to_string(ImGui::GetIO().Framerate * 0.5f);

	for (size_t i = 0; i < 59; i++)
	{
		frame_array[i] = frame_array[i + 1];
	}
	frame_array[59] = ImGui::GetIO().Framerate * 0.5f;
	frames = &frame_array[0];

	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoCollapse);
	if (ImGui::CollapsingHeader("Frame Rate", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PlotHistogram("##Framerate", frames, 60, 0, framerate.c_str(), 0.0f, 160.0f, ImVec2(300, 160));
		if (ImGui::SliderInt("FPS Limit", frameLimit, 30, 120))
		{
			app->SetFPS(*frameLimit);
		}
	}
	if (ImGui::CollapsingHeader("Window", ImGuiTreeNodeFlags_DefaultOpen))
	{
		std::string w_size = std::to_string(app->window->width) + "x" + std::to_string(app->window->height);
		std::string display_options[] = { "1280x720", "1920x1080", "2560x1440", "3840x2160" };
		ImGui::TextWrapped("Window Size:"); ImGui::SameLine();
		if (ImGui::BeginCombo("##Window Size", w_size.c_str()))
		{
			for (size_t i = 0; i < 4; i++)
			{
				bool is_selected = (w_size == display_options[i]);
				if (ImGui::Selectable(display_options[i].c_str(), is_selected))
				{
					size_t npos = display_options[i].find_last_of("x") + 1;
					std::string value = display_options[i];
					app->window->height = stoi(value.substr(npos));
					app->window->width = stoi(value.erase(npos, 9));
					if (is_selected) ImGui::SetItemDefaultFocus();
					SDL_SetWindowSize(app->window->window, app->window->width, app->window->height);
					SDL_DisplayMode DM;
					SDL_GetCurrentDisplayMode(0, &DM);
					SDL_SetWindowPosition(app->window->window, (DM.w - app->window->width) / 2, (DM.h - app->window->height) / 2);
				}
			}
			ImGui::EndCombo();
		}

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