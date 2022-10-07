#pragma once

struct Rect
{
	int x, y, w, h;
	Rect(int _x, int _y, int _w, int _h) : x(_x), y(_y), w(_w), h(_h) {}
	Rect() : x(0), y(0), w(0), h(0) {}
	bool Contains(Rect b);
};