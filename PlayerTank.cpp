#include "PlayerTank.h"
#include "EnumImg.h"
#include "EnumRenderOrder.h"
#include "Missile.h"
#include "TankGameScene.h"
using namespace std;

PlayerTank::PlayerTank(int xpos, int ypos)
{
	position.x = xpos;
	position.y = ypos;

	unordered_map<int, Image*>& imgMap = *requireResource<unordered_map<int, Image*>>("imgmap");

	imgVec.resize(4);
	imgVec[D_LEFT].push_back(imgMap[P_TANK_LEFT_0]);
	imgVec[D_LEFT].push_back(imgMap[P_TANK_LEFT_1]);
	imgVec[D_RIGHT].push_back(imgMap[P_TANK_RIGHT_0]);
	imgVec[D_RIGHT].push_back(imgMap[P_TANK_RIGHT_1]);
	imgVec[D_UP].push_back(imgMap[P_TANK_UP_0]);
	imgVec[D_UP].push_back(imgMap[P_TANK_UP_1]);
	imgVec[D_DOWN].push_back(imgMap[P_TANK_DOWN_0]);
	imgVec[D_DOWN].push_back(imgMap[P_TANK_DOWN_1]);

	
	ImageRenderer::Init(imgVec[LookingDirection][imgVecIndex], RGB(255, 0, 255), ORDER_TANK);
	ImageRenderer::SetPosition(xpos, ypos);	
	int width = imgVec[LookingDirection][imgVecIndex]->bmWidth;
	int height = imgVec[LookingDirection][imgVecIndex]->bmHeight;
	RectCollider::Init(xpos+3, ypos+3, width-6, height-6, COL_TAG_P_TANK);
	
}

void PlayerTank::OnCollision(int destTag, int xpos, int ypos, Collider* destCol)
{

}

void PlayerTank::Update()
{
	int beforeLooking = LookingDirection;
	float deltaTime = getDeltaTime().count();
	movingDistLeft = movingDistLeft + deltaTime * movingSpeed;
	if(GetKey(VK_LEFT))
	{
		LookingDirection = D_LEFT;
		dx -= movingDistLeft;
	}
	else if (GetKey(VK_RIGHT))
	{
		LookingDirection = D_RIGHT;
		dx += movingDistLeft;
	}
	else if (GetKey(VK_DOWN))
	{
		LookingDirection = D_DOWN;
		dy += movingDistLeft;
	}
	else if (GetKey(VK_UP))
	{
		LookingDirection = D_UP;
		dy -= movingDistLeft;
	}
	else
	{
		movingDistLeft = 0.0f;
		animCounter = 0;
	}

	if (GetKey(VK_SPACE) && hasShot == false)
	{
		hasShot = true;
		ShotSequence();
	}
	else if (!GetKey(VK_SPACE))
	{
		hasShot = false;
	}

	movingDistLeft -= abs(dx) + abs(dy);

	if (beforeLooking != LookingDirection)
	{
		animCounter = 0;
		imgVecIndex = 0;
	}
	else
	{
		animCounter += deltaTime;
		if (animCounter > animLimit)
		{
			animCounter -= animLimit;
			imgVecIndex = (imgVecIndex + 1) % 2;
		}
	}

	Collider::MoveDelta(dx, dy);
	position = position + Vector2D<int>{dx, dy};

	ImageRenderer::SetPosition(position.x, position.y);
	SetImg(imgVec[LookingDirection][imgVecIndex]);

}

void PlayerTank::LateUpdate()
{
	if(haveToGoingBack || isOutOfBoundary())
	{
		haveToGoingBack = false;
		position = position - Vector2D<int>{dx, dy};
		Collider::MoveDelta(-dx, -dy);
		ImageRenderer::SetPosition(position.x, position.y);
	}
	dx = 0;
	dy = 0;

}

void PlayerTank::ShotSequence()
{
	Missile* mi = requireResource<Missile>("pmissile");
	if (mi == nullptr)
	{
		return;
	}

	if (mi->Collider::isActive)
	{
		return;
	}
	int fireX = 0;
	int fireY = 0;
	int width = imgVec[0][0]->bmWidth;
	int height = imgVec[0][0]->bmHeight;

	switch (LookingDirection)
	{
	case D_UP:
		fireX = position.x + width / 2;
		fireY = position.y;
		break;
	case D_DOWN:
		fireX = position.x + width / 2;
		fireY = position.y + height;
		break;
	case D_LEFT:
		fireX = position.x;
		fireY = position.y + height / 2;
		break;
	case D_RIGHT:
		fireX = position.x + width;
		fireY = position.y + height / 2;
		break;
	}

	mi->Init(LookingDirection, fireX, fireY, true);
}


void PlayerTank::Start()
{

}

void PlayerTank::SendMsg(int code)
{
	switch (code)
	{
	case 1:
		haveToGoingBack = true;
		break;
	}
}

bool PlayerTank::isOutOfBoundary()
{
	int left = position.x;
	int top = position.y;
	int width = imgVec[LookingDirection][imgVecIndex]->bmWidth;
	int height = imgVec[LookingDirection][imgVecIndex]->bmHeight;

	if (left < TankGameScene::leftBound || top < TankGameScene::topBound
		|| TankGameScene::rightBound < left + width || TankGameScene::bottomBound < top + height)
	{
		return true;
	}
	return false;
}