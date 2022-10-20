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
};