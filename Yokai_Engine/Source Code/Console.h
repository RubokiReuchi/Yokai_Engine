#pragma once

#include "Globals.h"

#include <string>
#include <map>

#define LOG_PATH "Config/console_log.txt"
#define MAX_CONSOLE_LOGS 3000

namespace Htool
{
	template<class T> class CycleArray;
}

using CArrayS = Htool::CycleArray<std::string>;

class Console
{
public:
	static void S_Init();

	static void S_Close();

	static void S_Log(const std::string text);

	static uint S_GetLog(std::string** buffer);

	static std::map<std::string, uint> S_GetCollapseLog();

	static std::string S_GetLastLog();

	static const char* S_GetLogCounts();

	static void S_ClearLog();

	static void S_SaveLog();

private:
	static CArrayS* buffers;

	static std::map<std::string, uint> buffers_map;

	static std::string log_count_text;

	static size_t log_count;
};