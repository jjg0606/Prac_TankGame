#include "Collider.h"
#include <math.h>
#include "Line.h"
using namespace std;

std::vector<Vector2D<int>>& Collider::getVertexInfo()
{
	return vertexInfo;
}

bool Collider::getMinMax(int& xmin, int& xmax, int& ymin, int& ymax)
{
	if (vertexInfo.empty())
	{
		return false;
	}

	xmin = vertexInfo[0].x;
	xmax = vertexInfo[0].x;
	ymin = vertexInfo[0].y;
	ymax = vertexInfo[0].y;

	for (int i = 1; i < vertexInfo.size(); i++)
	{
		xmin = min(xmin, vertexInfo[i].x);
		xmax = max(xmax, vertexInfo[i].x);
		ymin = min(ymin, vertexInfo[i].y);
		ymax = max(ymax, vertexInfo[i].y);
	}
	return true;
}

bool Collider::MinMaxTest(Collider& dest)
{
	int sorXmin, sorXmax, sorYmin, sorYmax;
	if (!getMinMax(sorXmin, sorXmax, sorYmin, sorYmax))
	{
		return false;
	}

	int destXmin, destXmax, destYmin, destYmax;
	if (!dest.getMinMax(destXmin, destXmax, destYmin, destYmax))
	{
		return false;
	}

	if (sorXmax < destXmin || destXmax < sorXmin || sorYmax < destYmin || destYmax < sorYmin)
	{
		return false;
	}

	return true;
}

bool Collider::InclusionTest(Collider& dest, int& xout,int& yout)
{
	static float pi = 3.141592f;
	vector<Vector2D<int>>& destVertex = dest.getVertexInfo();

	for (int i = 0; i < vertexInfo.size(); i++)
	{
		float angleSum = 0.0f;
		for (int j = 0; j < destVertex.size(); j++)
		{
			Vector2D<int> v1 = destVertex[j] - vertexInfo[i];
			int second = (j + 1) % destVertex.size();
			Vector2D<int> v2 = destVertex[second] - vertexInfo[i];

			int inpro = v1 * v2;
			float size = sqrtf(v1.x * v1.x + v1.y * v1.y) * sqrtf(v2.x * v2.x + v2.y * v2.y);
			angleSum += acosf(inpro / size);
		}

		if (angleSum > 1.9 * pi)
		{
			xout = vertexInfo[i].x;
			yout = vertexInfo[i].y;
			return true;
		}
	}
	return false;
}

void Collider::SetRectCollider(int left, int top, int right, int bottom)
{
	vertexInfo.clear();
	vertexInfo.push_back({ left,top });
	vertexInfo.push_back({ right,top });
	vertexInfo.push_back({ right,bottom });
	vertexInfo.push_back({ left,bottom });
}

void Collider::MoveDelta(int dx, int dy)
{
	for (int i = 0; i < vertexInfo.size(); i++)
	{
		vertexInfo[i].x += dx;
		vertexInfo[i].y += dy;
	}
}

void Collider::MoveTo(int left, int top)
{
	if (vertexInfo.empty())
	{
		return;
	}

	int dx = left - vertexInfo[0].x;
	int dy = top - vertexInfo[0].y;
	MoveDelta(dx, dy);
}

void Collider::DrawCollider(HDC hdc)
{

}

bool Collider::isCollide(Collider& dest, int& xposOut, int& yposOut)
{
	if (!MinMaxTest(dest))
	{
		return false;
	}
	if (!InclusionTest(dest, xposOut, yposOut) && !dest.InclusionTest(*this, xposOut, yposOut))
	{
		return false;		
	}
	return true;	
}

void Collider::ChkCollision(Collider& dest)
{
	if (!this->isActive || !dest.isActive)
	{
		return;
	}

	if (!MinMaxTest(dest))
	{
		return;
	}

	for (int i = 0; i < dest.vertexInfo.size(); i++)
	{
		if (this->IsIncluded(dest.vertexInfo[i]))
		{
			this->OnCollision(dest.Tag, dest.vertexInfo[i].x, dest.vertexInfo[i].y, &dest);
			dest.OnCollision(this->Tag, dest.vertexInfo[i].x, dest.vertexInfo[i].y, this);
			return;
		}
	}

	for (int i = 0; i < vertexInfo.size(); i++)
	{
		if (dest.IsIncluded(vertexInfo[i]))
		{
			this->OnCollision(dest.Tag, vertexInfo[i].x, vertexInfo[i].y, &dest);
			dest.OnCollision(this->Tag, vertexInfo[i].x, vertexInfo[i].y, this);
			return;
		}
	}
}




bool Collider::IsIncluded(Vector2D<int> pos)
{
	static const float pi = 3.141592f;
	float angleSum = 0.0f;
	for (int i = 0; i < vertexInfo.size(); i++)
	{
		int next = (i + 1) % vertexInfo.size();
		Vector2D<int> v1 = vertexInfo[i] - pos;
		Vector2D<int> v2 = vertexInfo[next] - pos;

		int inpro = v1 * v2;
		float size = sqrtf(v1.x * v1.x + v1.y * v1.y) * sqrtf(v2.x * v2.x + v2.y * v2.y);
		angleSum += acosf(inpro / size);
	}

	return angleSum > 1.9f * pi ? true : false;
}

bool Collider::IsLineMeet(Vector2D<int> lp1, Vector2D<int> lp2, Vector2D<int> rp1, Vector2D<int> rp2)
{
	Line l1(lp1, lp2);
	Line l2(rp1, rp2);
	return l1.isMeet(l2);
}