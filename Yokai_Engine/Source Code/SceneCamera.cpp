#include "SceneCamera.h"
#include "ModuleInput.h"
#include "GameObject.h"
#include "ModuleEngineOrder.h"
#include "EO_Editor.h"
#include "ModuleWindow.h"

SceneCamera::SceneCamera()
{
}

SceneCamera::~SceneCamera()
{
}

void SceneCamera::UpdateCameraInput(float dt)
{
	if (editor == nullptr) editor = app->engine_order->editor;
	GameObject* selectedGO = editor->GetSelectedGameObject();

	float dx = -(float)app->input->GetMouseXMotion();
	float dy = -(float)app->input->GetMouseYMotion();
	float Sensitivity = 0.25f;

	float3 empty = { 0,0,0 };
	Quat lookingDir = Quat::identity;
	cameraFrustum.WorldMatrix().Decompose(empty, lookingDir, empty);

	float3 newPos(0, 0, 0);
	float speed;
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN) fast_move = !fast_move;
	if (!fast_move)	speed = 1.0f * dt;
	else speed = 5.0f * dt;

	// Move
	if (confine_move && app->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		if (app->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += cameraFrustum.up * speed;
		if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos -= cameraFrustum.up * speed;
		if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += cameraFrustum.front * speed;
		if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= cameraFrustum.front * speed;
		if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= cameraFrustum.WorldRight() * speed;
		if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += cameraFrustum.WorldRight() * speed;
		
		Position += newPos;
		Reference += newPos;

		cameraFrustum.pos += newPos;

		if (dy != 0)
		{
			float3 viewDir = { cameraFrustum.ViewMatrix().v[2][0],  cameraFrustum.ViewMatrix().v[2][1], cameraFrustum.ViewMatrix().v[2][2] };
			float cosAngle = math::Dot(float3(0, 1, 0), cameraFrustum.front);

			float DeltaY = math::DegToRad((float)dy * Sensitivity * 0.75f);

			if (math::Abs(cosAngle) > 0.99f)
			{
				if (cosAngle > 0 && DeltaY > 0) DeltaY = 0;
				if (cosAngle < 0 && DeltaY < 0) DeltaY = 0;
			}

			math::Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle({ 1.0f, 0.0f, 0.0f }, DeltaY);

			lookingDir = lookingDir * rotation;
		}

		if (dx != 0)
		{
			float DeltaX = math::DegToRad((float)dx * Sensitivity * 0.75f);

			math::Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle({ 0.0f, 1.0f, 0.0f }, DeltaX);

			lookingDir = rotation * lookingDir;
		}

		float4x4 newWorldMatrix = cameraFrustum.WorldMatrix();
		newWorldMatrix.SetRotatePart(lookingDir.Normalized());
		cameraFrustum.SetWorldMatrix(newWorldMatrix.Float3x4Part());
	}

	// pan
	if (confine_pan && app->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
	{
		if (dx != 0 || dy != 0)
		{
			float3 movVector((cameraFrustum.WorldRight() * dx) + (-cameraFrustum.up * dy));
			cameraFrustum.pos += movVector * dt;
		}
	}

	// rotate arround the selected game object
	if (confine_move && app->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		float3 rotationCenter = { 0.0f, 0.0f, 0.0f };

		if (selectedGO != nullptr) rotationCenter = selectedGO->transform->GetGlobalTransform().position;

		float distFromCenter = cameraFrustum.pos.Distance(rotationCenter);

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle(float3(1, 0, 0), DeltaY * DEGTORAD);

			lookingDir = lookingDir * rotation;
		}

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			Quat rotation = Quat::identity;
			rotation.SetFromAxisAngle(float3(0, 1, 0), DeltaX * DEGTORAD);

			lookingDir = rotation * lookingDir;
		}

		float4x4 newWorldMatrix = cameraFrustum.WorldMatrix();
		newWorldMatrix.SetRotatePart(lookingDir.Normalized());
		cameraFrustum.SetWorldMatrix(newWorldMatrix.Float3x4Part());

		cameraFrustum.pos = rotationCenter + (cameraFrustum.front * -distFromCenter);
		LookAt(rotationCenter);
	}

	// zoom in and zoom out
	if (app->input->GetMouseZ() != 0)
	{
		float newFOV = math::RadToDeg(GetFOV()) + (1.5f * -app->input->GetMouseZ());

		if (newFOV > 20.0f && newFOV < 160.0f) SetFOV(newFOV);
	}

	// go to selected game object
	if (app->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	{
		if (selectedGO != nullptr)
		{
			Focus(selectedGO->transform->GetGlobalTransform().position);
		}
	}
}
void SceneCamera::CalculateMousePicking()
{
	float mouse_x = (((float)app->input->GetMouseX() - app->engine_order->scene_pos.x) / app->engine_order->scene_size.x) - 0.5f;
	float mouse_y = (((float)app->input->GetMouseY() - app->engine_order->scene_pos.y) / app->engine_order->scene_size.y) - 0.5f;
	Confine(mouse_x, -0.5f, 0.5f);
	Confine(mouse_y, -0.5f, 0.5f);
	LineSegment picking_ray = cameraFrustum.UnProjectLineSegment(mouse_x * 2, mouse_y * 2);

	std::vector<GameObject*> go_hitted;
	for (auto& go : app->engine_order->game_objects)
	{
		if (go.second->GetComponent(Component::TYPE::MESH_RENDERER))
		{
			RayCastHit hit;
			hit.hit = picking_ray.Intersects(go.second->global_aabb);
			if (hit.hit)
			{
				go_hitted.push_back(go.second);
			}
		}
	}

	if (!go_hitted.empty())
	{
		GameObject* hitted_go = NULL;
		float min_distance = 9999;
		for (auto& go : go_hitted)
		{
			Re_Mesh aux_mesh = dynamic_cast<C_MeshRenderer*>(go->GetComponent(Component::TYPE::MESH_RENDERER))->GetMesh();
			LineSegment ray = picking_ray;
			ray.Transform(go->transform->GetGlobalMatrix().Inverted());

			for (size_t i = 0; i < aux_mesh.indices->size(); i += 3)
			{
				float3 vertex1_pos(aux_mesh.vertices->at(aux_mesh.indices->at(i)).position);
				float3 vertex2_pos(aux_mesh.vertices->at(aux_mesh.indices->at(i + 1)).position);
				float3 vertex3_pos(aux_mesh.vertices->at(aux_mesh.indices->at(i + 2)).position);

				Triangle face(vertex1_pos, vertex2_pos, vertex3_pos);

				RayCastHit hit;
				hit.hit = picking_ray.Intersects(face, &hit.distance, nullptr);
				if (hit.hit && hit.distance < min_distance)
				{
					min_distance = hit.distance;
					hitted_go = go;
				}
			}
		}

		go_hitted.clear();

		app->engine_order->editor->SetSelectedGameObject(hitted_go);
	}
	else
	{
		app->engine_order->editor->SetSelectedGameObject(NULL);
	}
	
}

void SceneCamera::Focus(const float3& focusPoint)
{
	float3 newPos = focusPoint;
	newPos += float3(4.0f, 0.0f, 2.0f);
	cameraFrustum.pos = newPos;
	LookAt(focusPoint);
}

void SceneCamera::Confine(float& value, float min_value, float max_value)
{
	if (value > max_value)
	{
		value = max_value;
	}
	else if (value < min_value)
	{
		value = min_value;
	}
}