#include "EO_Editor.h"
#include "ModuleEngineOrder.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderer3D.h"
#include "Serialization.h"

#include "EW_Hierarchy.h"
#include "EW_Game.h"
#include "EW_Scene.h"
#include "EW_About.h"
#include "EW_Inspector.h"
#include "EW_Performance.h"
#include "EW_Project.h"
#include "EW_Console.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "IconsFontAwesome.h"

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
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 10.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.0f;
		style.Colors[ImGuiCol_Border] = ImVec4(255, 1, 1, 255);
	}

	io.IniFilename = "Config/settings.ini";
	io.ConfigWindowsMoveFromTitleBarOnly;

	// Setup ImGui style
	

	// Setup font
	default_font = io.Fonts->AddFontDefault();
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig icons_config; icons_config.MergeMode = true; icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF(FONT_ICON_FILE_NAME_FAS, 10.0f, &icons_config, icons_ranges); // icons font
	arial_font_30 = io.Fonts->AddFontFromFileTTF("Assets/Fonts/arial-regular.ttf", 30.0f);

	// Init OpenGL
	const char* glsl_version = "#version 130";
	ImGui_ImplSDL2_InitForOpenGL(app->window->window, app->renderer3D->context);
	ImGui_ImplOpenGL3_Init(glsl_version);

	// Create Editor Windows
	editor_windows[(uint)EW_TYPE::HIERARCHY] = new EW_Hierarchy();
	editor_windows[(uint)EW_TYPE::ABOUT] = new EW_About();
	editor_windows[(uint)EW_TYPE::GAME] = new EW_Game();
	editor_windows[(uint)EW_TYPE::SCENE] = new EW_Scene();
	editor_windows[(uint)EW_TYPE::PERFORMANCE] = new EW_Performance();
	editor_windows[(uint)EW_TYPE::INSPECTOR] = new EW_Inspector();
	editor_windows[(uint)EW_TYPE::CONSOLE] = new EW_Console();
	editor_windows[(uint)EW_TYPE::PROJECT] = new EW_Project();
}

void EO_Editor::PreUpdate()
{}

void EO_Editor::Update()
{
	// will be moved to scene
	std::vector<float3> grid;
	float grid_radius = 10;
	for (float i = -grid_radius; i <= grid_radius; i++)
	{
		grid.push_back(float3(i, 0, grid_radius));
		grid.push_back(float3(i, 0, -grid_radius));
		grid.push_back(float3(grid_radius, 0, i));
		grid.push_back(float3(-grid_radius, 0, i));
	}

	app->renderer3D->AddLines(grid, float4(1.0f, 1.0f, 1.0f, 1.0f));

	for (auto& go : app->engine_order->game_objects)
	{
		go.second->Update();
	}
}

void EO_Editor::PostUpdate()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(app->window->window);
	ImGui::NewFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	if (!SetMenuBar())
	{
		app->renderer3D->exit = true;
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void EO_Editor::CleanUp()
{
	for (int i = 0; i < (uint)EW_TYPE::NUM_EW_TYPE; i++)
	{
		RELEASE(editor_windows[i]);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
}

void EO_Editor::SetSelectedGameObject(GameObject* go)
{
	selected_go = go;

	// outline
	for (auto& game_object : app->engine_order->game_objects)
	{
		C_MeshRenderer* mr = dynamic_cast<C_MeshRenderer*>(game_object.second->GetComponent(Component::TYPE::MESH_RENDERER));
		if (mr != NULL)
		{
			mr->GetMesh().is_outlined = false;
			mr->GetMesh().is_outlined_child = false;
		}
	}
	if (selected_go) SetOutline(go, go);
}

bool EO_Editor::SetMenuBar()
{
	bool exit = false;

	// Top Bar
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("New Scene"))
		{

		}
		if (ImGui::MenuItem("Load Scene"))
		{

		}
		if (ImGui::MenuItem("Save Scene"))
		{
			Serialization::YK_SaveScene();
		}
		if (ImGui::MenuItem("Save Scene As"))
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
		ImGui::Checkbox("Wireframe", &app->renderer3D->wireframe);

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
	if (ImGui::BeginMenu("Windows"))
	{
		for (int i = 0; i < (uint)EW_TYPE::NUM_EW_TYPE; i++)
		{
			if (i != (uint)EW_TYPE::ABOUT) // about goes in help
			{
				ImGui::Checkbox(editor_windows[i]->window_name.c_str(), &editor_windows[i]->enabled);
			}
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Settings"))
	{
		ImGui::Checkbox("Vsync", &app->renderer3D->vsync);
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help"))
	{
		ImGui::Checkbox(editor_windows[(uint)EW_TYPE::ABOUT]->window_name.c_str(), &editor_windows[(uint)EW_TYPE::ABOUT]->enabled);
		ImGui::EndMenu();
	}
	ImGui::SameLine(ImGui::GetWindowWidth() - 80);
	if (ImGui::MenuItem(ICON_FA_WINDOW_MINIMIZE))
	{
		SDL_MinimizeWindow(app->window->window);
	}
	if (SDL_GetWindowFlags(app->window->window) & SDL_WINDOW_FULLSCREEN_DESKTOP)
	{
		if (ImGui::MenuItem(ICON_FA_WINDOW_RESTORE))
		{
			SDL_SetWindowFullscreen(app->window->window, 0);
		}
	}
	else
	{
		if (ImGui::MenuItem(ICON_FA_WINDOW_MAXIMIZE))
		{
			SDL_SetWindowFullscreen(app->window->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
	}
	if (ImGui::MenuItem(ICON_FA_XMARK))
	{
		exit = true; // close window
	}
	ImGui::EndMainMenuBar();

	// update enabled windows
	for (int i = 0; i < (uint)EW_TYPE::NUM_EW_TYPE; i++)
	{
		if (editor_windows[i]->enabled)
		{
			editor_windows[i]->Update();
		}
		else if (i == (uint)EW_TYPE::SCENE)
		{
			dynamic_cast<EW_Scene*>(editor_windows[(uint)EW_TYPE::SCENE])->SetSceneCamera(false);
		}
		else if (i == (uint)EW_TYPE::GAME)
		{
			dynamic_cast<EW_Game*>(editor_windows[(uint)EW_TYPE::GAME])->SetCurrentGameCamera(false);
		}
	}

	return !exit;
}

void EO_Editor::SetOutline(GameObject* selected_game_object, GameObject* game_object)
{
	C_MeshRenderer* mr = dynamic_cast<C_MeshRenderer*>(game_object->GetComponent(Component::TYPE::MESH_RENDERER));
	if (mr != NULL)
	{
		mr->GetMesh().is_outlined = true;
		if (selected_game_object->id != game_object->id)
		{
			mr->GetMesh().is_outlined_child = true;
		}
	}
	for (auto& childs : game_object->GetChilds())
	{
		SetOutline(selected_game_object, childs);
	}
}