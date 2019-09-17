#pragma once
#include "Vector2D.h"

// ax + b = y �� ����
// a�� �ش��ϴ� ���� xparam
// b�� �ش��ϴ� ���� cons
// c�� �ش��ϴ� ���� yparam
// ���ǿ��� �ش��ϴ� ���� start end
class Line
{
private:

	// Yparal - not Yparal
	bool resultYParalOne(Line& dest);
	// Yparal - Yparal
	bool resultYParalEach(Line & dest);
	// Ordinary one
	bool resultOrdinary(Line& dest);
	float xparam;
	float cons;
	float yparam;
	int start;
	int end;
public:
	float getY(float X);
	Line(Vector2D<int> p1, Vector2D<int> p2);
	bool isMeet(Line& dest);
};