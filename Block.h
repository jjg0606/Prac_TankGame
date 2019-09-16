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

class Block
	: virtual public RectCollider, virtual public ImageRenderer, virtual public GameObject
{
	BlockType type;
	std::vector<std::vector<int>> innerState;
	void SetInnerState(int Direction);
	int cntActiveBlock = 0;
	void HandleColl(int row,int col, int destTag, Collider* destCol);
public:
	Block(int BlockState,int x,int y,int width,int height);
	void OnCollision(int destTag, int xpos, int ypos, Collider* destCol) override;
	virtual void Render(HDC hdc) override;
	virtual void Start() override;
};