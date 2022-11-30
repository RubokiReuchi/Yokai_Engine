#include "GameObject.h"
#include "Application.h"
#include "EO_Editor.h"

GameObject::GameObject(GameObject* parent, std::string name, std::string tag, bool is_camera) : name(name), tag(tag), is_camera(is_camera)
{
	id = app->engine_order->AddGameObject(this);
	transform = dynamic_cast<C_Transform*>(AddComponent(Component::TYPE::TRANSFORM));
	if (parent != NULL) parent->AddChild(this);

	if (UUID == "-1")
	{
		UUID = app->engine_order->GenerateUUID();
	}
}

GameObject::GameObject(SerializedGO go)
{
	name = go.name;
	tag = go.tag;
	is_camera = go.is_camera;

	id = app->engine_order->AddGameObject(this);
	transform = dynamic_cast<C_Transform*>(AddComponent(Component::TYPE::TRANSFORM));
	UUID = go.UUID;
	if (go.parentUUID != "-1") app->engine_order->GetGameObjectByUUID(go.parentUUID)->AddChild(this);

	enabled = go.enabled;
	visible = go.visible;
	visible_on_editor = go.visible_on_editor;

	// components
	SaveMesh aux_mesh;

	for (size_t i = 0; i < go.components_type.size(); i++)
	{
		switch (go.components_type[i])
		{
		case 2: // Mesh Renderer
			if (app->engine_order->loadedSerializedMeshes.find(go.mesh_path) != app->engine_order->loadedSerializedMeshes.end()) // check if path is loaded
			{
				dynamic_cast<C_MeshRenderer*>(AddComponent(Component::TYPE::MESH_RENDERER))->InitAsInstanciedMesh(app->engine_order->loadedSerializedMeshes[go.mesh_path]);
			}
			else
			{
				app->engine_order->loadedSerializedMeshes[go.mesh_path] = app->renderer3D->model_render.GetMapSize();

				SaveMesh aux_mesh;
				aux_mesh.YK_LoadMesh(go.mesh_path.c_str());

				dynamic_cast<C_MeshRenderer*>(AddComponent(Component::TYPE::MESH_RENDERER))->InitAsNewMesh(aux_mesh.vertices, aux_mesh.indices, go.mesh_path);
			}
			break;
		case 3: // Material
			dynamic_cast<C_Material*>(AddComponent(Component::TYPE::MATERIAL))->SetTexture(go.selected_texture);
			break;
		case 4: // Camera
			app->camera->InitNewGameCamera(this);
			C_Camera* cam = dynamic_cast<C_Camera*>(GetComponent(Component::TYPE::CAMERA));
			cam->GetCamera()->cameraFrustum.SetWorldMatrix(go.camera_matrix);
			cam->GetCamera()->SetFOV(math::RadToDeg(go.fov));
			cam->GetCamera()->SetRange(go.camera_range);
			break;
		}
	}

	if (!is_camera) GenerateAABB();
	dynamic_cast<C_Transform*>(GetComponent(Component::TYPE::TRANSFORM))->SetTransform(go.position, go.scale, go.rotation);
}

GameObject::~GameObject()
{
	for (size_t i = 0; i < components.size(); i++)
	{
		RELEASE(components[i]);
	}
	components.clear();

	for (size_t i = 0; i < children.size(); i++)
	{
		RELEASE(children[i]);
	}
	children.clear();
}

void GameObject::Update()
{
	for (auto& component : components)
	{
		if (component->IsEnabled()) component->Update();
	}
}

void GameObject::DeleteGameObject()
{
	if (app->engine_order->editor->GetSelectedGameObject() == this) app->engine_order->editor->SetSelectedGameObject(NULL);
	if (parent) parent->RemoveChild(this);
	for (size_t i = 1; i < app->engine_order->game_objects.size() + 1; i++)
	{
		if (app->engine_order->game_objects[i]->id > this->id)
		{
			app->engine_order->game_objects[i - 1] = app->engine_order->game_objects[i];
			app->engine_order->game_objects[i]->id--;
		}
	}
	app->engine_order->game_objects.erase(app->engine_order->game_objects.size());
	app->engine_order->delete_queu.push_back(this);
	while (children.size() > 0)
	{
		children[0]->DeleteGameObject();
	}
	app->engine_order->id_counter--;
}

