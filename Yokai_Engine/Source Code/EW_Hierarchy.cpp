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
    // Hierarchy
    ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    DrawGameObjectChildren(gameObjectsReference->at(1), 1);
    ImGui::End();
}

void EW_Hierarchy::DrawGameObjectChildren(GameObject* gameObject, bool onlyChildren)
{
    if (!onlyChildren) ProcessGameObject(gameObject, 0);
    else
    {
        for (size_t i = 0; i < gameObject->children.size(); i++)
        {
            ProcessGameObject(gameObject->children[i], i);
        }
    }

}

void EW_Hierarchy::ProcessGameObject(GameObject* gameObject, int iteration)
{
    ImGuiTreeNodeFlags node_flags = base_flags;

    if (gameObject == editor->GetSelectedGameObject()) node_flags |= ImGuiTreeNodeFlags_Selected;

    bool node_open;

    if (gameObject->children.empty())
    {
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration);
        node_open = false;
    }
    else
    {
        node_open = ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration);
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("GameObject", gameObject, sizeof(GameObject*));

        draggingGameObject = gameObject;

        ImGui::Text("Change game object parent");
        ImGui::EndDragDropSource();
    }

    if (ImGui::IsItemHovered() && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left))
    {
        editor->SetSelectedGameObject(gameObject);
    }

    if (ImGui::BeginDragDropTarget())
    {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("GameObject"))
        {
            draggingGameObject->SetParent(gameObject);
            draggingGameObject = nullptr;
        }
        ImGui::EndDragDropTarget();
    }

    if (node_open)
    {
        if (!gameObject->children.empty()) DrawGameObjectChildren(gameObject, true);
        ImGui::TreePop();
    }
}