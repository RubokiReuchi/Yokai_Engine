#include "EW_Blueprint.h"
#include "EO_Editor.h"
#include "BlueprintHelpers.h"
#include "ImGuiHelpers.h"


EW_Blueprint::EW_Blueprint()
{
	window_name = "Blueprint";
	enabled = true;

	BP::Config config;
	config.SettingsFile = "Config/node.json";
    config.NavigateButtonIndex = 2;
	context = BP::CreateEditor(&config);
}

EW_Blueprint::~EW_Blueprint()
{
	BP::DestroyEditor(context);
}

void EW_Blueprint::Update()
{
    if (!blueprint_blocked)
    {
        GameObject* selected_go = app->engine_order->editor->GetSelectedGameObject();
        if (selected_go)
        {
            C_Blueprint* bp = dynamic_cast<C_Blueprint*>(selected_go->GetComponent(Component::TYPE::BLUEPRINT));
            if (bp != NULL) current_blueprint = bp->GetBluePrint();
            else current_blueprint = NULL;
        }
        else current_blueprint = NULL;
    }

    ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoCollapse/* | ImGuiWindowFlags_NoFocusOnAppearing*/);

    // Blueprint
    std::string bp_name, lock_icon;
    if (current_blueprint) bp_name = current_blueprint->name;
    else bp_name = "No Blueprint Selected";
    if (blueprint_blocked) lock_icon = ICON_FA_LOCK;
    else lock_icon = ICON_FA_UNLOCK;

    ImGui::Text(bp_name.c_str()); ImGui::SameLine();
    if (ImGuiH::ButtonAlignOnLine(lock_icon.c_str(), 1.0f))
    {
        blueprint_blocked = !blueprint_blocked;
    }

    BP::SetCurrentEditor(context);
    BP::Begin(bp_name.c_str());

    if (current_blueprint)
    {
        int unique_id = 1;

        for (auto& node : current_blueprint->nodes)
        {
            BP::NodeId node_id = unique_id++;
            BP::SetNodePosition(node_id, node->position);
            BP::BeginNode(node_id);
            ImGui::Text(node->name.c_str());

            NH::BeginColumn();
            for (auto& input_pin : node->inputs)
            {
                BP::NH_BeginPin(input_pin, BP::PinKind::Input);
                NH::PinIcon(input_pin, input_pin.IsPinLinked());
                if (input_pin.IsPinLinked() && input_pin.input_text)
                {
                    std::string aux = "##" + std::to_string(unique_id);
                    switch (input_pin.text_type)
                    {
                    case BP_Pin::TextType::COMBO:
                        break;
                    case BP_Pin::TextType::STRING:
                        break;
                    case BP_Pin::TextType::GAMEOBJECT:
                        break;
                    }
                }
                BP::EndPin();
                NH::NextColumn();
            }
            NH::EndColumn();

            NH::BeginColumn();
            for (auto& output_pin : node->outputs)
            {
                BP::NH_BeginPin(output_pin, BP::PinKind::Output);
                NH::PinIcon(output_pin, output_pin.IsPinLinked());
                BP::EndPin();
                NH::NextColumn();
            }
            NH::EndColumn();
        }

        for (auto& link : current_blueprint->links)
        {
            BP::Link(link->id, link->input_id, link->output_id, link->color);
        }

        // create links
        if (BP::BeginCreate())
        {
            BP::PinId inputPinId, outputPinId;
            if (BP::QueryNewLink(&inputPinId, &outputPinId))
            {
                BP_Pin* aux = GetPinByID(inputPinId);
                if (BP::AcceptNewItem() && NH::CanLink(aux, GetPinByID(outputPinId)))
                {
                    BP_Link* new_link = new BP_Link(BP::LinkId(nextLinkId++), inputPinId, outputPinId, NH::GetIconColor(aux->type));
                    current_blueprint->links.push_back(new_link);
                }
            }
        }
        BP::EndCreate();

        // delete links
        if (BP::BeginDelete())
        {
            BP::LinkId deletedLinkId;
            while (BP::QueryDeletedLink(&deletedLinkId))
            {
                if (BP::AcceptDeletedItem())
                {
                    std::vector<BP_Link*>::iterator it;
                    for (auto& link : current_blueprint->links)
                    {
                        if (link->id == deletedLinkId)
                        {
                            it = std::find(current_blueprint->links.begin(), current_blueprint->links.end(), link);
                            current_blueprint->links.erase(it);
                            break;
                        }
                    }
                }
            }
        }
        BP::EndDelete();

        // create node
        BP::Suspend();
        if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_::ImGuiMouseButton_Right))
        {
            ImGui::OpenPopup("New Node");
            popUpOpen = true;
            ori = ImGui::GetMousePosOnOpeningCurrentPopup();
        }
        if (popUpOpen)
        {
            ImGui::SetNextWindowSize(ImVec2(200.0f, 250.0f));
            if (ImGui::BeginPopup("New Node"))
            {
                DisplayNodes();
                ImGui::EndPopup();
            }
            if (!ImGuiH::CheckMouseInPopUp(ori, ImVec2(200.0f, 250.0f)))
            {
                popUpOpen = false;
                ImGui::CloseCurrentPopup();
            }
        }
        BP::Resume();
        
        // delete node
        if (BP::BeginDelete())
        {
            BP::NodeId deletedNodeId;
            while (BP::QueryDeletedNode(&deletedNodeId))
            {
                if (BP::AcceptDeletedItem())
                {
                    std::vector<BP_Node*>::iterator it;
                    for (auto& node : current_blueprint->nodes)
                    {
                        if (node->id == deletedNodeId)
                        {
                            it = std::find(current_blueprint->nodes.begin(), current_blueprint->nodes.end(), node);
                            current_blueprint->nodes.erase(it);
                            break;
                        }
                    }
                }
            }
        }
        BP::EndDelete();
    }

    BP::End();
	BP::SetCurrentEditor(NULL);

	ImGui::End();
}

void EW_Blueprint::DisplayNodes()
{
    //ImGui::AlignTextToFramePadding();
    //ImGui::Text(ICON_FA_MAGNIFYING_GLASS); ImGui::SameLine();
    //filter.Draw("##Filter");
    //std::string nodeNames[numNodes] = { "Camera", "Blueprint" };
    //for (int i = 0; i < numNodes; i++)
    //{
    //    std::string name = nodeNames[i];
    //    if (filter.PassFilter(name.c_str()))
    //    {
    //        if (ImGui::Selectable(name.c_str()))
    //        {
    //            switch (i)
    //            {
    //            case 0: /*selectGameobject->AddComponent(Component::TYPE::CAMERA);*/ break;
    //            case 1: /*selectGameobject->AddComponent(Component::TYPE::BLUEPRINT);*/ break;
    //            }
    //        }
    //    }
    //}
}

BP_Pin* EW_Blueprint::GetPinByID(PinId id)
{
    for (auto& pin : current_blueprint->pins)
    {
        if (pin->id == id) return pin;
    }

    return NULL;
}