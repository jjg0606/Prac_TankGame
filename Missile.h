#pragma once
#include "GameObject.h"
#include "ImageRenderer.h"
#include "RectCollider.h"
#include "EnumColliderTag.h"
#include <string>

class Missile
	: virtual public GameObject, virtual public RectCollider,virtual public ImageRenderer
{
	static const float movingSpeed;
	int direction;
public:
	Missile(int direction, int x, int y);
	void OnCollision(int destTag, int xpos, int ypos, Collider* destCol) override;
	void Start() override;
	void Update() override;
};
