#include "EW_Hierarchy.h"
#include "ModuleEngineOrder.h"
#include "EO_Editor.h"
#include "GameObject.h"

EW_Hierarchy::EW_Hierarchy()
{
	window_name = "Hierarchy";
	enabled = true;

    editor = dynamic_cast<EO_Editor*>(app->engine_order->engine_order[(uint)EO_NUM::EDITOR]);
    gameObjectsReference = &app->engine_order->game_objects;

    base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
}

EW_Hierarchy::~EW_Hierarchy()
{
}

void EW_Hierarchy::Update()
{
    ImGui::ShowDemoWindow();

	// Hierarchy
    ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    // example
    if (ImGui::TreeNode("Root"))
    {
        DrawGameObjectChildren(gameObjectsReference->at(1), 1);
        ImGui::TreePop();
    }
    ImGui::End();
}

void EW_Hierarchy::DrawGameObjectChildren(GameObject* gameObject, int layer)
{
    for (int i = 0; i < gameObject->GetChilds().size(); i++)
    {
        if (i == 0)  ImGui::SetNextItemOpen(true, ImGuiCond_Once);

        ImGuiTreeNodeFlags node_flags = base_flags;

        if (gameObject->GetChilds().at(i) == editor->GetSelectedGameObject()) node_flags |= ImGuiTreeNodeFlags_Selected;

        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)i, node_flags, gameObject->GetChilds().at(i)->name.c_str(), i);

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("GameObject", gameObject->GetChilds().at(i), sizeof(GameObject*));

            draggingGameObject = gameObject->GetChilds().at(i);

            ImGui::Text("Change game object parent");
            ImGui::EndDragDropSource();
        }

        if (ImGui::IsItemHovered() && ImGui::IsMouseReleased(ImGuiMouseButton_::ImGuiMouseButton_Left))
        {
            editor->SetSelectedGameObject(gameObject->GetChilds().at(i));
        }

        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
            {
                draggingGameObject->SetParent(gameObject->GetChilds().at(i));
                draggingGameObject = nullptr;
            }
            ImGui::EndDragDropTarget();
        }

        if (node_open)
        {
            DrawGameObjectChildren(gameObject->GetChilds().at(i), layer++);
            ImGui::TreePop();
        }
    }
}