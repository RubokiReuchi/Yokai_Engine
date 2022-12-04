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

    // create one NODE version 1
    BP::BeginNode(nodeA_id);
    ImGui::Text("Node A");
    BP::NH_BeginPin(a_inPin, BP::PinKind::Input);
    NH::PinIcon(a_inPin, NH::IsPinLinked(a_inPin.id, links), 255);
    BP::EndPin();
    ImGui::SameLine(55);
    BP::NH_BeginPin(a_outPin, BP::PinKind::Output);
    NH::PinIcon(a_outPin, NH::IsPinLinked(a_outPin.id, links), 255);
    BP::EndPin();
    BP::EndNode();

    BP::NodeId nodeB_id = unique_id++;
    Pin b_inPin(unique_id++, "-> In", PinType::Float);
    Pin b_outPin(unique_id++, "-> Out", PinType::Float);

    BP::BeginNode(nodeB_id);
    ImGui::Text("Node B");
    BP::NH_BeginPin(b_inPin, BP::PinKind::Input);
    NH::PinIcon(b_inPin, NH::IsPinLinked(b_inPin.id, links), 255);
    BP::EndPin();
    ImGui::SameLine(55);
    BP::NH_BeginPin(b_outPin, BP::PinKind::Output);
    NH::PinIcon(b_outPin, NH::IsPinLinked(b_outPin.id, links), 255);
    BP::EndPin();
    BP::EndNode();

    for (auto& link : links)
    {
        BP::Link(link.id, link.input_id, link.output_id, link.color);
    }

    // create links
    if (BP::BeginCreate())
    {
        BP::PinId inputPinId, outputPinId;
        if (BP::QueryNewLink(&inputPinId, &outputPinId))
        {
            Pin aux = NH::GetPinByID(inputPinId);
            if (inputPinId && outputPinId)
            {
                if (BP::AcceptNewItem() && NH::CanLink(aux, NH::GetPinByID(outputPinId)))
                {
                    links.push_back(LinkInfo(BP::LinkId(nextLinkId++), inputPinId, outputPinId, aux.type));
                    BP::Link(links.back().id, links.back().input_id, links.back().output_id);
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
    BluePrint::pins.clear();
	ImGui::End();
}