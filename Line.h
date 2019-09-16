#pragma once
#include "Vector2D.h"

class Line
{
private:
	bool resultYParal(Line& dest);
public:
	bool isYParal;
	int xmin;
	int xmax;
	int ymin;
	int ymax;
	float dec;
	Line(Vector2D<int> p1, Vector2D<int> p2);
	bool isMeet(Line& dest);
};