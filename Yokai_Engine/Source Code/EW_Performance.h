#pragma once

#include "EditorWindow.h"

class EW_Performance : public EditorWindow
{
public:
	EW_Performance();

	~EW_Performance();

	void Update() override;

private:

	std::vector<float> frame_array;
	float* frames = nullptr;

	bool fullscreen = false;

	int countCPU = 0;

	float systemRAM = 0;

	int* frameLimit = nullptr;

	const int* windowWidth = nullptr;
	const int* windowHeight = nullptr;

	bool* isVSyncOn = nullptr;
};