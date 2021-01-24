#pragma once
#ifndef __PLAY_SCENE__
#define __PLAY_SCENE__

#include "Scene.h"
#include "Plane.h"
#include "Player.h"
#include "Marvin.h"
#include "Button.h"
#include "Label.h"

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
private:
	// IMGUI Function
	void GUI_Function() const;
	std::string m_guiTitle;
	
	glm::vec2 m_mousePosition;

	SDL_Rect m_platforms[5] = { {350, 250, 100, 20}, // 0
								{150, 400, 100, 20}, // 1
								{550, 400, 100, 20}, // 2
								{-100, 0, 1000, 50}, // 3 // Ceiling
								{-100, 550, 1000, 100} };  // 4 Ground

	Plane* m_pPlaneSprite;
	Player* m_pPlayer;
	Marvin* m_pMarvin;
	bool m_playerFacingRight;

	// UI Items
	Button* m_pBackButton;
	Button* m_pNextButton;
	Label* m_pInstructionsLabel;
};

#endif /* defined (__PLAY_SCENE__) */