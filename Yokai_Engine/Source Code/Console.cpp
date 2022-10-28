#include "Console.h"
#include "ModuleFile.h"
#include "CycleArray.hpp"

CArrayS* Console::buffers = nullptr;
std::map<std::string, uint> Console::buffers_map;
std::string Console::log_count_text = "";
size_t Console::log_count = 0;

void Console::InitConsole()
{
    static bool isInit = false;

    if (isInit) return;

    buffers = new CArrayS(MAX_CONSOLE_LOGS);
}

void Console::CloseConsole()
{
    static bool isClosed = false;

    if (isClosed) return;

    RELEASE(buffers);
}

void Console::LogInConsole(const std::string text)
{
    buffers->push_back("\n> " + text);

    auto it = buffers_map.find(text);

    if (it == buffers_map.end()) buffers_map.insert(std::make_pair(text, 0));

    buffers_map[text]++;

    log_count++;
}

uint Console::GetConsoleLog(std::string** buffer)
{
    *buffer = buffers->front();

    return buffers->size();
}

std::map<std::string, uint> Console::GetCollapseLog()
{
    return buffers_map;
}

std::string Console::GetLastLog()
{
    return *(buffers->front() + buffers->size());
}

const char* Console::GetLogCounts()
{
    log_count_text = log_count > 999 ? "999+" : std::to_string(log_count);

    return log_count_text.c_str();
}

void Console::ClearConsole()
{
    buffers->reset();

    buffers_map.clear();

    log_count_text.clear();

    log_count = 0;
}

void Console::SaveConsoleLog()
{
    std::string buffer = "DEBUG INFO:\n";

    // Read all context in the _buffers and put into buffer
    auto* b = buffers->front();

    for (int i = 0; i < buffers->size(); i++, b++) buffer += *b;

    // Convert string buffer to char* buffer
    uint n = buffer.size() + 1;

    char* arr = new char[n];

    strcpy_s(arr, n, buffer.c_str());

    // Save buffer info and release unnecessary memory
    ModuleFile::S_Save(LOG_PATH, arr, n, false);

    RELEASE_ARRAY(arr);
}