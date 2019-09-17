#pragma once
#include "Scene.h"
#include "Image.h"
#include "EnumImg.h"
#include "Renderer.h"
#include <map>
#include <list>
#include "Block.h"
#include "Missile.h"
#include "PlayerTank.h"
#include "EnemyTank.h"

#define _DEBUG_MODE_

class TankGameScene :
	public virtual Scene
{
	
protected:
	//std::unordered_map<std::string, void*> resourceMap;
	//Camera* mainCam = nullptr;
	std::unordered_map<int, Image*> imageMap;
	std::multimap<int, Renderer*> renderMap;
	std::list<GameObject*> activeObjList;
	std::list<GameObject*> deactiveObjList;
	std::list<Collider*> floorCol;
	std::list<Collider*> colList;
	// called in init
	void LoadImages();
	void SetResourceMap();
	void GenerateObjects();

	// factory methods
	Block* GenerateBlock(int blockImg, int xpos, int ypos);
	Missile* GeneratePMissile();
	Missile* GenerateEMissile(int direction, int xpos, int ypos);
	PlayerTank* GeneratePlayerTank(int xpos, int ypos);
	EnemyTank* GenerateEnemyTank(int xpos, int ypos);
	// called in update
	void DetectCollision();
	//
	void LoadStage(int num);
	int blockWidth = 0;
	int blockHeight = 0;
	RECT gameRect;

	std::vector<std::vector<Block*>> blockMap;
public:
	TankGameScene();
	~TankGameScene();
	virtual void DrawScreen(HDC hdc) override;
	virtual void OnInput(WPARAM wParam) {}
	virtual void Update() override;
	
	virtual void Release() override;
	void Init() override;
	virtual void OnMouseClick(int x, int y, int E_MOUSE_BTN) {}
	virtual void OnMenuMessage(WPARAM wParam) {}
	
	static int leftBound;
	static int rightBound;
	static int topBound;
	static int bottomBound;	
};



