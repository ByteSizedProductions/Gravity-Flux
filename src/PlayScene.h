#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include <fstream>
#include "Health.h"
#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Marvin.h"
#include "Platform.h"
#include "Crate.h"
#include "bomb.h"
#include "BombPickup.h"
#include "Button.h"
#include "Label.h"
#include "CollisionManager.h"
#include "Door.h"
#include "UserInterface.h"
#include "Util.h"
#include "PauseMenu.h"
#include "Tile.h"
#include "PhysicsTile.h"
#include "FireEnemy.h"
#include "BackgroundImage.h"
#include "AbilityBar.h"
#include "GravityNullifier.h"
#include "Fireball.h"
#include "BossEnemy.h"
#include "DestructibleTile.h"
#include "time.h"

class PlayScene : public Scene
{
public:
	PlayScene();
	~PlayScene();

	// Scene LifeCycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	void updateCollisions();
	void checkBombs();
	void scrollObjects();
	void buildLevel();
	void updateTimer();
	void BossAttack();
	static int m_level;
private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	std::map<char, Tile> m_tiles;
	
	std::vector<Tile*> m_pTiles;
	std::vector<PhysicsTile*> m_pCrates;
	std::vector<PhysicsTile*> m_pBombCrates;
	std::vector<PhysicsTile*> m_pHealthCrates;
	std::vector<Bomb*> m_pBombs;
	std::vector<BombPickup*> m_pBombPickups;
	std::vector<Tile*> m_pFloorSpikes;
	std::vector<Tile*> m_pRoofSpikes;
	std::vector<DestructibleTile*> m_pDestructibleTile;
	
	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Marvin* m_pMarvin;
	BossEnemy* m_pBossEnemy;
	bool m_playerFacingRight;
	Door* m_pDoor;
	PauseMenu* m_pPauseMenu;
	BackgroundImage* m_pBackground;
	std::vector<FireEnemy*> m_pFireEnemies;
	std::vector<Fireball*> m_pHadesFlamingOrb;

	// UI Items
	Label* m_pInstructionsLabel;
	std::vector<Label*> m_pLabels;
	UserInterface* m_UI;
	AbilityBar* m_AbilityBar;
	int m_timer;
	int cooldown = 10;
	//bool m_paused = 0;
	int m_insanity = 1;
	int m_event1Countdown = 0;
	//int m_enemyThrowCooldown = 40;
	int m_footsteps = 0;
	int m_hadesFireTimer = 40;

	SDL_Rect* m_pauseMenu;

	void m_DetectedPlayer(DisplayObject* object);
};

#endif /* defined (__PLAY_SCENE__) */