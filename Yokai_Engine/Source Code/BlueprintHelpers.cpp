#include "BlueprintHelpers.h"

// BP
void NH::BeginColumn()
{
    ImGui::BeginGroup();
}

void NH::NextColumn()
{
    ImGui::EndGroup();
    ImGui::SameLine();
    ImGui::BeginGroup();
}

void NH::EndColumn()
{
    ImGui::EndGroup();
}

bool NH::CanLink(BP_Pin a, BP_Pin b)
{
    if (a.type == b.type && a.kind != b.kind && a.node->id != b.node->id)
    {
        return true;
    }
        
    return false;
}

// WG
ImColor NH::GetIconColor(PinType type)
{
    switch (type)
    {
    default:
    case PinType::Executable: return ImColor(255, 255, 255);
    case PinType::Bool: return ImColor(220, 48, 48);
    case PinType::Int: return ImColor(68, 201, 156);
    case PinType::Float: return ImColor(147, 226, 74);
    case PinType::String: return ImColor(124, 21, 153);
    case PinType::Object: return ImColor(51, 150, 215);
    case PinType::Function: return ImColor(218, 0, 183);
    case PinType::None: return ImColor(0, 0, 0);
    }
}

void NH::PinIcon(BP_Pin& pin, bool connected, int alpha)
{
    IconType iconType;
    ImColor color = GetIconColor(pin.type);
    color.Value.w = alpha / 255.0f;
    switch (pin.type)
    {
    case PinType::Executable:     iconType = IconType::Flow;   break;
    case PinType::Bool:     iconType = IconType::Circle; break;
    case PinType::Int:      iconType = IconType::Circle; break;
    case PinType::Float:    iconType = IconType::Circle; break;
    case PinType::String:   iconType = IconType::Circle; break;
    case PinType::Object:   iconType = IconType::Circle; break;
    case PinType::Function: iconType = IconType::Circle; break;
    default:
        return;
    }

    ax::Widgets::Icon(ImVec2(24, 24), iconType, connected, color, ImColor(32, 32, 32, alpha));
}