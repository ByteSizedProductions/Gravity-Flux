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
#include "Brain.h"
#include "FireEnemy.h"

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
	void updateInsanity();
private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	std::map<char, Tile> m_tiles;
	std::vector<Tile*> m_pTiles;

	std::vector<Platform*> m_platforms;
	std::vector<Crate*> m_pCrates;

	std::vector<Bomb*> m_pBombs;
	BombPickup* m_pBombPickup;
	
	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Marvin* m_pMarvin;
	bool m_playerFacingRight;
	Door* m_pDoor;
	PauseMenu* m_pPauseMenu;
	Brain* m_pBrain;

	std::vector<FireEnemy*> m_pFireEnemies;

	// UI Items
	Label* m_pInstructionsLabel;
	UserInterface* m_UI;
	int m_timer;
	Health* m_marvinHealth;
	int cooldown = 10;
	//bool m_paused = 0;
	int m_insanity = 1;
	int m_event1Countdown = 0;

	SDL_Rect* m_pauseMenu;
};

#endif /* defined (__PLAY_SCENE__) */