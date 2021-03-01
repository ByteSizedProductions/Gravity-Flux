#include "PauseMenu.h"
#include "Game.h"
#include "SceneState.h"

PauseMenu::PauseMenu()
{
	TextureManager::Instance()->load("../Assets/textures/pausemenu.png", "pausemenu");
	auto size = TextureManager::Instance()->getTextureSize("pausemenu");	
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(250.0f, 100.0f);
	setType(PAUSE_MENU);
	getRigidBody()->isColliding = false;

	m_pResumeButton = new Button("../Assets/textures/PauseResumeButton.png", "ResumeButton", RESUME_BUTTON);
	m_pResumeButton->getTransform()->position = glm::vec2(400.0f, 220.0f);

	m_pResumeButton->addEventListener(CLICK, [&]()-> void
		{
			setPaused(false);
		});

	m_pResumeButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pResumeButton->setAlpha(128);
		});

	m_pResumeButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pResumeButton->setAlpha(255);
		});
	

	m_pSettingsButton = new Button("../Assets/textures/PauseSettingsButton.png", "SettingsButton", SETTINGS_BUTTON);
	m_pSettingsButton->getTransform()->position = glm::vec2(400.0f, 307.0f);

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
	

	m_pExitButton = new Button("../Assets/textures/PauseExitButton.png", "ExitButton", EXIT_BUTTON);
	m_pExitButton->getTransform()->position = glm::vec2(400.0f, 394.0f);

	m_pExitButton->addEventListener(CLICK, [&]()-> void
		{
			setPaused(false);
			TheGame::Instance()->changeSceneState(START_SCENE);
		});

	m_pExitButton->addEventListener(MOUSE_OVER, [&]()->void
		{
			m_pExitButton->setAlpha(128);
		});

	m_pExitButton->addEventListener(MOUSE_OUT, [&]()->void
		{
			m_pExitButton->setAlpha(255);
		});
	
}

PauseMenu::~PauseMenu()
{
}


void PauseMenu::draw()
{
	if (m_paused)
	{
		TextureManager::Instance()->draw("pausemenu", getTransform()->position.x, getTransform()->position.y);
		m_pResumeButton->draw();
		m_pSettingsButton->draw();
		m_pExitButton->draw();
	}
}
void PauseMenu::update()
{
	if(m_paused)
	{		
		m_pResumeButton->update();
		m_pSettingsButton->update();
		m_pExitButton->update();
	}
	
}

void PauseMenu::clean()
{
}

void PauseMenu::setPaused(const bool paused)
{
	m_paused = paused;
}

bool PauseMenu::getPaused() const
{
	return m_paused;
}

