#include "LoadingScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

LoadingScene::LoadingScene()
{
	LoadingScene::start();
}

LoadingScene::~LoadingScene()
= default;

void LoadingScene::draw()
{
	drawDisplayList();
}

void LoadingScene::update()
{
	updateDisplayList();
}

void LoadingScene::clean()
{
	removeAllChildren();
}

void LoadingScene::handleEvents()
{
	EventManager::Instance().update();

	// Button Events
	m_pRestartButton->update();

	// Keyboard Events
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
}

void LoadingScene::start()
{
	m_pBackground = new StaticSprite("../Assets/textures/SpaceBackground.png", "SpaceBackground");
	addChild(m_pBackground);
	
	const SDL_Color blue = { 0, 0, 255, 255 };
	m_label = new Label("Loading next level", "Consolas", 80, blue, glm::vec2(400.0f, 40.0f));
	m_label2 = new Label("Click button to start next level", "Consolas", 35, blue, glm::vec2(400.0f, 200.0f));
	m_label->setParent(this);
	m_label2->setParent(this);
	addChild(m_label);
	addChild(m_label2);

	// Restart Button
	m_pRestartButton = new Button("../Assets/textures/nextButton.png", "nextButton", NEXT_BUTTON);
	m_pRestartButton->getTransform()->position = glm::vec2(400.0f, 350.0f);
	m_pRestartButton->addEventListener(CLICK, [&]()-> void
		{
			m_pRestartButton->setActive(false);
			TheGame::Instance()->changeSceneState(PLAY_SCENE);
		});

	m_pRestartButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pRestartButton->setAlpha(128);
		});

	m_pRestartButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pRestartButton->setAlpha(255);
		});

	addChild(m_pRestartButton);
}
