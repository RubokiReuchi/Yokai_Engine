#include "EW_Inspector.h"
#include "Application.h"
#include "EO_Editor.h"
#include "C_Camera.h"
#include "C_Transform.h"
#include "C_MeshRenderer.h"
#include "ImGuiHelpers.h"

EW_Inspector::EW_Inspector()
{
	window_name = "Inspector";
	enabled = true;

	editor = dynamic_cast<EO_Editor*>(app->engine_order->engine_order[(uint)EO_NUM::EDITOR]);
}

EW_Inspector::~EW_Inspector()
{
}

void EW_Inspector::Update()
{
	selectGameobject = editor->GetSelectedGameObject();
	// Inspector
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
	ImGui::Checkbox("##Enable", &selectGameobject->enabled); ImGui::SameLine();
	ImGuiH::InputText("##Name", &selectGameobject->name);

	if (editor->GetSelectedGameObject() != NULL)
	{
		if (editor->GetSelectedGameObject()->GetComponent(Component::TYPE::CAMERA) != NULL)
		{
			dynamic_cast<C_Camera*>(editor->GetSelectedGameObject()->GetComponent(Component::TYPE::CAMERA))->OnEditor();
		}
		if (editor->GetSelectedGameObject()->GetComponent(Component::TYPE::TRANSFORM) != NULL)
		{
			dynamic_cast<C_Transform*>(editor->GetSelectedGameObject()->GetComponent(Component::TYPE::TRANSFORM))->OnEditor();
		}
		if (editor->GetSelectedGameObject()->GetComponent(Component::TYPE::MESH_RENDERER) != NULL)
		{
			dynamic_cast<C_MeshRenderer*>(editor->GetSelectedGameObject()->GetComponent(Component::TYPE::MESH_RENDERER))->OnEditor();
		}
	}
	ImGui::End();
}