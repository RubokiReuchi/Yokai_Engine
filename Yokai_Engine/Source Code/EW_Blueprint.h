#pragma once

#include "EditorWindow.h"

#include "ImGuiNodeEditor/imgui_node_editor.h"
#include "ImGui/imgui_internal.h"
#include "ImGuiNodeEditor/utilities/widgets.h"

namespace BP = ax::NodeEditor;
namespace WG = ax::Widgets;

struct Node;
struct LinkInfo;
struct Pin;

class EW_Blueprint : public EditorWindow
{
public:
	EW_Blueprint();

	~EW_Blueprint();

	void Update() override;

private:
	BP::EditorContext* context;
	ImVector<Node> nodes;
	ImVector<LinkInfo> links;
	ImVector<Pin> pins;
	int nextLinkId = 100;
};