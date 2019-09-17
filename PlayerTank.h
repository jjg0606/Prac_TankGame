#pragma once
#include "RectCollider.h"
#include "EnumBlockState.h"
#include "EnumBlockType.h"
#include "ImageRenderer.h"
#include "EnumColliderTag.h"
#include "ImageRenderer.h"
#include <string>
#include <vector>
#include "GameObject.h"
#include "EnumDirection.h"

class PlayerTank
	: virtual public GameObject, virtual public RectCollider, virtual public ImageRenderer
{
private:
	Vector2D<int> position{ 0,0 };
	int LookingDirection = D_UP;
	std::vector<std::vector<Image*>> imgVec;
	int imgVecIndex = 0;
	const float animLimit = 0.1f;
	float animCounter = 0.0f;
	const float movingSpeed = 100.0f;
	float movingDistLeft = 0.0f;
	int dx = 0;
	int dy = 0;
	bool haveToGoingBack = false;
	void ShotSequence();
	bool isOutOfBoundary();
	bool hasShot = false;
public:
	PlayerTank(int xpos, int ypos);
	void OnCollision(Collider* destCol) override;
	virtual void Update() override;
	virtual void LateUpdate() override;
	virtual void Start() override;
	virtual void SendMsg(int code) override;
};