#include "GameObject.h"
#include "Application.h"
#include "EO_Editor.h"

GameObject::GameObject(GameObject* parent, std::string name, std::string tag, bool is_camera) : name(name), tag(tag), is_camera(is_camera)
{
	id = app->engine_order->AddGameObject(this);
	transform = dynamic_cast<C_Transform*>(AddComponent(Component::TYPE::TRANSFORM));
	if (parent != NULL) parent->AddChild(this);
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
	app->engine_order->editor->SetSelectedGameObject(NULL);
	parent->RemoveChild(this);
	app->engine_order->game_objects[id] = NULL;
	app->engine_order->delete_qeue.push_back(this);
	for (size_t i = 0; i < children.size(); i++)
	{
		children[i]->DeleteGameObject();
	}
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