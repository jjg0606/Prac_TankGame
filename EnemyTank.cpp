#include "EnemyTank.h"
#include "TankGameScene.h"

#include "EnumRenderOrder.h"
using namespace std;

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

}

void EnemyTank::Update()
{
	float deltime = getDeltaTime().count();
	timeNextDirection -= deltime;
	timeNextShot -= deltime;
	if (timeNextDirection < 0.0f)
	{
		setTimeNextDirection();
		LookingDirection = rand() % 4;
	}
	if (timeNextShot < 0.0f)
	{
		setTimeNextShot();
		ShotSequence();
	}
	// TODO::
}

void EnemyTank::LateUpdate()
{
	if (haveToGoingBack || isOutOfBoundary())
	{
		haveToGoingBack = false;
		position = position - delta;
		Collider::MoveDelta(-delta.x, -delta.y);
	}
	delta = Vector2D<int>{ 0,0 };
	ImageRenderer::SetPosition(position.x, position.y);
}

void EnemyTank::Start()
{
	animCounter = 0.0f;
	movingDistLeft = 0.0f;
	setTimeNextDirection();
	setTimeNextShot();

}

void EnemyTank::SendMsg(int code)
{

}

void EnemyTank::ShotSequence()
{
	if (missile->Collider::isActive)
	{
		return;
	}

	missile->Init(LookingDirection, position.x, position.y, false);
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

void EnemyTank::setTimeNextShot()
{
	timeNextShot = minDelayNextShot + (rand() % 10)*0.25;
}