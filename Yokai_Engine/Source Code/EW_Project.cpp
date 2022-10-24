#include "EW_Project.h"
#include "ModuleFile.h"
#include "FileTree.hpp"
#include "ImGuiHelpers.h"

EW_Project::EW_Project()
{
	window_name = "Project";
	enabled = true;

    fileTree = currentNode = ModuleFile::S_GetFileTree("Assets");
}

EW_Project::~EW_Project()
{
    RELEASE(fileTree);
}

void EW_Project::Update()
{
	// Project
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
    FileTree* new_dir = nullptr;

    // Filter
    ImGui::AlignTextToFramePadding();
    if (ImGui::Button(ICON_FA_FOLDER_PLUS)) ImGui::OpenPopup("Create"); ImGui::SameLine(ImGui::GetWindowWidth() - 220);
    ImGui::Text(ICON_FA_MAGNIFYING_GLASS); ImGui::SameLine(); filter.Draw("##Filter", 200);
    if (!currentNode->isRoot())
    {
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_ARROW_LEFT))
        {
            new_dir = currentNode->GetParent();
        }
    }
    ImGui::Separator();

    ImVec2 windowSize = ImGui::GetWindowSize();
    static float width1 = 200;
    static float width2 = 1200;
    static float windowInitX = windowSize.x;

    // Splitter
    ImGuiH::DrawSplitter(0, 10, &width1, &width2, 10, 200);
    width2 = (windowSize.x - width1 - 20);

    // Left Box
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.05f, 0.05f, 0.05f, 1.0f));
    ImGui::BeginChild("LeftBox", ImVec2(width1, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
    DrawTreeNode(fileTree);
    ImGui::EndChild();
    ImGui::PopStyleColor();

    ImGui::SameLine();

    // RightBox
    ImGui::BeginChild("RightBox", ImVec2(width2, 0), true, ImGuiWindowFlags_HorizontalScrollbar);
    for (size_t i = 0; i < currentNode->directories.size(); i++)
    {
        std::string s;
        if (currentNode->directories.at(i)->files.empty()) s = ICON_FA_FOLDER_OPEN "\n";
        else s = ICON_FA_FOLDER_CLOSED "\n";
        s += currentNode->directories[i]->name;
        if (ImGui::Button(s.c_str(), ImVec2(120, 120)))
        {
            new_dir = currentNode->directories[i];
        }
        ImGui::SameLine();
    }
    for (size_t i = 0; i < currentNode->files.size(); i++)
    {
        ImGui::Button(currentNode->files[i].c_str(), ImVec2(120, 120));
        ImGui::SameLine();
    }
    ImGui::EndChild();
    if (new_dir) currentNode = new_dir;
	ImGui::End();
}

void EW_Project::DrawTreeNode(const FileTree* node) const
{
    if (ImGui::TreeNode(node->name.c_str()))
    {
        for (size_t i = 0; i < node->directories.size(); i++)
        {
            DrawTreeNode(node->directories[i]);
        }
        for (size_t i = 0; i < node->files.size(); i++)
        {
            ImGui::Text(node->files[i].c_str());
        }
        ImGui::TreePop();
    }
}

void EW_Project::OnDrop(std::string filePath)
{
    std::string ret = ModuleFile::S_NormalizePath(filePath);

    ModuleFile::S_Copy(ret, currentNode->path);

    currentNode->files.push_back(ModuleFile::S_GetFileName(ret));

    //RELEASE(fileTree);

    //fileTree = ModuleFiles::S_GetFileTree("Assets");

    //currentNode = fileTree;
}