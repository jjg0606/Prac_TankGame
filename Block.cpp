#include "Block.h"
#include "EnumImg.h"
#include "EnumRenderOrder.h"
using namespace std;

Block::Block(int BlockState, int x, int y, int width, int height)
	: RectCollider(x,y,width,height,COL_TAG_BLOCK)
{
	SetPosition(x, y);
	SetTransparent(RGB(255, 0, 255));
	
	switch (BlockState)
	{
	case NORMAL_FULL:
		type = BlockType::NORMAL;
		SetInnerState(D_FULL);
		break;
	case NORMAL_UP:
		type = BlockType::NORMAL;
		SetInnerState(D_UP);
		break;
	case NORMAL_LEFT:
		type = BlockType::NORMAL;
		SetInnerState(D_LEFT);
		break;
	case NORMAL_DOWN:
		type = BlockType::NORMAL;
		SetInnerState(D_DOWN);
		break;
	case NORMAL_RIGHT:
		type = BlockType::NORMAL;
		SetInnerState(D_RIGHT);
		break;
	case SEA:
		type = BlockType::SEA;
		SetInnerState(D_NONE);
		break;
	case FOREST:
		type = BlockType::FOREST;
		SetInnerState(D_NONE);
		break;
	case STEEL:
		type = BlockType::STEEL;
		SetInnerState(D_NONE);
		break;
	case ADVANCED_FULL:
		type = BlockType::ADVANCED;
		SetInnerState(D_FULL);
		break;
	case ADVANCED_UP:
		type = BlockType::ADVANCED;
		SetInnerState(D_UP);
		break;
	case ADVANCED_LEFT:
		type = BlockType::ADVANCED;
		SetInnerState(D_LEFT);
		break;
	case ADVANCED_DOWN:
		type = BlockType::ADVANCED;
		SetInnerState(D_DOWN);
		break;
	case ADVANCED_RIGHT:
		type = BlockType::ADVANCED;
		SetInnerState(D_RIGHT);
		break;
	case EAGLE:
		type = BlockType::EAGLE;
		SetInnerState(D_FULL);
		break;
	case ENUM_END:
		type = BlockType::NONE;
		SetInnerState(D_NONE);
		break;
	default:
		break;
	}
}

void Block::Start()
{
	GameObject::isActive = false;
	unordered_map<int,Image*>& imgMap = *requireResource<unordered_map<int, Image*>>("imgmap");
	switch (type)
	{
	case BlockType::NONE:
		SetImg(nullptr);
		Renderer::zIndex = ORDER_FLOOR;
		break;
	case BlockType::NORMAL:
		SetImg(imgMap[NORMAL_BLOCK_FULL]);
		Renderer::zIndex = ORDER_TANK;
		break;
	case BlockType::SEA:
		SetImg(imgMap[SEA_TILE]);
		Renderer::zIndex = ORDER_FLOOR;
		break;
	case BlockType::FOREST:
		SetImg(imgMap[FOREST_TILE]);
		Renderer::zIndex = ORDER_OVERTANK;
		break;
	case BlockType::STEEL:
		SetImg(imgMap[STEEL_TILE]);
		Renderer::zIndex = ORDER_FLOOR;
		break;
	case BlockType::ADVANCED:
		SetImg(imgMap[ADVANCED_BLOCK_FULL]);
		Renderer::zIndex = ORDER_TANK;
		break;
	case BlockType::EAGLE:
		SetImg(imgMap[EAGLE_NORMAL]);
		Renderer::zIndex = ORDER_TANK;
		break;
	default:
		break;
	}

}

void Block::SetInnerState(int Direction)
{
	if (Direction == D_NONE)
	{
		Collider::isActive = false;
		return;
	}

	int rowStart = 0;
	int rowEnd = 3;
	int colStart = 0;
	int colEnd = 3;
	Collider::isActive = true;

	switch (Direction)
	{
	case D_FULL:
		break;
	case D_LEFT:
		colEnd = 1;
		break;
	case D_RIGHT:
		colStart = 2;
		break;
	case D_UP:
		rowEnd = 1;
		break;
	case D_DOWN:
		rowStart = 2;
		break;
	}
	   
	innerState.resize(4);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			innerState[i].push_back(0);
		}		
	}

	for (int r = 0; r < 4; r++)
	{
		if (r < rowStart || r > rowEnd)
		{
			continue;
		}
		for (int c = 0; c < 4; c++)
		{
			if (c<colStart || c>colEnd)
			{
				continue;
			}
			innerState[r][c] = 1;
		}
	}	
}

void Block::OnCollision(int destTag, int xpos, int ypos, Collider* destCol)
{
	xpos -= vertexInfo[0].x;
	ypos -= vertexInfo[0].y;
	int width = vertexInfo[2].x - vertexInfo[0].x;
	int heigth = vertexInfo[2].y - vertexInfo[0].y;
	xpos /= (width * 0.25);
	ypos /= (heigth * 0.25);

	if (xpos < 0 || xpos >= 4 || ypos < 0 || ypos >= 4)
	{
		return;
	}

	if (innerState[ypos][xpos] == 0)
	{
		return;
	}

	// TODO
	if (type == BlockType::EAGLE)
	{

	}
	else if (type == BlockType::ADVANCED)
	{

	}
	else if (type == BlockType::NORMAL)
	{
		innerState[ypos][xpos] = 0;
	}
}

void Block::Render(HDC hdc)
{
	if (type == BlockType::NONE)
	{
		return;
	}
	if (innerState.empty())
	{
		ImageRenderer::Render(hdc);
		return;
	}
	SelectObject(getMemDC(), img->handle);
	float dc = img->bmWidth / 4.0f;
	float dh = img->bmHeight / 4.0f;
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			if (innerState[r][c] == 0)
			{
				continue;
			}
			TransparentBlt(hdc, position.x + c * dc, position.y + r * dh, dc, dh, getMemDC(), c*dc, r*dh, dc, dh, transparent);			
		}
	}

}