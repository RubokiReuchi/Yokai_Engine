#include "C_MeshRenderer.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "M_ModelRender.h"
#include "Re_Mesh.h"
#include "TextureImporter.h"

C_MeshRenderer::C_MeshRenderer(GameObject* gameObject) : Component(gameObject, TYPE::MESH_RENDERER)
{
	mesh_id = 0;
	instance_id = 0;
}

C_MeshRenderer::~C_MeshRenderer()
{
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id);
	manager->GetMap()[instance_id].CleanUp();
	manager->GetMap().erase(instance_id);
	if (manager->GetMap().size() == 0)
	{
		app->renderer3D->model_render.RemoveRender(mesh_id);
	}
}

void C_MeshRenderer::Update()
{
	if (!GetGameObject()->enabled)
	{
		GetMesh().visible = false;
	}
	else if (GetGameObject()->visible)
	{
		GetMesh().visible = true;
	}

	float3 pos, rot, scl;
	Quat rot_q;
	GetGameObject()->transform->GetGlobalMatrix().Decompose(pos, rot_q, scl);
	rot = rot_q.ToEulerXYZ();

	GetMesh().SetTransform(pos, scl, float3(rot.x * RADTODEG, rot.y * RADTODEG, rot.z * RADTODEG));

	if (show_aabb)
	{
		float3 points[8];
		GetGameObject()->global_aabb.GetCornerPoints(points);

		std::vector<float3> lines = PointsToLines_AABB(points);
		
		app->renderer3D->AddLines(lines, float4(0.235f, 0.529f, 0.0f, 1.0f));
	}
	if (show_obb)
	{
		float3 points[8];
		GetGameObject()->global_obb.GetCornerPoints(points);

		std::vector<float3> lines = PointsToLines_AABB(points);

		app->renderer3D->AddLines(lines, float4(0.69f, 0.203f, 0.321f, 1.0f));
	}
}

void C_MeshRenderer::OnEditor()
{
	ImGui::AlignTextToFramePadding();
	bool no_collapsed = ImGui::CollapsingHeader("Mesh Render", ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap); ImGui::SameLine(ImGui::GetWindowWidth() - 22);
	if (ImGui::Button(ICON_FA_ELLIPSIS_VERTICAL "##MeshRender", ImVec2(20.0f, 0)))
	{
		popUpOpen = true;
		ImGui::OpenPopup("Component Options");
		ori = ImGui::GetMousePosOnOpeningCurrentPopup();
	}
	else
	{
		ImGui::CloseCurrentPopup();
	}
	if (no_collapsed)
	{
		ImGui::Text("Visible: "), ImGui::SameLine();
		if (GetMesh().visible)
		{
			ImGui::TextColored(ImVec4(0.2f, 1.0f, 0.0f, 1.0f), "True");
		}
		else
		{
			ImGui::TextColored(ImVec4(0.9f, 0.0f, 0.0f, 1.0f), "False");
		}

		ImGui::Checkbox("Show AABB", &show_aabb);
		ImGui::Checkbox("Show OBB", &show_obb);
	}
	if (popUpOpen)
	{
		ComponentOptions(true);
	}
}

void C_MeshRenderer::InitAsInstanciedMesh(uint mesh_id)
{
	this->mesh_id = mesh_id + 2;

	M_Render* manager = app->renderer3D->model_render.GetRenderManager(this->mesh_id);

	Re_Mesh instanceMesh;
	instanceMesh.InitMeshInfo(manager->mesh_vertices, manager->mesh_indices, float3(0, 0, 0), float3(1, 1, 1));

	instance_id = manager->AddMesh(instanceMesh);
}

void C_MeshRenderer::InitAsNewMesh(std::vector<VertexInfo>& vertices, std::vector<uint>& indices, std::string mesh_path)
{
	Re_Mesh newMesh;
	newMesh.InitMeshInfo(vertices, indices, float3(0, 0, 0), float3(1, 1, 1));

	mesh_id = app->renderer3D->model_render.GetMapSize() + 2;
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id); // Create a M_Render
	manager->mesh_path = mesh_path;
	manager->mesh_vertices = vertices;
	manager->mesh_indices = indices;

	instance_id = manager->InitManageRender(newMesh);
}

Re_Mesh& C_MeshRenderer::GetMesh()
{
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id);

	Re_Mesh& meshReference = manager->GetMap()[instance_id];

	return meshReference;
}

std::string C_MeshRenderer::GetMeshPath()
{
	M_Render* manager = app->renderer3D->model_render.GetRenderManager(mesh_id);
	return manager->GetMeshPath();
}

std::string C_MeshRenderer::GetTexture(float texture_id)
{
	std::string texture_path;

	for (auto& loaded_tex : M_Texture::loaded_textures)
	{
		if (texture_id > 0 && texture_id == loaded_tex.first)
		{
			if (loaded_tex.second.name != "Checkers")
			{
				texture_path = loaded_tex.second.name;
			}
			else
			{
				texture_path = "Checkers";
			}
		}
	}
	if (GetMesh().texture_id == -1)
	{
		texture_path = "Default";
	}

	return texture_path;
}

std::vector<float3> C_MeshRenderer::PointsToLines_AABB(float3 points[8])
{
	std::vector<float3> lines;

	// face 1
	lines.push_back(points[0]); // line 1
	lines.push_back(points[1]);
	lines.push_back(points[0]); // line 2
	lines.push_back(points[4]);
	lines.push_back(points[1]); // line 3
	lines.push_back(points[5]);
	lines.push_back(points[4]); // line 4
	lines.push_back(points[5]);

	// face 2
	lines.push_back(points[2]); // line 5
	lines.push_back(points[3]);
	lines.push_back(points[2]); // line 6
	lines.push_back(points[6]);
	lines.push_back(points[6]); // line 7
	lines.push_back(points[7]);
	lines.push_back(points[3]); // line 8
	lines.push_back(points[7]);

	// joints
	lines.push_back(points[0]); // line 9
	lines.push_back(points[2]);
	lines.push_back(points[1]); // line 10
	lines.push_back(points[3]);
	lines.push_back(points[4]); // line 11
	lines.push_back(points[6]);
	lines.push_back(points[5]); // line 12
	lines.push_back(points[7]);

	return lines;
}