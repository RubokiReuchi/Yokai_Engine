#include "EO_Editor.h"
#include "ModuleEngineOrder.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "ModuleFile.h"

EO_Editor::EO_Editor()
{
}

EO_Editor::~EO_Editor()
{
}

void EO_Editor::Start()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();

	io.IniFilename = "Config/settings.ini";

	// Setup ImGui style
	

	// Setup font


	// Init OpenGL
	const char* glsl_version = "#version 130";
	ImGui_ImplSDL2_InitForOpenGL(app->window->window, app->renderer3D->context);
	ImGui_ImplOpenGL3_Init(glsl_version);
}

void EO_Editor::PreUpdate()
{}

void EO_Editor::Update()
{}


void EO_Editor::PostUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(app->window->window);
	ImGui::NewFrame();

	if (!SetMenuBar())
	{
		app->renderer3D->exit = true;
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EO_Editor::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

bool EO_Editor::SetMenuBar()
{
	bool exit = false;

	// Top Bar
	ImGui::Begin("Yokai_Engine", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New"))
			{

			}
			if (ImGui::MenuItem("Open"))
			{

			}
			if (ImGui::MenuItem("Save"))
			{

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::Checkbox("Wireframe", &app->renderer3D->wireframe)) {}
			// wireframe

			if (ImGui::Checkbox("Multi Sample", &app->renderer3D->multi_sample))
				(app->renderer3D->multi_sample) ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);

			if (ImGui::Checkbox("Depth Test", &app->renderer3D->depth_test))
				(app->renderer3D->depth_test) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);

			if (ImGui::Checkbox("Cull Face", &app->renderer3D->cull_face))
				(app->renderer3D->cull_face) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

			if (ImGui::Checkbox("Lighting", &app->renderer3D->lighting))
				(app->renderer3D->lighting) ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);

			if (ImGui::Checkbox("Color Material", &app->renderer3D->color_material))
				(app->renderer3D->color_material) ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);

			if (ImGui::Checkbox("Texture 2D", &app->renderer3D->texture_2d))
				(app->renderer3D->texture_2d) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::Checkbox("Vsync", &app->renderer3D->vsync);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About"))
			{
				show_about = !show_about;
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem("Exit"))
		{
			exit = true; // close window
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();

	// Hierarchy
	ImGui::Begin(("Hierarchy"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("test");
	ImGui::Text("test");
	ImGui::Text("test");
	ImGui::Text("test");
	ImGui::End();

	// Screen
	ImGui::Begin("Scene", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImVec2 gameDimensions = ImGui::GetContentRegionAvail();

	if (gameDimensions.x != game_width || gameDimensions.y != game_height)
	{
		// If the size of this imgui window is different from the one stored.
		game_width = gameDimensions.x;
		game_height = gameDimensions.y;

		app->renderer3D->frameBuffer.SetDimensions(game_width, game_height);
	}

	ImGui::Image((ImTextureID)app->renderer3D->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
	ImGui::End();

	// Inspector
	ImGui::Begin(("Inspector"), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::Text("test");
	ImGui::Text("test");
	ImGui::Text("test");
	ImGui::Text("test");
	ImGui::End();

	if (show_about)
	{
		if (ImGui::Begin("About"), NULL, ImGuiWindowFlags_AlwaysAutoResize)
		{
			ImGui::TextColored({ 240, 138, 0, 255 }, "Yokai Engine");
			ImGui::Text("A 3D Game Engine");
			ImGui::Text("By Ruben Ayora");
			ImGui::Text("");
			ImGui::Text("3rd Party Libraries used:");
			ImGui::Bullet(); if (ImGui::Button("Glew 2.1.0.0")) OsOpenInShell("https://github.com/nigels-com/glew");
			ImGui::Bullet(); if (ImGui::Button("ImGui 1.88")) OsOpenInShell("https://github.com/ocornut/imgui");
			ImGui::Bullet(); if (ImGui::Button("Json parser 1.1.0")) OsOpenInShell("https://github.com/json-parser/json-parser");
			ImGui::Bullet(); if (ImGui::Button("MathGeoLib 1.15")) OsOpenInShell("https://github.com/juj/MathGeoLib");
			ImGui::Bullet(); if (ImGui::Button("SDL 2.0.12.0")) OsOpenInShell("https://github.com/libsdl-org/SDL");
			ImGui::Text("MIT License\n\nCopyright(c) 2022 RubokiReuchi\n\nPermission is hereby granted, free of charge, to any person obtaining a copy\nof this softwareand associated documentation files(the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and /or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions :\nThe above copyright noticeand this permission notice shall be included in all\ncopies or substantial portions of the Software.\nTHE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.");
			ImGui::End();
		}
	}

	return !exit;
}

void EO_Editor::OsOpenInShell(const char* path)
{
#ifdef _WIN32
	// Note: executable path must use backslashes!
	::ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);
#else
#if __APPLE__
	const char* open_executable = "open";
#else
	const char* open_executable = "xdg-open";
#endif
	char command[256];
	snprintf(command, 256, "%s \"%s\"", open_executable, path);
	system(command);
#endif
}