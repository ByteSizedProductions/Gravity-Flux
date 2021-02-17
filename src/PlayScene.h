#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Marvin.h"
#include "Platform.h"
#include "bomb.h"
#include "BombPickup.h"
#include "Button.h"
#include "Label.h"
#include "CollisionManager.h"
#include "Door.h"
#include "UserInterface.h"

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

	void updateTimer();
private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	std::vector<Platform*> m_platforms;

	std::vector<Bomb*> m_pBombs;
	BombPickup* m_pBombPickup;
	
	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Marvin* m_pMarvin;
	bool m_playerFacingRight;
	Door* m_pDoor;


	// UI Items
	Button* m_pStartButton;
	Button* m_pSettingsButton;
	Button* m_pExitButton;
	Label* m_pInstructionsLabel;
	UserInterface* m_UI;
	int m_timer;

	bool m_paused = 0;

	SDL_Rect* m_pauseMenu;
};

#endif /* defined (__PLAY_SCENE__) */