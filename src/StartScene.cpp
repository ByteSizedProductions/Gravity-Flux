#include "StartScene.h"
#include <algorithm>
#include "Game.h"
#include "glm/gtx/string_cast.hpp"
#include "EventManager.h"

StartScene::StartScene()
{
	StartScene::start();
}

StartScene::~StartScene()
= default;

void StartScene::draw()
{
	drawDisplayList();
}

void StartScene::update()
{
	updateDisplayList();
}

void StartScene::clean()
{
	removeAllChildren();
}

void StartScene::handleEvents()
{
	EventManager::Instance().update();

	// Keyboard Events
	if(EventManager::Instance().isKeyDown(SDL_SCANCODE_ESCAPE))
	{
		TheGame::Instance()->quit();
	}
}

void StartScene::start()
{
	m_pBackground = new StaticSprite("../Assets/textures/SpaceBackground.png", "SpaceBackground");
	addChild(m_pBackground);

	m_pMarvinField = new MarvinField();
	for (auto m : m_pMarvinField->GetFlyingMarvins())
	{
		addChild(m);
	}

	m_pLogo = new StaticSprite("../Assets/textures/gfluxlogo.png", "logo");
	m_pLogo->getTransform()->position = glm::vec2(100.0f, 20.0f);
	addChild(m_pLogo);

	// Start Button
	m_pStartButton = new Button("../Assets/textures/playbutton.png", "PlayButton", START_BUTTON);
	m_pStartButton->getTransform()->position = glm::vec2(275.0f, 400.0f); 

	m_pStartButton->addEventListener(CLICK, [&]()-> void
	{
		m_pStartButton->setActive(false);
		TheGame::Instance()->changeSceneState(PLAY_SCENE);
	});
	
	m_pStartButton->addEventListener(MOUSE_OVER, [&]()->void
	{
		m_pStartButton->setAlpha(128);
	});

	m_pStartButton->addEventListener(MOUSE_OUT, [&]()->void
	{
		m_pStartButton->setAlpha(255);
	});
	addChild(m_pStartButton);

	//Setting button
	m_pSettingsButton = new Button("../Assets/textures/settingsbutton.png", "SettingsButton", SETTINGS_BUTTON);
	m_pSettingsButton->getTransform()->position = glm::vec2(525.0f, 400.0f);

	m_pSettingsButton->addEventListener(CLICK, [&]()-> void
		{

		});

	m_pSettingsButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pSettingsButton->setAlpha(128);
		});

	m_pSettingsButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pSettingsButton->setAlpha(255);
		});
	addChild(m_pSettingsButton);


	//Exit button
	m_pExitButton = new Button("../Assets/textures/quitbutton.png", "ExitButton", EXIT_BUTTON);
	m_pExitButton->getTransform()->position = glm::vec2(400.0f, 500.0f);

	m_pExitButton->addEventListener(CLICK, [&]()-> void
		{
			m_pExitButton->setActive(false);
			//just jumps to the end scene
			TheGame::Instance()->quit();
		});

	m_pExitButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pExitButton->setAlpha(128);
		});

	m_pExitButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pExitButton->setAlpha(255);
		});
	addChild(m_pExitButton);

	
}
