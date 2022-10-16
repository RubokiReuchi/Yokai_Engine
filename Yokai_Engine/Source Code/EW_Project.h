#pragma once

#include "EditorWindow.h"

class EW_Project : public EditorWindow
{
public:
	EW_Project();

	~EW_Project();

	void Update() override;

private:

	ImGuiTextFilter filter;

	void DrawSplitter(int split_vertically, float thickness, float* size0, float* size1, float min_size0, float min_size1);
};