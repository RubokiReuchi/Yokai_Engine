#include "EW_Blueprint.h"
#include "BlueprintHelpers.h"


EW_Blueprint::EW_Blueprint()
{
	window_name = "Blueprint";
	enabled = true;

	BP::Config config;
	config.SettingsFile = "Config/node.json";
	context = BP::CreateEditor(&config);
}

EW_Blueprint::~EW_Blueprint()
{
	BP::DestroyEditor(context);
}

void EW_Blueprint::Update()
{
	// Blueprint
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoCollapse/* | ImGuiWindowFlags_NoFocusOnAppearing*/);
	BP::SetCurrentEditor(context);
    
	BP::Begin("Node Editor");
    int unique_id = 1;

    BP::NodeId nodeA_id = unique_id++;
    Pin a_inPin(unique_id++, "-> In", PinType::Float);
    Pin a_outPin(unique_id++, "-> Out", PinType::Float);

    BP::BeginNode(nodeA_id);
    ImGui::Text("Node A");
    BP::BeginPin(a_inPin.id, BP::PinKind::Input);
    NodeEditorH::PinIcon(a_inPin, NodeEditorH::IsPinLinked(a_inPin.id, links), 255);
    BP::EndPin();
    ImGui::SameLine(55);
    BP::BeginPin(a_outPin.id, BP::PinKind::Output);
    NodeEditorH::PinIcon(a_outPin, NodeEditorH::IsPinLinked(a_outPin.id, links), 255);
    BP::EndPin();
    BP::EndNode();

    BP::NodeId nodeB_id = unique_id++;
    Pin b_inPin(unique_id++, "-> In", PinType::Float);
    Pin b_outPin(unique_id++, "-> Out", PinType::Float);

    BP::BeginNode(nodeB_id);
    ImGui::Text("Node B");
    BP::BeginPin(b_inPin.id, BP::PinKind::Input);
    NodeEditorH::PinIcon(b_inPin, NodeEditorH::IsPinLinked(b_inPin.id, links), 255);
    BP::EndPin();
    ImGui::SameLine(55);
    BP::BeginPin(b_outPin.id, BP::PinKind::Output);
    NodeEditorH::PinIcon(b_outPin, NodeEditorH::IsPinLinked(b_outPin.id, links), 255);
    BP::EndPin();
    BP::EndNode();

    for (auto& link : links)
    {
        BP::Link(link.id, link.input_id, link.output_id, link.color);
    }

    if (BP::BeginCreate())
    {
        BP::PinId inputPinId, outputPinId;
        if (BP::QueryNewLink(&inputPinId, &outputPinId))
        {
            if (inputPinId && outputPinId)
            {
                if (BP::AcceptNewItem())
                {
                    for (auto& pin : BluePrint::pins)
                    {
                        if (pin.id == inputPinId)
                        {
                            links.push_back(LinkInfo(BP::LinkId(nextLinkId++), inputPinId, outputPinId, pin.type));
                            BP::Link(links.back().id, links.back().input_id, links.back().output_id);
                        }
                    }
                }
            }
        }
    }
    BP::EndCreate();


    if (BP::BeginDelete())
    {
        BP::LinkId deletedLinkId;
        while (BP::QueryDeletedLink(&deletedLinkId))
        {
            if (BP::AcceptDeletedItem())
            {
                for (auto& link : links)
                {
                    if (link.id == deletedLinkId)
                    {
                        links.erase(&link);
                        break;
                    }
                }
            }
        }
    }
    BP::EndDelete();

	BP::End();

	BP::SetCurrentEditor(NULL);

	ImGui::End();
}