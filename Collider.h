#pragma once
#include "Vector2D.h"
#include <vector>
#include <Windows.h>
#include "Camera.h"

class Collider
{
private:
	std::vector<Vector2D<int>>& getVertexInfo();
	bool MinMaxTest(Collider& dest);
	bool InclusionTest(Collider& dest,int& xout,int& yout);
	bool IsLineMeet(Vector2D<int> lp1, Vector2D<int> lp2, Vector2D<int> rp1, Vector2D<int> rp2);
	bool IsIncluded(Vector2D<int> pos);
protected:
	std::vector<Vector2D<int>> vertexInfo;
	void SetRectCollider(int left, int top, int right, int bottom);
	void MoveDelta(int dx, int dy);
	void MoveTo(int left, int top);
public:
	virtual bool getMinMax(int& xmin, int& xmax, int& ymin, int& ymax);
	bool isActive = true;
	virtual void DrawCollider(HDC hdc);
	int Tag;
	virtual void ChkCollision(Collider& dest);
	virtual bool isCollide(Collider& dest,int& xposOut,int& yposOut);
	virtual void OnCollision(int destTag, int xpos, int ypos, Collider* destCol) {};
	virtual void SendMsg(int code) {};
	virtual ~Collider() {};
};