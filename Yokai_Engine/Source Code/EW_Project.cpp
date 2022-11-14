#include "EW_Project.h"
#include "EO_Editor.h"
#include "ModuleFile.h"
#include "FileTree.hpp"
#include "ImGuiHelpers.h"

EW_Project::EW_Project()
{
	window_name = "Project";
	enabled = true;

    fileTree = currentNode = ModuleFile::FS_GetFileTree("Assets");
    allFiles.clear();
    allFiles = ModuleFile::FS_GetAllFiles("Assets");
}

EW_Project::~EW_Project()
{
    RELEASE(fileTree);
}

void EW_Project::Update()
{
    if (app->file->new_file)
    {
        std::string safe_path = currentNode->path;
        fileTree = currentNode = ModuleFile::FS_GetFileTree("Assets");
        FileTree* tree = currentNode; // from here
        std::string first_folder;
        std::string actual_path = safe_path.substr(safe_path.find_first_of("/") + 1);
        while (!tree->directories.empty())
        {
            if (actual_path == "") break;
            size_t npos = actual_path.find_first_of("/");
            first_folder = actual_path;
            first_folder.erase(npos, actual_path.length());
            for (size_t i = 0; i < tree->directories.size(); i++)
            {
                if (first_folder == tree->directories[i]->name)
                {
                    actual_path.erase(0, first_folder.length() + 1);
                    tree = tree->directories[i];
                }
            }
        } // to here, the currentNode is set to the one that was before reset files
        currentNode = tree;
        allFiles.clear();
        allFiles = ModuleFile::FS_GetAllFiles("Assets");
        app->file->new_file = false;
    }

	// Project
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse);
    FileTree* new_dir = nullptr;

    // Filter
    ImGui::AlignTextToFramePadding();
    if (ImGui::Button(ICON_FA_FOLDER_PLUS)) ImGui::OpenPopup("Create"); 
    if (!currentNode->isRoot())
    {
        ImGui::SameLine();
        if (ImGui::Button(ICON_FA_ARROW_LEFT))
        {
            new_dir = currentNode->GetParent();
        }
    }
    ImGui::SameLine(ImGui::GetWindowWidth() - 220);
    ImGui::Text(ICON_FA_MAGNIFYING_GLASS); ImGui::SameLine(); filter.Draw("##Filter", 200);
    
    ImGui::Separator();

    ImVec2 windowSize = ImGui::GetWindowSize();
    static float width1 = 200;
    static float width2 = 1200;
    static float windowInitX = windowSize.x;

    // Splitter
    ImGuiH::DrawSplitter(0, 10, &width1, &width2, 200, 275);
    width2 = (windowSize.x - width1 - 20);

    // Left Box
    ImGui::BeginChild("LeftBox", ImVec2(width1, 0), true);
    ImGui::SetNextItemOpen(true, ImGuiCond_FirstUseEver);
    DrawTreeNode(fileTree);
    ImGui::EndChild();

    ImGui::SameLine();
    
    // RightBox
    ImGui::BeginChild("RightBox", ImVec2(width2, 0), true);
    if (!filter.IsActive())
    {
        uint rows = 0;
        uint dir_size = currentNode->directories.size();
        for (uint i = 0; i < dir_size; i++)
        {
            std::string s;
            if (currentNode->directories.at(i)->files.empty()) s = ICON_FA_FOLDER_OPEN "\n";
            else s = ICON_FA_FOLDER_CLOSED "\n";
            s += currentNode->directories[i]->name;
            if (ImGui::Button(s.c_str(), ImVec2(120, 120))) new_dir = currentNode->directories[i];

            if (8 + 128 * (i + 2 - rows) > width2) rows = i + 1;
            else ImGui::SameLine();
        }
        for (size_t i = dir_size; i < currentNode->files.size() + dir_size; i++)
        {
            std::string s = ICON_FA_FILE "\n";
            s += currentNode->files[i - dir_size];
            ImGui::Button(s.c_str(), ImVec2(120, 120));
            if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoPreviewTooltip))
            {
                std::string file_path = currentNode->path + currentNode->files[i - dir_size];
                ImGui::SetDragDropPayload(file_path.c_str(), &i, sizeof(std::string));
                app->engine_order->editor->dd_file = file_path;
                ImGui::EndDragDropSource();
            }

            if (8 + 128 * (i + 2 - rows) > width2) rows = i + 1;
            else ImGui::SameLine();
        }
    }
    else
    {
        uint rows = 0;
        uint dir_size = currentNode->directories.size();
        for (size_t i = 0; i < allFiles.size(); i++)
        {
            if (filter.PassFilter(allFiles[i].c_str()))
            {
                std::string s = ICON_FA_FILE "\n";
                s += allFiles[i];
                ImGui::Button(s.c_str(), ImVec2(120, 120));
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoDisableHover | ImGuiDragDropFlags_SourceNoPreviewTooltip))
                {
                    std::string file_path = app->file->FS_GetFileName(allFiles[i]);
                    ImGui::SetDragDropPayload(file_path.c_str(), &i, sizeof(std::string));
                    app->engine_order->editor->dd_file = file_path;
                    ImGui::EndDragDropSource();
                }

                if (8 + 128 * (i + 2 - rows) > width2) rows = i + 1;
                ImGui::SameLine();
            }
        }
    }
    ImGui::EndChild();
    if (new_dir) currentNode = new_dir;
	ImGui::End();
}

std::string EW_Project::GetCurrentNodePath()
{
    return currentNode->path;
}

void EW_Project::DrawTreeNode(FileTree* node)
{
    bool opened = ImGui::TreeNodeEx(node->name.c_str(), ImGuiTreeNodeFlags_OpenOnArrow);
    if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) currentNode = node;
    if (opened)
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