#include "RectCollider.h"

RectCollider::RectCollider(int left, int top, int width, int height, int tag)
{
	Collider::Tag = tag;
	SetRectCollider(left, top, left + width, top + height);
	render = new RectRenderer();
	render->SetWidthHeight(width, height);
}

bool RectCollider::getMinMax(int& xmin, int& xmax, int& ymin, int& ymax)
{
	if (vertexInfo.size() != 4)
	{
		return false;
	}

	xmin = vertexInfo[0].x;
	xmax = vertexInfo[2].x;
	ymin = vertexInfo[0].y;
	ymax = vertexInfo[2].y;
}

void RectCollider::DrawCollider(HDC hdc)
{
	int xleft = vertexInfo[0].x;
	int ytop = vertexInfo[0].y;
	int width = vertexInfo[2].x - xleft;
	int height = vertexInfo[2].y - ytop;

	render->SetPosition(xleft, ytop);
	render->Render(hdc);
}

RectCollider::~RectCollider()
{
	delete render;
}