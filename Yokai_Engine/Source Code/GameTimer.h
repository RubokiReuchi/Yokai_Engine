#pragma once

#include "Globals.h"

struct GameTimer
{
	GameTimer()
	{
		frameCount = 0;
		time = 0.0f;
		time_scale = 0.0f;
		dt = 0.0f;
		realTime_frameCount = 0;
		realTime_dt = 0.0f;
	}

	uint frameCount;
	float time;
	float time_scale;
	float dt;
	uint realTime_frameCount;
	float realTime_dt;

	void Reset()
	{
		frameCount = 0;
		time = 0.0f;
		time_scale = 0.0f;
		dt = 0.0f;
	}
};