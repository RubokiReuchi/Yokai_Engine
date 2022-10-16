#include "Console.h"
#include "ModuleFile.h"
#include "CycleArray.hpp"

CArrayS* Console::buffers = nullptr;
std::map<std::string, uint> Console::buffers_map;
std::string Console::log_count_text = "";
size_t Console::log_count = 0;

void Console::S_Init()
{
    static bool isInit = false;

    if (isInit) return;

    buffers = new CArrayS(MAX_CONSOLE_LOGS);
}

void Console::S_Close()
{
    static bool isClosed = false;

    if (isClosed) return;

    RELEASE(buffers);
}

void Console::S_Log(const std::string text)
{
    buffers->push_back("\nDebug.Log: " + text);

    auto it = buffers_map.find(text);

    if (it == buffers_map.end()) buffers_map.insert(std::make_pair(text, 0));

    buffers_map[text]++;

    log_count++;
}

uint Console::S_GetLog(std::string** buffer)
{
    *buffer = buffers->front();

    return buffers->size();
}

std::map<std::string, uint> Console::S_GetCollapseLog()
{
    return buffers_map;
}

std::string Console::S_GetLastLog()
{
    return *(buffers->front() + buffers->size());
}

const char* Console::S_GetLogCounts()
{
    log_count_text = log_count > 999 ? "999+" : std::to_string(log_count);

    return log_count_text.c_str();
}

void Console::S_ClearLog()
{
    buffers->reset();

    buffers_map.clear();

    log_count_text.clear();

    log_count = 0;
}

void Console::S_SaveLog()
{
    std::string buffer = "DEBUG INFO:\n";

    // Read all context in the _buffers and put into buffer
    auto* b = buffers->front();

    for (uint i = 0; i < buffers->size(); i++, b++) buffer += *b;

    // Convert string buffer to char* buffer
    uint n = buffer.size() + 1;

    char* arr = new char[n];

    strcpy_s(arr, n, buffer.c_str());

    // Save buffer info and release unnecessary memory
    ModuleFile::S_Save(LOG_PATH, arr, n, false);

    RELEASE_ARRAY(arr);
}