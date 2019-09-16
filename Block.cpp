#include "Block.h"
#include "EnumImg.h"
#include "EnumRenderOrder.h"
#include "Missile.h"
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
		SetInnerState(D_FULL);
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
			cntActiveBlock++;
		}
	}	
}

void Block::OnCollision(int destTag, int xpos, int ypos, Collider* destCol)
{
	if (cntActiveBlock == 0)
	{
		return;
	}

	int dxmin;
	int dxmax;
	int dymin;
	int dymax;
	destCol->getMinMax(dxmin, dxmax, dymin, dymax);
	dxmin -= vertexInfo[0].x;
	dxmax -= vertexInfo[0].x;
	dymin -= vertexInfo[0].y;
	dymax -= vertexInfo[0].y;
	int dx = (vertexInfo[2].x - vertexInfo[0].x) / 4;
	int dy = (vertexInfo[2].y - vertexInfo[0].y) / 4;

	for (int r = 0; r < 4 ; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			if (innerState[r][c] == 0 || dx*(c+1) <dxmin || dx*c > dxmax || dy*(r+1) < dymin || dy*(r) > dymax)
			{
				continue;
			}
			HandleColl(r, c, destTag,destCol);
		}
	}	
}

void Block::HandleColl(int row, int col, int destTag, Collider* destCol)
{
	// TODO
	if (destTag == COL_TAG_P_TANK || destTag == COL_TAG_E_TANK)
	{
		destCol->SendMsg(1); // go back
	}
	else if (destTag == COL_TAG_P_MISSILE || destTag == COL_TAG_E_MISSILE)
	{
		if (type == BlockType::EAGLE)
		{
			// game over
		}
		else if (type == BlockType::ADVANCED)
		{
			destCol->SendMsg(1);
		}
		else if (type == BlockType::NORMAL && (destTag == COL_TAG_E_MISSILE || destTag == COL_TAG_P_MISSILE))
		{
			innerState[row][col] = 0;
			cntActiveBlock--;
			if (cntActiveBlock == 0)
			{
				Collider::isActive = false;
			}
			destCol->SendMsg(1);
		}
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
	Camera* maincam = getMainCam();
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
			TransparentBlt(hdc, position.x + c * dc - maincam->left , position.y + r * dh - maincam->top, dc, dh, getMemDC(), c*dc, r*dh, dc, dh, transparent);			
		}
	}

}