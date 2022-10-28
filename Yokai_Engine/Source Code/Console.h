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
	static void InitConsole();

	static void CloseConsole();

	static void LogInConsole(const std::string text);

	static uint GetConsoleLog(std::string** buffer);

	static std::map<std::string, uint> GetCollapseLog();

	static std::string GetLastLog();

	static const char* GetLogCounts();

	static void ClearConsole();

	static void SaveConsoleLog();

private:
	static CArrayS* buffers;
	static std::map<std::string, uint> buffers_map;
	static std::string log_count_text;
	static size_t log_count;
};