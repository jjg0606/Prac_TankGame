#include "Missile.h"
#include "EnumDirection.h"
#include "EnumImg.h"
#include "EnumColliderTag.h"
#include "EnumRenderOrder.h"
using namespace std;

const float Missile::movingSpeed = 100.0f;

Missile::Missile(int direction, int x, int y)
	: direction(direction)
{
	GameObject::isActive = false;
	unordered_map<int, Image*>& imgMap = *requireResource<unordered_map<int, Image*>>("imgmap");
	Image* curImg = nullptr;
	switch (direction)
	{
	case D_UP:
		curImg = imgMap[MISSILE_UP];
		break;
	case D_DOWN:
		curImg = imgMap[MISSILE_DOWN];
		break;
	case D_LEFT:
		curImg = imgMap[MISSILE_LEFT];
		break;
	case D_RIGHT:
		curImg = imgMap[MISSILE_RIGHT];
		break;
	default:
		curImg = imgMap[MISSILE_UP];
		break;
	}

	SetImg(curImg);
	ImageRenderer::SetTransparent(RGB(255, 0, 255));

	int width = curImg->bmWidth;
	int height = curImg->bmHeight;

	SetRectCollider(x, y, x + width, y + height);
	Collider::Tag = COL_TAG_MISSILE;
	RectCollider::render = new RectRenderer();
	RectCollider::render->SetWidthHeight(width, height);
	Renderer::zIndex = ORDER_TANK;

}

void Missile::OnCollision(int destTag, int xpos, int ypos, Collider* destCol)
{

}

void Missile::Start()
{
	GameObject::isActive = true;
}

void Missile::Update()
{
	float dx = 0.0f;
	float dy = 0.0f;
	switch (direction)
	{
	case D_UP:
		dy -= movingSpeed * getDeltaTime().count();
		break;
	case D_DOWN:
		dy += movingSpeed * getDeltaTime().count();
		break;
	case D_LEFT:
		dx -= movingSpeed * getDeltaTime().count();
		break;
	case D_RIGHT:
		dx += movingSpeed * getDeltaTime().count();
		break;
	}

	Collider::MoveDelta(dx, dy);
	ImageRenderer::SetPosition(vertexInfo[0].x, vertexInfo[0].y);
	
}