#include "EnemyTank.h"
#include "TankGameScene.h"
#include "EnumColMsg.h"
#include "EnumRenderOrder.h"
using namespace std;

const Vector2D<int> EnemyTank::speedUnit[] = { 
	//D_LEFT
	Vector2D<int>{-1,0},
	//D_RIGHT
	Vector2D<int>{1,0},
	//D_UP 
	Vector2D<int>{0,-1},
	//D_DOWN
	Vector2D<int>{0,1} 
};

EnemyTank::EnemyTank(int xpos, int ypos,Missile* mi)
{
	this->missile = mi;

	position = Vector2D<int>{ xpos,ypos };

	unordered_map<int, Image*>& imgMap = *requireResource< unordered_map<int, Image*>>("imgmap");	
	imgVec.resize(4);
	imgVec[D_LEFT].push_back(imgMap[E_TANK_LEFT_0]);
	imgVec[D_LEFT].push_back(imgMap[E_TANK_LEFT_1]);
	imgVec[D_RIGHT].push_back(imgMap[E_TANK_RIGHT_0]);
	imgVec[D_RIGHT].push_back(imgMap[E_TANK_RIGHT_1]);
	imgVec[D_UP].push_back(imgMap[E_TANK_UP_0]);
	imgVec[D_UP].push_back(imgMap[E_TANK_UP_1]);
	imgVec[D_DOWN].push_back(imgMap[E_TANK_DOWN_0]);
	imgVec[D_DOWN].push_back(imgMap[E_TANK_DOWN_1]);

	ImageRenderer::Init(imgVec[LookingDirection][imgVecIndex], RGB(255, 0, 255), ORDER_TANK);
	ImageRenderer::SetPosition(xpos, ypos);
	int width = imgVec[LookingDirection][imgVecIndex]->bmWidth;
	int height = imgVec[LookingDirection][imgVecIndex]->bmHeight;
	RectCollider::Init(xpos + 3, ypos + 3, width - 6, height - 6, COL_TAG_E_TANK);

}

void EnemyTank::OnCollision(Collider* destCol)
{
	switch (destCol->Tag)
	{
	case COL_TAG_E_TANK:
		destCol->SendMsg(COL_MSG_TANK_GO_BACK);
		break;
	default:
		break;
	}
}

void EnemyTank::Update()
{
	float deltime = getDeltaTime().count();
	movingDistLeft = movingDistLeft + deltime * Velocity;

	timeNextDirection -= deltime;
	if (timeNextDirection < 0.0f)
	{
		setTimeNextDirection();
		LookingDirection = rand() % 4;
		animCounter = 0;
		imgVecIndex = 0;
		
	}
	ShotSequence();
	// TODO::
	
	delta = (int)movingDistLeft * speedUnit[LookingDirection];
	movingDistLeft -= (int)movingDistLeft;
	//movingDistLeft -= abs(delta.x) + abs(delta.y);

	animCounter += deltime;
	if (animCounter > animLimit)
	{
		animCounter -= animLimit;
		imgVecIndex = (imgVecIndex + 1) % imgVec[0].size();
	}
	
	Vector2D<int> before = position;
	position = position + delta;
	
	if (isOutOfBoundary())
	{
		position = before;
		delta = delta * 0;
	}
	else
	{
		Collider::MoveDelta(delta.x, delta.y);
	}	
}

void EnemyTank::LateUpdate()
{
	if (haveToGoingBack || isOutOfBoundary())
	{
		haveToGoingBack = false;
		position = position - delta;
		Collider::MoveDelta(-delta.x, -delta.y);
		Collider::MoveDelta(0, 0);
	}
	delta = Vector2D<int>{ 0,0 };
	ImageRenderer::SetPosition(position.x, position.y);
	SetImg(imgVec[LookingDirection][imgVecIndex]);
}

void EnemyTank::Start()
{
	animCounter = 0.0f;
	movingDistLeft = 0.0f;
	setTimeNextDirection();
	LookingDirection = rand() % 4;	
}

void EnemyTank::SendMsg(int code)
{
	switch (code)
	{
	case COL_MSG_TANK_GO_BACK:
		haveToGoingBack = true;
		break;
	default:
		break;
	}
}

void EnemyTank::ShotSequence()
{
	if (missile->Collider::isActive)
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
		fireY = position.y + height * 0.8f;
		break;
	case D_LEFT:
		fireX = position.x;
		fireY = position.y + height / 2;
		break;
	case D_RIGHT:
		fireX = position.x + width * 0.8f;
		fireY = position.y + height / 2;
		break;
	}

	missile->Init(LookingDirection, fireX, fireY, false);
}

bool EnemyTank::isOutOfBoundary()
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

void EnemyTank::setTimeNextDirection()
{
	timeNextDirection = minDelayDirectionChange + (rand() % 10)*0.25;
}
