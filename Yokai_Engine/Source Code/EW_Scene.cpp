#include "EW_Scene.h"
#include "Application.h"
#include "EO_Editor.h"
#include "ModuleInput.h"
#include "ModuleFile.h"

#include <math.h>

EW_Scene::EW_Scene()
{
	window_name = "Scene";
	enabled = true;

	scene_camera = &app->camera->sceneCamera;

	guizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
	guizmo_mode = ImGuizmo::MODE::WORLD;
	guizmo_hide = true;
	prev_transform.SetIdentity();
	prev_go = NULL;
}

EW_Scene::~EW_Scene()
{
}

void EW_Scene::Update()
{
	// Screen
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoCollapse);
	
	scene_camera->active = true;
	app->camera->updateSceneCamera = (bool)ImGui::IsWindowHovered();
	if (app->camera->updateSceneCamera && !ImGuizmo::IsUsing() && !ImGuizmo::IsOver())
	{
		if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
		{
			app->camera->click_state = 2;
		}
		else if(ImGui::IsMouseClicked(ImGuiMouseButton_Left))
		{
			app->camera->click_state = 1;
		}

		app->engine_order->scene_pos.x = ImGui::GetWindowPos().x;
		app->engine_order->scene_pos.y = ImGui::GetWindowPos().y + ImGui::GetFrameHeight();
		app->engine_order->scene_size.x = ImGui::GetWindowSize().x;
		app->engine_order->scene_size.y = ImGui::GetWindowSize().y - ImGui::GetFrameHeight();
	}

	if (app->camera->updateSceneCamera && prev_go != NULL)
	{
		if (app->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
		{
			prev_go->transform->SetTransform(prev_transform);
			prev_transform.SetIdentity();
			prev_go = NULL;
		}
	}

	ImVec2 gameDimensions = ImGui::GetContentRegionAvail();

	if (gameDimensions.x != scene_width || gameDimensions.y != scene_height)
	{
		// If the size of this imgui window is different from the one stored.
		scene_width = (int)gameDimensions.x;
		scene_height = (int)gameDimensions.y;

		app->camera->sceneCamera.ChangeAspectRatio((float)scene_width / (float)scene_height);
	}

	ImGui::Image((ImTextureID)scene_camera->frameBuffer.GetTexture(), ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

	if (ImGui::IsWindowHovered() && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN) scene_camera->confine_move = true;
	else if (ImGui::IsWindowHovered() && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_UP) scene_camera->confine_move = false;
	if (ImGui::IsWindowHovered() && app->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_DOWN) scene_camera->confine_pan = true;
	else if (ImGui::IsWindowHovered() && app->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_UP) scene_camera->confine_pan = false;

	if ((scene_camera->confine_move && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		|| scene_camera->confine_pan && app->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		uint dx = math::Abs(app->input->GetMouseXMotion());
		uint dy = math::Abs(app->input->GetMouseYMotion());
		if (ImGui::GetMousePos().x - 2 - dx < ImGui::GetWindowPos().x)
		{
			app->input->SetMousePos(ImGui::GetWindowPos().x + ImGui::GetWindowWidth() - 5, ImGui::GetMousePos().y);
		}
		else if (ImGui::GetMousePos().x + 4 + dx > ImGui::GetWindowPos().x + ImGui::GetWindowWidth())
		{
			app->input->SetMousePos(ImGui::GetWindowPos().x + 3, ImGui::GetMousePos().y);
		}
		else if (ImGui::GetMousePos().y - 22 - dy < ImGui::GetWindowPos().y)
		{
			app->input->SetMousePos(ImGui::GetMousePos().x, ImGui::GetWindowPos().y + ImGui::GetWindowHeight() - 12);
		}
		else if (ImGui::GetMousePos().y + 11 + dy > ImGui::GetWindowPos().y + ImGui::GetWindowHeight())
		{
			app->input->SetMousePos(ImGui::GetMousePos().x, ImGui::GetWindowPos().y + 23);
		}
	}

	// ImGuizmo
	GameObject* go = app->engine_order->editor->GetSelectedGameObject();
	if (go != NULL)
	{
		if (!go->is_camera)
		{
			if (guizmo_operation == ImGuizmo::OPERATION::ROTATE_Y)
			{
				guizmo_operation = ImGuizmo::OPERATION::ROTATE;
			}
			else if (guizmo_operation == ImGuizmo::OPERATION::ROTATE_Z)
			{
				guizmo_operation = ImGuizmo::OPERATION::SCALE;
			}
		}
		else if (go->is_camera)
		{
			if (guizmo_operation == ImGuizmo::OPERATION::ROTATE)
			{
				guizmo_operation = ImGuizmo::OPERATION::ROTATE_Y;
			}
			else if (guizmo_operation == ImGuizmo::OPERATION::SCALE)
			{
				guizmo_operation = ImGuizmo::OPERATION::ROTATE_Z;
			}
		}
		if (!ImGuizmo::IsUsing() && ImGui::IsWindowHovered() && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_IDLE)
		{
			if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
			{
				guizmo_hide = true;
			}
			else if(app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			{
				guizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
				guizmo_hide = false;
			}
			else if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			{
				guizmo_operation = ImGuizmo::OPERATION::ROTATE;
				guizmo_hide = false;
				if (go->is_camera) guizmo_operation = ImGuizmo::OPERATION::ROTATE_Y;
			}
			else if (app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
			{
				guizmo_operation = ImGuizmo::OPERATION::SCALE;
				guizmo_hide = false;
				if (go->is_camera) guizmo_operation = ImGuizmo::OPERATION::ROTATE_Z;
			}
			else if (app->input->GetKey(SDL_SCANCODE_T) == KEY_DOWN)
			{
				if (guizmo_mode == ImGuizmo::MODE::WORLD) guizmo_mode = ImGuizmo::MODE::LOCAL;
				else guizmo_mode = ImGuizmo::MODE::WORLD;
			}
		}

		if (!guizmo_hide)
		{
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

			ImGuizmo::SetDrawlist();
			float4x4 matrix = go->transform->GetGlobalMatrix().Transposed();

			// force scale mode to local transform
			ImGuizmo::MODE aux_mode = ImGuizmo::MODE::WORLD;
			if (guizmo_mode == ImGuizmo::MODE::WORLD && guizmo_operation == ImGuizmo::OPERATION::SCALE)
			{
				aux_mode = ImGuizmo::MODE::LOCAL;
			}
			else if (guizmo_mode == ImGuizmo::MODE::LOCAL && go->is_camera && (guizmo_operation == ImGuizmo::OPERATION::ROTATE_Y || guizmo_operation == ImGuizmo::OPERATION::ROTATE_Z))
			{
				aux_mode = ImGuizmo::MODE::WORLD;
			}
			else
			{
				aux_mode = guizmo_mode;
			}

			if (ImGuizmo::Manipulate(app->camera->sceneCamera.GetViewMatrix(), app->camera->sceneCamera.GetProjectionMatrix(), guizmo_operation, aux_mode, matrix.ptr()) && ImGui::IsWindowHovered())
			{
				if (!once)
				{
					once = true;
					prev_transform = go->transform->GetGlobalMatrix();
					prev_go = go;
				}
				matrix.Transpose();
				go->transform->SetTransform(matrix);
			}
			else
			{
				once = false;
			}
		}
	}

	if (ImGui::BeginDragDropTarget() && app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
	{
		std::string dd_file = app->engine_order->editor->dd_file;
		app->engine_order->editor->dd_file = "";
		ImGuiDragDropFlags target_flags = 0;
		target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;
		target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect;
		if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(dd_file.c_str(), target_flags))
		{
			app->engine_order->editor->message = ResourceManager::LoadResource(dd_file);
			popUp = true;
			popUp_cd = 0;
			ImGui::OpenPopup("Message");
		}
		ImGui::EndDragDropTarget();
	}

	if (popUp)
	{
		popUp_cd++;
		if (popUp_cd > 23)
		{
			float new_alpha = (255 - ((popUp_cd - 23) * 2)) / 255.0f;
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, new_alpha);
		}
		else
		{
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
		}
		ImGui::SetNextWindowPos(ImVec2(ImGui::GetWindowPos().x + (ImGui::GetWindowSize().x / 2) - 150, ImGui::GetWindowPos().y + (ImGui::GetWindowSize().y / 2) - 75));
		ImGui::SetNextWindowSize(ImVec2(300, 150));
		if (ImGui::BeginPopup("Message"))
		{
			ImGui::PushFont(app->engine_order->editor->arial_font_30);
			ImVec2 text_size = ImVec2(ImGui::CalcTextSize(app->engine_order->editor->message.c_str()).x, ImGui::CalcTextSize(app->engine_order->editor->message.c_str()).y);
			ImGui::SetCursorPos(ImVec2((ImGui::GetWindowSize().x - text_size.x) * 0.5f, (ImGui::GetWindowSize().y - text_size.y) * 0.5f));
			ImGui::Text(app->engine_order->editor->message.c_str());
			ImGui::PopFont();
			ImGui::EndPopup();
		}
		ImGui::PopStyleVar();
		if (popUp_cd >= 150)
		{
			popUp = false;
			popUp_cd = 0;
			app->engine_order->editor->message = "";
		}
	}
	ImGui::End();
}