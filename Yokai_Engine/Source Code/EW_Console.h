#pragma once

#include "EditorWindow.h"

class EW_Console : public EditorWindow
{
public:
	EW_Console();

	~EW_Console();

	void Update() override;
};