#include "TankGameScene.h"
#include "ProgramCore.h"
#include <algorithm>
#include "EnumDirection.h"
#include <fstream>

using namespace std;

int TankGameScene::leftBound = 100;
int TankGameScene::rightBound = 1000;
int TankGameScene::topBound = 100;
int TankGameScene::bottomBound = 1000;

TankGameScene::TankGameScene()
{
	RECT winRect = getWinRect();
	mainCam = new Camera(0, 0, winRect.right, winRect.bottom);
	
}


TankGameScene::~TankGameScene()
{
}

void TankGameScene::Init()
{
	LoadImages();
	SetResourceMap();
	GenerateObjects();
	ProgramCore::instance.SetUpdateIntersec(0);
}

void TankGameScene::LoadImages()
{
	imageMap[NORMAL_BLOCK_FULL] = new Image(L"res/block00.bmp");
	imageMap[STEEL_TILE] = new Image(L"res/block05.bmp");
	imageMap[FOREST_TILE] = new Image(L"res/block06.bmp");
	imageMap[SEA_TILE] = new Image(L"res/block07.bmp");
	imageMap[ADVANCED_BLOCK_FULL] = new Image(L"res/block08.bmp");
	imageMap[EAGLE_NORMAL] = new Image(L"res/block13.bmp");
	imageMap[EAGLE_DESTROYED] = new Image(L"res/block14.bmp");
	imageMap[E_TANK_DOWN_0] = new Image(L"res/e_down_00.bmp");
	imageMap[E_TANK_DOWN_1] = new Image(L"res/e_down_01.bmp");
	imageMap[E_TANK_DOWN_RED] = new Image(L"res/e_down_02.bmp");
	imageMap[E_TANK_LEFT_0] = new Image(L"res/e_left_00.bmp");
	imageMap[E_TANK_LEFT_1] = new Image(L"res/e_left_01.bmp");
	imageMap[E_TANK_LEFT_RED] = new Image(L"res/e_left_02.bmp");
	imageMap[E_TANK_RIGHT_0] = new Image(L"res/e_right_00.bmp");
	imageMap[E_TANK_RIGHT_1] = new Image(L"res/e_right_01.bmp");
	imageMap[E_TANK_RIGHT_2] = new Image(L"res/e_right_02.bmp");
	imageMap[E_TANK_UP_0] = new Image(L"res/e_up_00.bmp");
	imageMap[E_TANK_UP_1] = new Image(L"res/e_up_01.bmp");
	imageMap[E_TANK_UP_RED] = new Image(L"res/e_up_02.bmp");
	imageMap[EXPLOSION_0] = new Image(L"res/explosion_00.bmp");
	imageMap[EXPLOSION_1] = new Image(L"res/explosion_01.bmp");
	imageMap[EXPLOSION_2] = new Image(L"res/explosion_02.bmp");
	imageMap[EXPLOSION_3] = new Image(L"res/explosion_03.bmp");
	imageMap[EXPLOSION_4] = new Image(L"res/explosion_04.bmp");
	imageMap[MISSILE_RIGHT] = new Image(L"res/missile_02.bmp");
	imageMap[MISSILE_LEFT] = new Image(L"res/missile_00.bmp");
	imageMap[MISSILE_UP] = new Image(L"res/missile_01.bmp");
	imageMap[MISSILE_DOWN] = new Image(L"res/missile_03.bmp");
	imageMap[SHIELD_0] = new Image(L"res/shield_00.bmp");
	imageMap[SHIELD_1] = new Image(L"res/shield_01.bmp");
	imageMap[P_TANK_DOWN_0] = new Image(L"res/tank_down_00.bmp");
	imageMap[P_TANK_DOWN_1] = new Image(L"res/tank_down_01.bmp");
	imageMap[P_TANK_LEFT_0] = new Image(L"res/tank_left_00.bmp");
	imageMap[P_TANK_LEFT_1] = new Image(L"res/tank_left_01.bmp");
	imageMap[P_TANK_RIGHT_0] = new Image(L"res/tank_right_00.bmp");
	imageMap[P_TANK_RIGHT_1] = new Image(L"res/tank_right_01.bmp");
	imageMap[P_TANK_UP_0] = new Image(L"res/tank_up_00.bmp");
	imageMap[P_TANK_UP_1] = new Image(L"res/tank_up_01.bmp");
	imageMap[ICON_E_TANK] = new Image(L"res/enemy_icon.bmp");
	imageMap[ICON_P_TANK] = new Image(L"res/player_icon.bmp");
	imageMap[ICON_STAGE] = new Image(L"res/stage_icon.bmp");


	blockWidth = imageMap[NORMAL_BLOCK_FULL]->bmWidth;
	blockHeight = imageMap[NORMAL_BLOCK_FULL]->bmHeight;
}


void TankGameScene::SetResourceMap()
{
	rightBound = leftBound + 13 * blockWidth;
	bottomBound = topBound + 13 * blockHeight;
	gameRect.left = leftBound;
	gameRect.right = rightBound;
	gameRect.top = topBound;
	gameRect.bottom = bottomBound;
	resourceMap["imgmap"] = &imageMap;
}

