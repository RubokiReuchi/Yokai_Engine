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

void NH::HelpText(std::string text)
{
    if (ImGui::IsItemHovered())
    {
        Suspend();
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(text.c_str());
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
        Resume();
    }
}

bool NH::CanLink(BP_Pin* a, BP_Pin* b)
{
    if (a->type == b->type && a->kind != b->kind && a->node->id != b->node->id)
    {
        return true;
    }
        
    return false;
}

BP_Pin* NH::GetPinByID(PinId id, BluePrint* bp)
{
    for (auto& pin : bp->pins)
    {
        if (pin->id == id) return pin;
    }

    return NULL;
}

// WG
ImColor NH::GetIconColor(BP_Pin::TYPE type)
{
    switch (type)
    {
    default:
    case BP_Pin::TYPE::Executable: return ImColor(255, 255, 255);
    case BP_Pin::TYPE::Bool: return ImColor(220, 48, 48);
    case BP_Pin::TYPE::Int: return ImColor(68, 201, 156);
    case BP_Pin::TYPE::Float: return ImColor(147, 226, 74);
    case BP_Pin::TYPE::String: return ImColor(124, 21, 153);
    case BP_Pin::TYPE::Object: return ImColor(51, 150, 215);
    case BP_Pin::TYPE::Function: return ImColor(218, 0, 183);
    case BP_Pin::TYPE::None: return ImColor(0, 0, 0);
    }
}

void NH::PinIcon(BP_Pin& pin, bool connected, float alpha)
{
    IconType iconType;
    ImColor color = GetIconColor(pin.type);
    color.Value.w = alpha / 255.0f;
    switch (pin.type)
    {
    case BP_Pin::TYPE::Executable: iconType = IconType::Flow;   break;
    case BP_Pin::TYPE::Bool: iconType = IconType::Circle; break;
    case BP_Pin::TYPE::Int: iconType = IconType::Circle; break;
    case BP_Pin::TYPE::Float: iconType = IconType::Circle; break;
    case BP_Pin::TYPE::String: iconType = IconType::Circle; break;
    case BP_Pin::TYPE::Object: iconType = IconType::Circle; break;
    case BP_Pin::TYPE::Function: iconType = IconType::Circle; break;
    case BP_Pin::TYPE::None: return; break;
    default:
        return;
    }

    ax::Widgets::Icon(ImVec2(24, 24), iconType, connected, color, ImVec4(32, 32, 32, alpha));
}