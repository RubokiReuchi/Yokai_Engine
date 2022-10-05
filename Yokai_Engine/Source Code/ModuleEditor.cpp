#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "OpenGL.h"

ModuleEditor::ModuleEditor(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

// Destructor
ModuleEditor::~ModuleEditor()
{}

// Called before render is available
bool ModuleEditor::Init()
{
	bool ret = true;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	ImGui::StyleColorsDark();

	io.IniFilename = "Config/settings.ini";

	return ret;
}

// PostUpdate present buffer to screen
update_status ModuleEditor::PostUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	if (!SetMenuBar()) return UPDATE_STOP;

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEditor::CleanUp()
{
	LOG("Destroying Editor");

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	return true;
}

bool ModuleEditor::SetMenuBar()
{
	ImGui::Begin("Game_Render", NULL, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
	ImGui::BeginChild("Game_Render", ImVec2(SCREEN_WIDTH, SCREEN_HEIGHT));
	ImVec2 size = ImGui::GetWindowSize();
	ImGui::Image((ImTextureID)App->renderer3D->frameBuffer.GetTextureBuffer(), size, ImVec2(0, 1), ImVec2(1, 0));
	ImGui::EndChild();
	ImGui::End();

	ImGui::Begin("Yokai_Engine", NULL, ImGuiWindowFlags_MenuBar);
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
			if (ImGui::Checkbox("Wireframe", &App->renderer3D->wireframe)) {}
				// wireframe

			if (ImGui::Checkbox("Multi Sample", &App->renderer3D->multi_sample))
				(App->renderer3D->multi_sample) ? glEnable(GL_MULTISAMPLE) : glDisable(GL_MULTISAMPLE);

			if (ImGui::Checkbox("Depth Test", &App->renderer3D->depth_test))
				(App->renderer3D->depth_test) ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);

			if (ImGui::Checkbox("Cull Face", &App->renderer3D->cull_face))
				(App->renderer3D->cull_face) ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

			if (ImGui::Checkbox("Lighting", &App->renderer3D->lighting))
				(App->renderer3D->lighting) ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);

			if (ImGui::Checkbox("Color Material", &App->renderer3D->color_material))
				(App->renderer3D->color_material) ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);

			if (ImGui::Checkbox("Texture 2D", &App->renderer3D->texture_2d))
				(App->renderer3D->texture_2d) ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Settings"))
		{
			ImGui::Checkbox("Vsync", &App->renderer3D->vsync);
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
			return false; // close window
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();

	if (show_about)
	{
		if (ImGui::Begin("About"), 0, ImGuiWindowFlags_AlwaysAutoResize);
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

	return true;
}

void ModuleEditor::OsOpenInShell(const char* path)
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