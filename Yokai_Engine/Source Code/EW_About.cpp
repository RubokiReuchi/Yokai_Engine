#include "EW_About.h"

EW_About::EW_About()
{
	window_name = "About";
	enabled = false;
}

EW_About::~EW_About()
{
}

void EW_About::Update()
{
	// About
	ImGui::Begin(window_name.c_str(), &enabled, ImGuiWindowFlags_NoCollapse);
	ImGui::TextColored({ 240, 138, 0, 255 }, "Yokai Engine");
	ImGui::Text("A 3D Game Engine");
	ImGui::Text("By Ruben Ayora");
	ImGui::Text("");
	ImGui::Text("3rd Party Libraries used:");
	ImGui::Bullet(); if (ImGui::Button("Glew 2.1.0.0")) OsOpenInShell("https://github.com/nigels-com/glew");
	ImGui::Bullet(); if (ImGui::Button("ImGui 1.88")) OsOpenInShell("https://github.com/ocornut/imgui");
	ImGui::Bullet(); if (ImGui::Button("Json parser 1.1.0")) OsOpenInShell("https://github.com/json-parser/json-parser");
	ImGui::Bullet(); if (ImGui::Button("MathGeoLib 1.15")) OsOpenInShell("https://github.com/juj/MathGeoLib");
	ImGui::Bullet(); if (ImGui::Button("SDL 2.0.12.0")) OsOpenInShell("https://github.com/libsdl-org/SDL");
	ImGui::Text("MIT License\n\nCopyright(c) 2022 RubokiReuchi\n\nPermission is hereby granted, free of charge, to any person obtaining a copy\nof this softwareand associated documentation files(the 'Software'), to deal\nin the Software without restriction, including without limitation the rights\nto use, copy, modify, merge, publish, distribute, sublicense, and /or sell\ncopies of the Software, and to permit persons to whom the Software is\nfurnished to do so, subject to the following conditions :\nThe above copyright noticeand this permission notice shall be included in all\ncopies or substantial portions of the Software.\nTHE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\nIMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\nFITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\nAUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\nLIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\nOUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\nSOFTWARE.");
	ImGui::End();
}

void EW_About::OsOpenInShell(const char* path)
{
#ifdef _WIN32
	// Note: executable path must use backslashes!
	::ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);
#else
#if __APPLE__
	const char* open_executable = "open";
#else
	const char* open_executable = "xdg-open";
#endif
	char command[256];
	snprintf(command, 256, "%s \"%s\"", open_executable, path);
	system(command);
#endif
}