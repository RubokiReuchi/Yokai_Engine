#pragma once

#include <queue>

#include "EditorWindow.h"

namespace Htool
{
	template<class T> class CycleArray;
}

using CArrayF = Htool::CycleArray<float>;

class EW_Performance : public EditorWindow
{
public:
	EW_Performance();

	~EW_Performance();

	void Update() override;

private:

	Htool::CycleArray<float>* frames = nullptr;

	bool fullscreen = false;

	int countCPU = 0;

	float systemRAM = 0;

	int* frameLimit = nullptr;

	const int* windowWidth = nullptr;
	const int* windowHeight = nullptr;

	bool* isVSyncOn = nullptr;
};