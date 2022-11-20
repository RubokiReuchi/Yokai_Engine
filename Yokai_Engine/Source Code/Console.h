#pragma once

#include "Globals.h"

#include <string>
#include <map>

#define LOG_PATH "Config/console_log.txt"
#define MAX_CONSOLE_LOGS 3000

class Console
{
public:
	static void LogInConsole(const std::string text);

	static void DrawConsoleLog();

	static std::string GetLastLog();

	static void ClearConsoleLog();

	static void SaveConsoleLog();

private:
	static std::map<uint, std::string> console;
	static size_t log_count;
};