void TankGameScene::GenerateObjects()
{
	LoadStage(1);
	//GenerateBlock(0, 0, 0);
	//GenerateBlock(0, 0, 40);
	//GenerateBlock(0, 0, 80);
	//GenerateBlock(SEA, 0, 120);
	////GenerateBlock(ADVANCED_FULL, 0, 160);
	//GenerateBlock(FOREST, 0, 200);
	//GeneratePMissile(D_UP, 5, 200);
	//GeneratePlayerTank(100, 100);
}

void TankGameScene::LoadStage(int num)
{
	string loadfile = "stage/"+to_string(num) + ".txt";

	ifstream fis(loadfile, ios_base::in);

	if (!fis.good())
	{
		return;
	}

	for (int row = 0; row < 13; row++)
	{
		blockMap.push_back(vector<Block*>());
		for (int col = 0; col < 13; col++)
		{
			int read;
			fis >> read;
			Block* cur = GenerateBlock(read, leftBound + col * blockWidth, topBound + row * blockHeight);
			blockMap[row].push_back(cur);
		}
	}

	
	fis.close();
	GeneratePlayerTank(leftBound + 4 * blockWidth, topBound + 12 * blockHeight);
	GeneratePMissile();
	
}

void TankGameScene::Release()
{
	for_each(activeObjList.begin(), activeObjList.end(), [](GameObject* obj) {
		delete obj;
	});

	for_each(deactiveObjList.begin(), deactiveObjList.end(), [](GameObject* obj) {
		delete obj;
	});

	for (auto iter = imageMap.begin(); iter != imageMap.end(); iter++)
	{
		delete iter->second;
	}	
}

void TankGameScene::DrawScreen(HDC hdc)
{
	FillRect(hdc, &getWinRect(), (HBRUSH)GetStockObject(GRAY_BRUSH));
	FillRect(hdc, &gameRect, (HBRUSH)GetStockObject(WHITE_BRUSH));
	//BLACK_BRUSH
	//black_brush
	for (auto iter = renderMap.begin(); iter != renderMap.end(); iter++)
	{
		iter->second->Render(hdc);
	}
#ifdef _DEBUG_MODE_
	for (auto iter = colList.begin(); iter != colList.end(); iter++)
	{
		if ((*iter)->isActive)
		{
			(*iter)->DrawCollider(hdc);
		}
	}
	for (auto iter = floorCol.begin(); iter != floorCol.end(); iter++)
	{
		if ((*iter)->isActive)
		{
			(*iter)->DrawCollider(hdc);
		}
	}
#endif
}

Block* TankGameScene::GenerateBlock(int blockImg,int xpos,int ypos)
{
	int width = imageMap[NORMAL_BLOCK_FULL]->bmWidth;
	int height = imageMap[NORMAL_BLOCK_FULL]->bmHeight;
	Block* b = new Block(blockImg, xpos, ypos, width, height);
	b->Start();
	renderMap.insert(make_pair(b->zIndex, b));
	deactiveObjList.push_back(b);
	if (b->Collider::isActive)
	{
		floorCol.push_back(b);
	}
	return b;
}

Missile* TankGameScene::GeneratePMissile()
{
	Missile* mi = new Missile();
	resourceMap["pmissile"] = mi;
	mi->Start();
	activeObjList.push_back(mi);
	renderMap.insert(make_pair(mi->zIndex, mi));
	colList.push_back(mi);
	return mi;
}

Missile* TankGameScene::GenerateEMissile(int direction, int xpos, int ypos)
{
	Missile* mi = new Missile(direction, xpos, ypos,false);
	mi->Start();
	activeObjList.push_back(mi);
	renderMap.insert(make_pair(mi->zIndex, mi));
	colList.push_back(mi);
	return mi;
}

PlayerTank* TankGameScene::GeneratePlayerTank(int xpos, int ypos)
{
	PlayerTank* ptank = new PlayerTank(xpos, ypos);
	ptank->Start();
	activeObjList.push_back(ptank);
	renderMap.insert(make_pair(ptank->zIndex, ptank));
	colList.push_back(ptank);
	return ptank;
}

void TankGameScene::Update()
{
	for (auto iter = activeObjList.begin(); iter != activeObjList.end(); iter++)
	{
		(*iter)->Update();
	}
	DetectCollision();

	for (auto iter = activeObjList.begin(); iter != activeObjList.end();iter++)
	{
		(*iter)->LateUpdate();
	}
	Invalidate();
}

void TankGameScene::DetectCollision()
{
	for (auto iter = colList.begin(); iter != colList.end(); iter++)
	{
		for (auto fiter = floorCol.begin(); fiter != floorCol.end(); fiter++)
		{
			(*iter)->ChkCollision(**fiter);
		}
	}


	for (auto iter = floorCol.begin(); iter != floorCol.end();)
	{
		if ((*iter)->isActive == false)
		{
			iter = floorCol.erase(iter);
		}
		else
		{
			iter++;
		}
	}


}

