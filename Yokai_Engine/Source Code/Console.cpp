#include "Console.h"
#include "ModuleFile.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"

std::map<uint, std::string> Console::console;
size_t Console::log_count = 0;

void Console::LogInConsole(const std::string text)
{
    console[log_count++] = (" > " + text);
}

void Console::DrawConsoleLog()
{
    for (auto& log : console)
    {
        ImGui::Text(log.second.c_str());
    }
}

std::string Console::GetLastLog()
{
    std::map<uint, std::string>::iterator aux = console.end();
    return aux->second;
}

void Console::ClearConsoleLog()
{
    console.clear();
    log_count = 0;
}

void Console::SaveConsoleLog()
{
    std::string save = "";

    for (auto& log : console)
    {
        save += log.second;
        save += "\n";
    }

    uint n = save.size() + 1;

    char* arr = new char[n];

    strcpy_s(arr, n, save.c_str());

    ModuleFile::FS_Save(LOG_PATH, arr, n, false);

    RELEASE_ARRAY(arr);
}