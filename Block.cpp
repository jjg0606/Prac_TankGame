#include "Block.h"
#include "EnumImg.h"
#include "EnumRenderOrder.h"
#include "Missile.h"
#include "EnumColMsg.h"
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

void Block::OnCollision(Collider* destCol)
{
	if (cntActiveBlock == 0)
	{
		return;
	}
	switch (destCol->Tag)
	{
	case COL_TAG_P_TANK:
		HandleTankCollision(destCol);
		break;
	case COL_TAG_P_MISSILE:
		// falling through
	case COL_TAG_E_MISSILE:
		HandleMissileCollision(destCol);
		break;
	}


}

void Block::HandleTankCollision(Collider* col)
{
	int dxmin;
	int dxmax;
	int dymin;
	int dymax;
	col->getMinMax(dxmin, dxmax, dymin, dymax);
	dxmin -= vertexInfo[0].x;
	dxmax -= vertexInfo[0].x;
	dymin -= vertexInfo[0].y;
	dymax -= vertexInfo[0].y;
	int dx = (vertexInfo[2].x - vertexInfo[0].x) / 4;
	int dy = (vertexInfo[2].y - vertexInfo[0].y) / 4;

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			if (innerState[r][c] == 0 || dx * (c + 1) < dxmin || dx * c > dxmax || dy * (r + 1) < dymin || dy * (r) > dymax)
			{
				continue;
			}
			col->SendMsg(COL_MSG_TANK_GO_BACK);
			return;
		}
	}
}

void Block::HandleMissileCollision(Collider* col)
{
	if (this->type == BlockType::EAGLE)
	{
		col->SendMsg(COL_MSG_MISSILE_EXPLODE);
		// game over method
	}
	else if (this->type == BlockType::ADVANCED)
	{
		col->SendMsg(COL_MSG_MISSILE_EXPLODE);
	}
	else if (this->type == BlockType::NORMAL)
	{
		HandleNormalBlockCollision(col);
	}

	if (cntActiveBlock <= 0)
	{
		Collider::isActive = false;
	}
}

void Block::HandleNormalBlockCollision(Collider* col)
{

	Vector2D<int> colVec = col->getDelta();
	int ColDirection = D_NONE;
	if (colVec.y == 0)
	{
		if (colVec.x > 0)
		{
			ColDirection = D_RIGHT;
		}
		else
		{
			ColDirection = D_LEFT;
		}
	}
	else if (colVec.x == 0)
	{
		if (colVec.y < 0)
		{
			ColDirection = D_UP;
		}
		else
		{
			ColDirection = D_DOWN;
		}
	}

	int dxmin,dxmax,dymin,dymax;
	col->getMinMax(dxmin, dxmax, dymin, dymax);

	dxmin -= vertexInfo[0].x;
	dxmax -= vertexInfo[0].x;
	dymin -= vertexInfo[0].y;
	dymax -= vertexInfo[0].y;

	int dx = (vertexInfo[2].x - vertexInfo[0].x) / 4;
	int dy = (vertexInfo[2].y - vertexInfo[0].y) / 4;

	int rowStart = 0;
	int rowEnd = 0;
	int colStart = 0;
	int colEnd = 0;
	bool isRowFirst = true;

	switch (ColDirection)
	{
	case D_NONE:
		return;
	case D_UP:
		isRowFirst = true;
		rowStart = 3;
		rowEnd = dymin/dy;
		colStart = (dxmin / dx) - 1;
		colEnd = (dxmax / dx) + 1;
		break;
	case D_DOWN:
		isRowFirst = true;
		rowStart = 0;
		rowEnd = dymax / dy;
		colStart = (dxmin / dx) - 1;
		colEnd = (dxmax / dx) + 1;
		break;
	case D_RIGHT:
		isRowFirst = false;
		colStart = 0;
		colEnd = dxmax /dx;
		rowStart = (dymin / dy) -1 ;
		rowEnd = (dymax / dy) +1;
		break;
	case D_LEFT:
		isRowFirst = false;
		colStart = 3;
		colEnd = dxmin / dx;
		rowStart = (dymin / dy) - 1;
		rowEnd = (dymax / dy) + 1;
		break;
	}

	colStart = between(colStart, 0, 3);
	colEnd = between(colEnd, 0, 3);
	rowStart = between(rowStart, 0, 3);
	rowEnd = between(rowEnd, 0, 3);

	if (isRowFirst)
	{
		for (int r = rowStart;; r += rowStart < rowEnd ? 1 : -1)
		{
			if (((rowStart < rowEnd) && (r > rowEnd)) || ((rowStart >= rowEnd) && (r < rowEnd)))
			{
				break;
			}
			bool isBlocked = false;
			for (int c = colStart; c <= colEnd; c++)
			{
				if (innerState[r][c] == 1)
				{
					isBlocked = true;
					innerState[r][c] = 0;
					cntActiveBlock--;
				}
			}
			if (isBlocked)
			{
				col->SendMsg(COL_MSG_MISSILE_EXPLODE);
				break;
			}
		}
	}
	else
	{
		for (int c = colStart;; c += colStart < colEnd ? 1 : -1)
		{
			if (((colStart < colEnd) && (c > colEnd)) || ((colStart >= colEnd) && (c < colEnd)))
			{
				break;
			}
			bool isBlocked = false;
			for (int r = rowStart; r <= rowEnd; r++)
			{
				if (innerState[r][c] == 1)
				{
					isBlocked = true;
					innerState[r][c] = 0;
					cntActiveBlock--;
				}
			}
			if (isBlocked)
			{
				col->SendMsg(COL_MSG_MISSILE_EXPLODE);
				break;
			}
		}
	}


}

int Block::between(int origin, int min, int max)
{
	if (origin < min)
	{
		return min;
	}
	if (origin > max)
	{
		return max;
	}
	return origin;
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