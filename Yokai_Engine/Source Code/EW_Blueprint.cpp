#include "EW_Blueprint.h"
#include "EO_Editor.h"
#include "BlueprintHelpers.h"
#include "ImGuiHelpers.h"

EW_Blueprint::EW_Blueprint()
{
	window_name = "Blueprint";
	enabled = true;

    FillNodeList();

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
        for (auto& node : current_blueprint->nodes)
        {
            BP::NodeId node_id = node->id;
            if (!node->init)
            {
                BP::SetNodePosition(node_id, node->position);
                node->init = true;
            }
            BP::PushStyleColor(BP::StyleColor_NodeBorder, ImVec4(node->color.Value.x, node->color.Value.y, node->color.Value.z, 200));
            BP::BeginNode(node_id);
            ImGui::Text(node->name.c_str());

            NH::BeginColumn();
            for (auto& input_pin : node->inputs)
            {
                if (input_pin.type != BP_Pin::TYPE::None)
                {
                    BP::NH_BeginPin(input_pin, BP::PinKind::Input);
                    NH::PinIcon(input_pin, input_pin.IsPinLinked());
                    BP::EndPin();
                    NH::HelpText(input_pin.name);
                }
                ImGui::SetNextItemWidth(40);
                // input box if not linked
                if (!input_pin.IsPinLinked() && input_pin.box_type != BP_Pin::BoxType::NONE)
                {
                    if (input_pin.type != BP_Pin::TYPE::None) ImGui::SameLine();
                    std::string aux_text = "";
                    switch (input_pin.box_type)
                    {
                    case BP_Pin::BoxType::COMBO:
                        aux_text = "##Node Combo" + std::to_string(input_pin.id_as_int);
                        if (BP::BeginNodeCombo(aux_text.c_str(), input_pin.string_box.c_str(), ImGuiComboFlags_HeightSmall))
                        {
                            for (size_t i = 0; i < input_pin.combo_box.size(); i++)
                            {
                                if (ImGui::Selectable(input_pin.combo_box[i].c_str())) input_pin.string_box = input_pin.combo_box[i];
                            }
                            BP::EndNodeCombo();
                        }
                        break;
                    case BP_Pin::BoxType::STRING:
                        aux_text = "##Node String" + std::to_string(input_pin.id_as_int);
                        ImGuiH::InputText(aux_text.c_str(), &input_pin.string_box);
                        break;
                    case BP_Pin::BoxType::NUMBER:
                        aux_text = "##Node Number" + std::to_string(input_pin.id_as_int);
                        ImGuiH::InputText(aux_text.c_str(), &input_pin.string_box, ImGuiInputTextFlags_CharsScientific);
                        break;
                    case BP_Pin::BoxType::NUMBER3:
                        aux_text = "##Node NumberThreeOne" + std::to_string(input_pin.id_as_int);
                        ImGuiH::InputText(aux_text.c_str(), &input_pin.vector3_box[0], ImGuiInputTextFlags_CharsScientific);
                        aux_text = "##Node NumberThreeTwo" + std::to_string(input_pin.id_as_int); ImGui::SetNextItemWidth(40);
                        ImGuiH::InputText(aux_text.c_str(), &input_pin.vector3_box[1], ImGuiInputTextFlags_CharsScientific);
                        aux_text = "##Node NumberThreeThree" + std::to_string(input_pin.id_as_int); ImGui::SetNextItemWidth(40);
                        ImGuiH::InputText(aux_text.c_str(), &input_pin.vector3_box[2], ImGuiInputTextFlags_CharsScientific);
                        break;
                    case BP_Pin::BoxType::GAMEOBJECT:
                        if (input_pin.go_box != NULL) aux_text = input_pin.go_box->name + "##Node Object" + std::to_string(input_pin.id_as_int);
                        else aux_text = "##Node Object" + std::to_string(input_pin.id_as_int);
                        if (ImGui::Button(aux_text.c_str(), ImVec2(40, 0)))
                        {

                        }
                        if (ImGui::BeginDragDropTarget() && app->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_UP)
                        {
                            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(app->engine_order->editor->GetHierarchyWindow()->GetDraggingGO()->name.c_str()))
                            {
                                input_pin.go_box = app->engine_order->editor->GetHierarchyWindow()->GetDraggingGO();
                                app->engine_order->editor->GetHierarchyWindow()->NullDraggingGO();
                            }
                            ImGui::EndDragDropTarget();
                        }
                        break;
                    }
                }
            }
            NH::EndColumn();
            ImGui::SameLine();
            ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical);
            ImGui::SameLine();
            NH::BeginColumn();
            for (auto& output_pin : node->outputs)
            {
                BP::NH_BeginPin(output_pin, BP::PinKind::Output);
                NH::PinIcon(output_pin, output_pin.IsPinLinked());
                BP::EndPin();
                NH::HelpText(output_pin.name);
            }
            NH::EndColumn();

            node->position = BP::GetNodePosition(node_id);
            
            BP::EndNode();
            BP::PopStyleColor();
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
                BP_Pin* aux_input = NH::GetPinByID(inputPinId, current_blueprint);
                BP_Pin* aux_output = NH::GetPinByID(outputPinId, current_blueprint);
                if (BP::AcceptNewItem() && NH::CanLink(aux_input, aux_output))
                {
                    if (aux_input->kind == PinKind::Input)
                    {
                        BP_Link* new_link = new BP_Link(current_blueprint->unique_id++, aux_input->id_as_int, aux_output->id_as_int, NH::GetIconColor(aux_input->type), current_blueprint);
                        current_blueprint->links.push_back(new_link);
                    }
                    else
                    {
                        BP_Link* new_link = new BP_Link(current_blueprint->unique_id++, aux_output->id_as_int, aux_input->id_as_int, NH::GetIconColor(aux_input->type), current_blueprint);
                        current_blueprint->links.push_back(new_link);
                    }
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
        if (ImGui::IsWindowHovered() && app->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_DOWN)
        {
            ImGui::OpenPopup("New Node");
            popUpOpen = true;
            ori = ImGui::GetMousePosOnOpeningCurrentPopup();
            BP::Resume();
            canvas_ori = ImGui::GetMousePosOnOpeningCurrentPopup();
            BP::Suspend();
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
                            for (auto& input : node->inputs)
                            {
                                current_blueprint->DestroyConectedLinks(input.id);
                            }
                            for (auto& output : node->outputs)
                            {
                                current_blueprint->DestroyConectedLinks(output.id);
                            }
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
    ImGui::AlignTextToFramePadding();
    ImGui::Text(ICON_FA_MAGNIFYING_GLASS); ImGui::SameLine();
    filter.Draw("##Filter");

    // input
    if (ImGui::CollapsingHeader("Input"))
    {
        for (auto& node : node_list[0])
        {
            if (filter.PassFilter(node.c_str()))
            {
                if (ImGui::Selectable(node.c_str()))
                {
                    current_blueprint->CreateNode(node, canvas_ori);
                    popUpOpen = false;
                }
            }
        }
    }
    // variable
    if (ImGui::CollapsingHeader("Variable"))
    {
        for (auto& node : node_list[1])
        {
            if (filter.PassFilter(node.c_str()))
            {
                if (ImGui::Selectable(node.c_str()))
                {
                    current_blueprint->CreateNode(node, canvas_ori);
                    popUpOpen = false;
                }
            }
        }
    }
    // action
    if (ImGui::CollapsingHeader("Action"))
    {
        for (auto& node : node_list[2])
        {
            if (filter.PassFilter(node.c_str()))
            {
                if (ImGui::Selectable(node.c_str()))
                {
                    current_blueprint->CreateNode(node, canvas_ori);
                    popUpOpen = false;
                }
            }
        }
    }
    // modify
    if (ImGui::CollapsingHeader("Modify"))
    {
        for (auto& node : node_list[3])
        {
            if (filter.PassFilter(node.c_str()))
            {
                if (ImGui::Selectable(node.c_str()))
                {
                    current_blueprint->CreateNode(node, canvas_ori);
                    popUpOpen = false;
                }
            }
        }
    }
    // get value
    if (ImGui::CollapsingHeader("Get Value"))
    {
        for (auto& node : node_list[4])
        {
            if (filter.PassFilter(node.c_str()))
            {
                if (ImGui::Selectable(node.c_str()))
                {
                    current_blueprint->CreateNode(node, canvas_ori);
                    popUpOpen = false;
                }
            }
        }
    }
    // operation
    if (ImGui::CollapsingHeader("Operation"))
    {
        for (auto& node : node_list[5])
        {
            if (filter.PassFilter(node.c_str()))
            {
                if (ImGui::Selectable(node.c_str()))
                {
                    current_blueprint->CreateNode(node, canvas_ori);
                    popUpOpen = false;
                }
            }
        }
    }
}

void EW_Blueprint::FillNodeList()
{
    std::vector<std::string> aux;

    // input
    aux.push_back("Press Key");
    aux.push_back("Mouse Motion X");
    aux.push_back("Mouse Motion Y");

    node_list.push_back(aux);
    aux.clear();

    // variable
    aux.push_back("String");
    aux.push_back("Boolean");
    aux.push_back("Float");
    aux.push_back("Vector3");
    aux.push_back("Integer");
    aux.push_back("Game Object");

    node_list.push_back(aux);
    aux.clear();

    // action
    aux.push_back("Print String");
    aux.push_back("Spawn Projectile");

    node_list.push_back(aux);
    aux.clear();

    // modify
    aux.push_back("Translate");
    aux.push_back("Rotate");

    node_list.push_back(aux);
    aux.clear();

    // get value
    aux.push_back("Get Delta Time");
    aux.push_back("Get Forward");
    aux.push_back("Get Up");
    aux.push_back("Get Right");

    node_list.push_back(aux);
    aux.clear();

    // operation
    aux.push_back("Multiply");
    aux.push_back("Multiply Vector3");

    node_list.push_back(aux);
    aux.clear();
}