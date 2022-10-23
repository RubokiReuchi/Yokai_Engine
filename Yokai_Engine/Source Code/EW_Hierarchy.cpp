#include "EW_Hierarchy.h"
#include "ModuleEngineOrder.h"
#include "EO_Editor.h"
#include "GameObject.h"
#include "IconsFontAwesome.h"

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
    
    if (ImGui::Button(ICON_FA_FILE "  Create new Game Object", ImVec2(ImGui::GetWindowWidth(), 25)))
    {
        popUpOpen = !popUpOpen;
        if (popUpOpen)
        {
            ImGui::OpenPopup("New GameObject");
            ori = ImGui::GetMousePosOnOpeningCurrentPopup();
        }
        else
        {
            ImGui::CloseCurrentPopup();
        }
    }
    DrawGameObjectChildren(gameObjectsReference->at(1), true);

    if (popUpOpen)
    {
        int selectedSubmenu = 0;
        std::string submenuNames[numSubmenus] = { "Primitives", "Lights" };

        ImGui::SetNextWindowSize(ImVec2(200.0f, 250.0f));
        if (ImGui::BeginPopup("New GameObject"))
        {
            if (ImGui::CollapsingHeader("Primitives"))
            {
                int selectedShape = 0;
                std::string shapeNames[numPrimitives] = { "Plane", "Cube", "Sphere", "Cylinder", "Cone" };

                ImGui::Separator();
                for (int i = 0; i < numPrimitives; i++)
                {
                    if (ImGui::Selectable(shapeNames[i].c_str(), false, 0, ImVec2(200, 15)))
                    {
                        selectedShape = i;
                        app->renderer3D->model_render.CreatePrimitive(rightClickedGameObject, (PrimitiveType)i);
                        popUpOpen = false;
                    }
                }
            }
            if (ImGui::CollapsingHeader("Lights"))
            {
                int selectedShape = 0;
                std::string shapeNames[numPrimitives] = { "Plane", "Cube", "Sphere", "Cylinder", "Cone" };

                ImGui::Separator();
                for (int i = 0; i < numPrimitives; i++)
                {
                    if (ImGui::Selectable(shapeNames[i].c_str(), false, 0, ImVec2(200, 15)))
                    {
                        selectedShape = i;
                        //app->renderer3D->model_render.CreatePrimitive(rightClickedGameObject, (PrimitiveType)i);
                        popUpOpen = false;
                    }
                }
            }
            ImGui::EndPopup();
        }
    }
    CheckMouseInPopUp();

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
        ImGui::AlignTextToFramePadding();
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_AllowItemOverlap;
        ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration); /*ImGui::SameLine(ImGui::GetWindowWidth() - 28);
        if (gameObject->visible_on_editor)
        {
            if (ImGui::Button(ICON_FA_EYE))
            {
                UpdateVisibleOnEditor(gameObject, false);
            }
        }
        else
        {
            if (ImGui::Button(ICON_FA_EYE_SLASH))
            {
                UpdateVisibleOnEditor(gameObject, true);
            }
        }*/
        node_open = false;
    }
    else
    {
        ImGui::AlignTextToFramePadding();
        node_flags |= ImGuiTreeNodeFlags_AllowItemOverlap;
        node_open = ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration); /*ImGui::SameLine(ImGui::GetWindowWidth() - 28);
        if (gameObject->visible_on_editor)
        {
            if (ImGui::Button(ICON_FA_EYE))
            {
                UpdateVisibleOnEditor(gameObject, false);
            }
        }
        else
        {
            if (ImGui::Button(ICON_FA_EYE_SLASH))
            {
                UpdateVisibleOnEditor(gameObject, true);
            }
        }*/
    }

    if (ImGui::BeginDragDropSource())
    {
        ImGui::SetDragDropPayload("GameObject", gameObject, sizeof(GameObject*));

        draggingGameObject = gameObject;

        ImGui::Text("Change game object parent");
        ImGui::EndDragDropSource();
    }

    if (ImGui::IsItemHovered())
    {
        if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left)) editor->SetSelectedGameObject(gameObject);
        if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right)) rightClickedGameObject = gameObject;
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

void EW_Hierarchy::CheckMouseInPopUp()
{
    ImVec2 act = ImVec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
    
    if (act.x > ori.x + 200 || act.x < ori.x || act.y < ori.y || act.y > ori.y + 250)
    {
        popUpOpen = false;
        ImGui::CloseCurrentPopup();
    }
}

void EW_Hierarchy::UpdateVisibleOnEditor(GameObject* gameObject, bool visible_on_editor)
{
    gameObject->visible_on_editor = visible_on_editor;
    C_MeshRenderer* mr = dynamic_cast<C_MeshRenderer*>(gameObject->GetComponent(Component::TYPE::MESH_RENDERER));
    if (mr != NULL && gameObject->GetParent()->visible_on_editor)
    {
        mr->GetMesh().visible_on_editor = gameObject->visible_on_editor;
    }
    for (auto& childs : gameObject->GetChilds())
    {
        childs->visible_on_editor = visible_on_editor;
        C_MeshRenderer* c_mr = dynamic_cast<C_MeshRenderer*>(childs->GetComponent(Component::TYPE::MESH_RENDERER));
        if (c_mr != NULL && childs->visible_on_editor)
        {
            c_mr->GetMesh().visible_on_editor = gameObject->visible_on_editor;
        }
    }
}