#include "Missile.h"
#include "EnumDirection.h"
#include "EnumImg.h"
#include "EnumColliderTag.h"
#include "EnumRenderOrder.h"
#include "EnumColMsg.h"
#include "TankGameScene.h"
using namespace std;

Missile::Missile()
{
	Collider::isActive = false;
	unordered_map<int, Image*>& imgMap = *requireResource<unordered_map<int, Image*>>("imgmap");
	explodeImgVec.push_back(imgMap[EXPLOSION_0]);
	explodeImgVec.push_back(imgMap[EXPLOSION_1]);
	explodeImgVec.push_back(imgMap[EXPLOSION_2]);
	ImageRenderer::SetTransparent(RGB(255, 0, 255));
	Collider::isContinuous = true;
}

Missile::Missile(int direction, int x, int y,bool isPlayers)
	: direction(direction)
{
	GameObject::isActive = true;
	unordered_map<int, Image*>& imgMap = *requireResource<unordered_map<int, Image*>>("imgmap");
	curImg = nullptr;
	explodeImgVec.push_back(imgMap[EXPLOSION_0]);
	explodeImgVec.push_back(imgMap[EXPLOSION_1]);
	explodeImgVec.push_back(imgMap[EXPLOSION_2]);

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
	ImageRenderer::Init(curImg, RGB(255, 0, 255),ORDER_TANK);
	int colTag = isPlayers ? COL_TAG_P_MISSILE : COL_TAG_E_MISSILE;
	RectCollider::Init(x, y, curImg->bmWidth, curImg->bmHeight, colTag,true);	
}

void Missile::Init(int direction, int x, int y, bool isPlayers)
{
	this->direction = direction;
	unordered_map<int, Image*>& imgMap = *requireResource<unordered_map<int, Image*>>("imgmap");
	curImg = nullptr;
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
	int height = curImg->bmHeight;
	int width = curImg->bmWidth;

	int colTag = isPlayers ? COL_TAG_P_MISSILE : COL_TAG_E_MISSILE;
	RectCollider::Init(x, y, curImg->bmWidth, curImg->bmHeight, colTag);
	Collider::isActive = true;
	ImageRenderer::SetImg(curImg);
	ImageRenderer::SetPosition(vertexInfo[0].x, vertexInfo[0].y);
	isExploded = false;
	animCounter = 0.0f;
	exImgIndex = 0;
	MoveDelta(0, 0);
}

void Missile::OnCollision(Collider* destCol)
{
	switch (destCol->Tag)
	{
	case COL_TAG_E_TANK:
		if (Collider::Tag == COL_TAG_P_MISSILE)
		{
			isExploded = true;
			Collider::Tag = COL_TAG_PASS;
		}
		break;
	case COL_TAG_P_TANK:
		if (Collider::Tag == COL_TAG_E_MISSILE)
		{
			isExploded = true;
			Collider::Tag = COL_TAG_PASS;
		}
		break;
	case COL_TAG_E_MISSILE:
	case COL_TAG_P_MISSILE:
		if (destCol->Tag != Collider::Tag)
		{
			isExploded = true;
		}
		break;
	}
}

void Missile::Start()
{
	GameObject::isActive = true;
}

void Missile::Update()
{
	if (isExploded || Collider::isActive == false)
	{
		return;
	}

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


void Missile::LateUpdate()
{
	if (Collider::isActive == false)
	{
		return;
	}
	if (isOutOfBoundary())
	{
		isExploded = true;
	}

	if (!isExploded)
	{
		return;
	}

	if (!isPositionSet)
	{
		SetExplosion();
	}

	animCounter += getDeltaTime().count();
	if (animCounter >= animLimit)
	{
		animCounter -= animLimit;
		exImgIndex++;
	}

	if (exImgIndex >= explodeImgVec.size())
	{
		Collider::isActive = false;
		return;
	}

	ImageRenderer::SetImg(explodeImgVec[exImgIndex]);
}

void Missile::SendMsg(int code)
{
	switch (code)
	{
	case COL_MSG_MISSILE_EXPLODE:
		isExploded = true;
		Collider::Tag = COL_TAG_PASS;
		break;
	default:
		break;
	}
}

void Missile::SetExplosion()
{
	
	int width = explodeImgVec.front()->bmWidth;
	int height = explodeImgVec.front()->bmHeight;
	RectCollider::MoveDelta(-width / 2, -height / 2);
	ImageRenderer::SetPosition(vertexInfo[0].x, vertexInfo[0].y);
	exImgIndex = 0;
	
	Missile::Tag = COL_TAG_PASS;
	isPositionSet = true;
}

void Missile::Render(HDC hdc)
{
	if (Collider::isActive)
	{
		ImageRenderer::Render(hdc);
	}
}

bool Missile::isOutOfBoundary()
{
	int left = vertexInfo[0].x;
	int top = vertexInfo[0].y;
	int width = curImg->bmWidth;
	int height = curImg->bmHeight;

	if (left < TankGameScene::leftBound || top < TankGameScene::topBound
		|| TankGameScene::rightBound < left + width || TankGameScene::bottomBound < top + height)
	{
		return true;
	}
	return false;
}