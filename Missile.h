#pragma once
#include "GameObject.h"
#include "ImageRenderer.h"
#include "RectCollider.h"
#include "EnumColliderTag.h"
#include <string>

class Missile
	: virtual public GameObject, virtual public RectCollider,virtual public ImageRenderer
{
	const float movingSpeed = 500.0f;
	int direction;
	bool isExploded = false;
	std::vector<Image*> explodeImgVec;
	int exImgIndex = 0;
	Image* curImg = nullptr;
	const float animLimit = 0.1f;
	float animCounter = 0.0f;
	bool isPositionSet = false;
	void SetExplosion();
	bool isOutOfBoundary();
public:
	Missile(int direction, int x, int y, bool isPlayers);
	void Init(int direction, int x, int y,bool isPlayers);
	Missile();
	void OnCollision(int destTag, int xpos, int ypos, Collider* destCol) override;
	void Start() override;
	void Update() override;
	void LateUpdate() override;
	void SendMsg(int code) override;
	void Render(HDC hdc) override;
};
