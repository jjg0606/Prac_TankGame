#pragma once
#include "RectCollider.h"
#include "GameObject.h"
#include "ImageRenderer.h"
#include "Missile.h"
#include "EnumDirection.h"

class EnemyTank
	: virtual public GameObject, virtual public RectCollider, virtual public ImageRenderer
{
private:
	Vector2D<int> position{ 0,0 };
	int LookingDirection = D_DOWN;
	std::vector<std::vector<Image*>> imgVec;
	int imgVecIndex = 0;
	const float animLimit = 0.1f;
	float animCounter = 0.0f;
	const float movingSpeed = 100.0f;
	float movingDistLeft = 0.0f;
	Vector2D<int> delta{ 0,0 };
	void ShotSequence();

	const float minDelayDirectionChange = 1.0f;
	float timeNextDirection = 0.0f;
	const float minDelayNextShot = 5.0f;
	float timeNextShot = 0.0f;
	Missile* missile;
	bool isOutOfBoundary();
	bool haveToGoingBack = false;
	void setTimeNextDirection();
	void setTimeNextShot();
	
public:
	EnemyTank(int xpos, int ypos, Missile* mi);
	void OnCollision(Collider* destCol) override;
	void Update() override;
	void LateUpdate() override;
	void Start() override;
	void SendMsg(int code) override;
};