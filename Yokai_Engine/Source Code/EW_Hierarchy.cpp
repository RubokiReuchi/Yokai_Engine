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

    base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
}

EW_Hierarchy::~EW_Hierarchy()
{
}

void EW_Hierarchy::Update()
{
    // Hierarchy
    ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoCollapse);
    
    if (ImGui::Button(ICON_FA_FOLDER_PLUS "  Create new Game Object", ImVec2(ImGui::GetWindowWidth(), 25)))
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
        ImGui::SetNextWindowSize(ImVec2(200.0f, 250.0f));
        if (ImGui::BeginPopup("New GameObject"))
        {
            if (ImGui::Selectable("Empty GameObject", false, 0, ImVec2(200, 15)))
            {
                GameObject* newGameObject = new GameObject(app->engine_order->rootGameObject, "Empty GameObject");
                popUpOpen = false;
            }
            ImGui::Separator();
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
                        app->renderer3D->model_render.CreatePrimitive(app->engine_order->rootGameObject, (PrimitiveType)i);
                        popUpOpen = false;
                    }
                }
            }
            if (ImGui::CollapsingHeader("Lights"))
            {
                
            }
            ImGui::EndPopup();
        }
        if (!ImGuiH::CheckMouseInPopUp(ori, ImVec2(200.0f, 250.0f)))
        {
            popUpOpen = false;
            ImGui::CloseCurrentPopup();
        }
    }

    if (optionsOpen)
    {
        ImGui::SetNextWindowSize(ImVec2(200.0f, 250.0f));
        if (ImGui::BeginPopup("Options"))
        {
            if (ImGui::Selectable("Unparent", false, 0, ImVec2(200, 15)))
            {
                optionedGameObject->SetParent(app->engine_order->rootGameObject);
                optionedGameObject = nullptr;
                optionsOpen = false;
            }
            if (ImGui::Selectable("Delete", false, 0, ImVec2(200, 15)))
            {
                optionedGameObject->DeleteGameObject();
                optionedGameObject = nullptr;
                optionsOpen = false;
            }
            ImGui::EndPopup();
        }
        if (!ImGuiH::CheckMouseInPopUp(ori, ImVec2(200.0f, 250.0f)))
        {
            optionsOpen = false;
            ImGui::CloseCurrentPopup();
        }
    }

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
        ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration); ImGui::SameLine(ImGui::GetWindowWidth() - 28);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
        {
            if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left)) editor->SetSelectedGameObject(gameObject);
            if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
            {
                optionedGameObject = gameObject;
                optionsOpen = !optionsOpen;
                if (optionsOpen)
                {
                    ImGui::OpenPopup("Options");
                    ori = ImGui::GetMousePosOnOpeningCurrentPopup();
                }
                else
                {
                    ImGui::CloseCurrentPopup();
                }
            }
        }

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload(gameObject->name.c_str(), gameObject, sizeof(GameObject*));

            draggingGameObject = gameObject;

            ImGui::Text("Change GO parent");
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(draggingGameObject->name.c_str()))
            {
                draggingGameObject->SetParent(gameObject);
                draggingGameObject = nullptr;
            }
            ImGui::EndDragDropTarget();
        }

        if (gameObject->visible_on_editor)
        {
            std::string s = ICON_FA_EYE "##";
            s += std::to_string(gameObject->id);
            if (ImGui::Button(s.c_str()) && gameObject->GetParent()->visible_on_editor)
            {
                SetVisibleOnEditor(gameObject, gameObject, false, true);
            }
        }
        else
        {
            std::string s = ICON_FA_EYE_SLASH "##";
            s += std::to_string(gameObject->id);
            if (ImGui::Button(s.c_str()) && gameObject->GetParent()->visible_on_editor)
            {
                SetVisibleOnEditor(gameObject, gameObject, true, true);
            }
        }
        node_open = false;
    }
    else
    {
        ImGui::AlignTextToFramePadding();
        node_flags |= ImGuiTreeNodeFlags_AllowItemOverlap;
        node_open = ImGui::TreeNodeEx((void*)(intptr_t)iteration, node_flags, gameObject->name.c_str(), iteration); ImGui::SameLine(ImGui::GetWindowWidth() - 28);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenOverlapped))
        {
            if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Left)) editor->SetSelectedGameObject(gameObject);
            if (ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
            {
                optionedGameObject = gameObject;
                optionsOpen = !optionsOpen;
                if (optionsOpen)
                {
                    ImGui::OpenPopup("Options");
                    ori = ImGui::GetMousePosOnOpeningCurrentPopup();
                }
                else
                {
                    ImGui::CloseCurrentPopup();
                }
            }
        }

        if (ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload(gameObject->name.c_str(), gameObject, sizeof(GameObject*));

            draggingGameObject = gameObject;

            ImGui::Text("Change game object parent");
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(draggingGameObject->name.c_str()))
            {
                draggingGameObject->SetParent(gameObject);
                draggingGameObject = nullptr;
            }
            ImGui::EndDragDropTarget();
        }

        if (gameObject->visible_on_editor)
        {
            std::string s = ICON_FA_EYE "##";
            s += std::to_string(gameObject->id);
            if (ImGui::Button(s.c_str()) && gameObject->GetParent()->visible_on_editor)
            {
                SetVisibleOnEditor(gameObject, gameObject, false, true);
            }
        }
        else
        {
            std::string s = ICON_FA_EYE_SLASH "##";
            s += std::to_string(gameObject->id);
            if (ImGui::Button(s.c_str()) && gameObject->GetParent()->visible_on_editor)
            {
                SetVisibleOnEditor(gameObject, gameObject, true, true);
            }
        }
    }

    if (node_open)
    {
        if (!gameObject->children.empty()) DrawGameObjectChildren(gameObject, true);
        ImGui::TreePop();
    }
}

void EW_Hierarchy::SetVisibleOnEditor(GameObject* first_game_object, GameObject* game_object, bool visible_on_editor, bool it_one)
{
    game_object->visible_on_editor = visible_on_editor;
    C_MeshRenderer* mr = dynamic_cast<C_MeshRenderer*>(game_object->GetComponent(Component::TYPE::MESH_RENDERER));
    if (mr != NULL && first_game_object->GetParent()->visible_on_editor)
    {
        mr->GetMesh().visible_on_editor = visible_on_editor;
    }
    for (auto& childs : game_object->GetChilds())
    {
        SetVisibleOnEditor(first_game_object, childs, visible_on_editor, false);
    }
}