bool GameObject::AddChild(GameObject* child)
{
	if (child->parent == this)
	{
		return false;
	}

	GameObject* aux_parent = parent;
	while (aux_parent)
	{
		if (aux_parent == child)
		{
			return false;
		}

		aux_parent = aux_parent->parent;
	}

	children.push_back(child);

	if (child->parent)
	{
		child->parent->RemoveChild(child);
		child->parent->GenerateAABB(); // recalculate aabb of old parent
	}

	child->parent = this;
	GenerateAABB(); // recalculate aabb of new parent
	child->transform->parentGlobalTransform = transform->GetGlobalTransform();
	child->transform->UpdateTransform();

	return true;
}

bool GameObject::SetParent(GameObject* parent)
{
	return parent->AddChild(this);
}

void GameObject::RemoveChild(GameObject* child)
{
	for (size_t i = 0; i < children.size(); ++i)
	{
		if (children[i] == child)
		{
			children.erase(children.begin() + i);
		}
	}
	child->parent = NULL;
}

void GameObject::GenerateAABB()
{
	if (!aabb_init) aabb_init = true;

	std::vector<float3> vertices_pos = GetAllVerticesPositions(this);

	float* v_pos = new float[vertices_pos.size() * 3];
	for (size_t i = 0; i < vertices_pos.size(); i++)
	{
		v_pos[i * 3] = vertices_pos[i].x;
		v_pos[i * 3 + 1] = vertices_pos[i].y;
		v_pos[i * 3 + 2] = vertices_pos[i].z;
	}

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)v_pos, vertices_pos.size());

	delete[] v_pos;
}

std::vector<float3> GameObject::GetAllVerticesPositions(GameObject* go)
{
	std::vector<float3> vertices_pos;

	for (size_t i = 0; i < go->children.size(); i++)
	{
		std::vector<float3> child_vertices_pos = GetAllVerticesPositions(go->children[i]);
		vertices_pos.insert(vertices_pos.end(), child_vertices_pos.begin(), child_vertices_pos.end());
	}

	if (go->GetComponent(Component::TYPE::MESH_RENDERER))
	{
		uint mesh_id = dynamic_cast<C_MeshRenderer*>(go->GetComponent(Component::TYPE::MESH_RENDERER))->GetMeshID();
		M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id);
		std::vector<float3> mesh_vertices_pos;
		for (auto& vertex : manager->GetVertices())
		{
			mesh_vertices_pos.push_back(vertex.position);
		}
		vertices_pos.insert(vertices_pos.end(), mesh_vertices_pos.begin(), mesh_vertices_pos.end());
	}

	return vertices_pos;
}

void GameObject::GenerateFixedAABB()
{
	if (!aabb_init) aabb_init = true;

	std::vector<float3> vertices_pos;
	vertices_pos.push_back(float3(-0.25f, -0.2f, -0.25f));
	vertices_pos.push_back(float3(0.25f, -0.2f, -0.25f));
	vertices_pos.push_back(float3(-0.25f, -0.2f, 0.25f));
	vertices_pos.push_back(float3(0.25f, -0.2f, 0.25f));
	vertices_pos.push_back(float3(-0.25f, 0.3f, -0.25f));
	vertices_pos.push_back(float3(0.25f, 0.3f, -0.25f));
	vertices_pos.push_back(float3(-0.25f, 0.3f, 0.25f));
	vertices_pos.push_back(float3(0.25f, 0.3f, 0.25f));

	float* v_pos = new float[vertices_pos.size() * 3];
	for (size_t i = 0; i < vertices_pos.size(); i++)
	{
		v_pos[i * 3] = vertices_pos[i].x;
		v_pos[i * 3 + 1] = vertices_pos[i].y;
		v_pos[i * 3 + 2] = vertices_pos[i].z;
	}

	aabb.SetNegativeInfinity();
	aabb.Enclose((float3*)v_pos, vertices_pos.size());

	delete[] v_pos;
}