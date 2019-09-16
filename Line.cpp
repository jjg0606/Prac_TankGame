#include "Line.h"
#include <math.h>

using namespace std;

Line::Line(Vector2D<int> p1, Vector2D<int> p2)
{
	xmin = p1.x < p2.x ? p1.x : p2.x;
	xmax = p1.x < p2.x ? p2.x : p1.x;
	ymin = p1.y < p2.y ? p1.y : p2.y;
	ymax = p1.y < p2.y ? p2.y : p1.y;
	isYParal = xmin == xmax;
	if (isYParal)
	{
		dec = 0;
	}
	else
	{
		dec = (p2.y - p1.y) / (float)(p2.x - p1.x);
	}
}

bool Line::isMeet(Line& dest)
{
	if (isYParal)
	{
		return resultYParal(dest);
	}


	return false;
}

bool Line::resultYParal(Line& dest)
{
	return false;
}