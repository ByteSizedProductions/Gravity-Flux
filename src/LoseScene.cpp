#include "LoseScene.h"
#include "Game.h"
#include "TextureManager.h"
#include "EventManager.h"

LoseScene::LoseScene()
{
	LoseScene::start();
}

LoseScene::~LoseScene()
= default;

void LoseScene::draw()
{
	drawDisplayList();
	TextureManager::Instance()->draw("hand", 100.0f, 400.0f, 0, 255, true);
	TextureManager::Instance()->draw("yourdead", 400.0f, 100.0f, 0, 255, true);
}

void LoseScene::update()
{
	updateDisplayList();
}

void LoseScene::clean()
{
	removeAllChildren();
}

void LoseScene::handleEvents()
{
	EventManager::Instance().update();
	m_pRestartButton->update();
}

void LoseScene::start()
{
	TextureManager::Instance()->load("../Assets/textures/bloodyHand.png", "hand");
	TextureManager::Instance()->load("../Assets/textures/YourDeadLOGO.png", "yourdead");

	m_pBackground = new StaticSprite("../Assets/textures/SpaceBackground.png", "SpaceBackground");
	addChild(m_pBackground);

	m_pRestartButton = new Button("../Assets/textures/bloodyRestartButton.png", "restartButton", RESTART_BUTTON);
	m_pRestartButton->getTransform()->position = glm::vec2(400.0f, 300.0f);
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
