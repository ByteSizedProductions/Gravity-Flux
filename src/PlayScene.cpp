#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

PlayScene::PlayScene()
{
	PlayScene::start();
}

PlayScene::~PlayScene()
= default;

void PlayScene::draw()
{
	if(EventManager::Instance().isIMGUIActive())
	{
		GUI_Function();
	}




	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 192, 64, 0, 255);
	drawDisplayList();
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), 255, 255, 255, 255);

}

void PlayScene::update()
{
	if (m_paused)
	{
		return;
	}

	updateDisplayList();
	updateCollisions();
	checkBombs();
}

void PlayScene::updateCollisions()
{
	for (auto& platform : m_platforms) {
		//did collision between player and platform occur?
		if (CollisionManager::AABBCheck(m_pMarvin, platform) || (platform->getRigidBody()->isColliding)) {
			m_pMarvin->handleCollisions(platform);
		}
		//did collision between bomb and platforms occur?
		for (auto& bomb : m_pBombs) {
			if (CollisionManager::AABBCheck(bomb, platform) || (platform->getRigidBody()->isColliding)) {
				bomb->handleCollisions(platform);
			}
		}
	}
	if (CollisionManager::AABBCheck(m_pMarvin, m_pDoor))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
	
	for (auto& bomb : m_pBombs)
	{
		// Check collision between bombs and player
		if (CollisionManager::circleAABBCheck(bomb, m_pMarvin) && (bomb->checkAnimationFrame() > 10 && bomb->checkAnimationFrame() < 21)) // Bomb is active for 9 frames
		{
			// TODO: subtract bomb damage(currently 1) from Marvin's health, can't do it since Marvin doesn't have health yet
			m_pMarvin->setEnabled(false);
		}
	}

	// check collision with BombPickup and player
	if (m_pBombPickup->isEnabled())
	{
		if (CollisionManager::AABBCheck(m_pMarvin, m_pBombPickup))
		{
			m_pBombPickup->setEnabled(false);
			m_pMarvin->setNumBombs(m_pMarvin->getNumBombs() + 1);
			m_pBombCount->setText("Bombs: " + std::to_string(m_pMarvin->getNumBombs())); // This can go in an 'updateHud() or something similar if we make one'
		}
	}
}

void PlayScene::checkBombs()
{
	for (auto i = 0; i < m_pBombs.size(); i++)
	{
		if (m_pBombs[i]->checkAnimationDone())
		{
			removeChild(m_pBombs[i]);
			m_pBombs[i] = nullptr;
			m_pBombs.erase(m_pBombs.begin() + i);
			m_pBombs.shrink_to_fit();
			std::cout << "Bomb Deleted.";
		}
	}
}

void PlayScene::clean()
{
	removeAllChildren();
	m_pBombs.clear();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

	// handle player movement with GameController
	if (SDL_NumJoysticks() > 0)
	{
		if (EventManager::Instance().getGameController(0) != nullptr)
		{
			const auto deadZone = 10000;
			if (EventManager::Instance().getGameController(0)->LEFT_STICK_X > deadZone)
			{
				m_playerFacingRight = true;
			}
			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_X < -deadZone)
			{
				m_playerFacingRight = false;
			}

			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_Y > deadZone)
			{
				//m_pPlayer->setAnimationState(PLAYER_RUN_UP);
				
			}

			else if (EventManager::Instance().getGameController(0)->LEFT_STICK_Y < -deadZone)
			{
				//m_pPlayer->setAnimationState(PLAYER_RUN_DOWN);
				
			}

			else
			{
				if (m_playerFacingRight)
				{
					//m_pPlayer->setAnimationState(PLAYER_IDLE_RIGHT);
				}
				else
				{
					//m_pPlayer->setAnimationState(PLAYER_IDLE_LEFT);
				}
			}
		}
	}


	// handle player movement if no Game Controllers found
	if (SDL_NumJoysticks() < 1)
	{
		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A)) {
			m_pMarvin->setIsMoving(true);
			m_pMarvin->setCurrentDirection(glm::vec2(-1.0f, m_pMarvin->getCurrentDirection().y));
		}
		else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D)) {
			m_pMarvin->setIsMoving(true);
			m_pMarvin->setCurrentDirection(glm::vec2(1.0f, m_pMarvin->getCurrentDirection().y));
		}
		else
			m_pMarvin->setIsMoving(false);

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W))
			m_pMarvin->jump();

		if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE) && m_pMarvin->getGravityCooldown() == 0 && m_pMarvin->isGrounded())
		{
			if (m_pMarvin->isGravityFlipped())
			{
				m_pMarvin->setGravity(12.0f);
				m_pMarvin->setJumpForce(-20.0f);
				m_pMarvin->setAngle(0.0f);
				m_pMarvin->setGravityFlipped(false);
				m_pMarvin->setCurrentDirection(glm::vec2(m_pMarvin->getCurrentDirection().x, 1.0f));
			}
			else{
				m_pMarvin->setGravity(-12.0f);
				m_pMarvin->setJumpForce(20.0f);
				m_pMarvin->setAngle(180.0f);
				m_pMarvin->setGravityFlipped(true);
				m_pMarvin->setCurrentDirection(glm::vec2(m_pMarvin->getCurrentDirection().x, -1.0f));
			}
			m_pMarvin->setGravityCooldown(15);
			m_pMarvin->setIsGrounded(false);
		}
	}

	if (EventManager::Instance().keyPressed(SDL_SCANCODE_E) && m_pMarvin->getNumBombs() > 0 /*&& m_pMarvin->getBombCooldown() == 0*/)
	{
		m_pBombs.push_back(new Bomb(m_pMarvin->getTransform()->position, m_pMarvin->getCurrentDirection()));
		addChild(m_pBombs.back());
		m_pBombs.shrink_to_fit();
		if (m_pMarvin->getNumBombs() != 0)
			m_pMarvin->setNumBombs(m_pMarvin->getNumBombs() - 1);
		m_pMarvin->setBombCooldown(60); // Sets bomb cooldown to 1 seconds
		m_pBombCount->setText("Bombs: " + std::to_string(m_pMarvin->getNumBombs())); // This can go in an 'updateHud() or something similar if we make one'
	}
	
	if (EventManager::Instance().keyPressed(SDL_SCANCODE_ESCAPE))
	{
		//TheGame::Instance()->quit();
		std::cout << "Paused" << std::endl;
		if (m_paused == true)
		{
			std::cout << " NOT Paused" << std::endl;
			m_paused = false;
		}
		else
		{
			std::cout << "Paused" << std::endl;

			m_paused = true;
		}
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_1))
	{
		TheGame::Instance()->changeSceneState(START_SCENE);
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
}

void PlayScene::start()
{
	// Set GUI Title
	m_guiTitle = "Play Scene";

		
		m_pauseMenu = new SDL_Rect();
		m_pauseMenu->x = 400.0f;
		m_pauseMenu->y = 300.0f;
		m_pauseMenu->w = 100.0f;
		m_pauseMenu->x = 50.0f;
		std::cout << "POP" << std::endl;
		SDL_RenderFillRect(Renderer::Instance()->getRenderer(), m_pauseMenu);

	std::cout << "LOL" << std::endl;
	//Platforms
	m_platforms.push_back(new Platform(glm::vec2(350.0f, 200.0f), 100, 20));
	m_platforms.push_back(new Platform(glm::vec2(150.0f, 350.0f), 100, 20));
	m_platforms.push_back(new Platform(glm::vec2(550.0f, 350.0f), 100, 20));
	m_platforms.push_back(new Platform(glm::vec2(-100.0f, 0.0f), 1000, 50)); // Ceiling
	m_platforms.push_back(new Platform(glm::vec2(-100.0f, 550.0f), 1000, 50)); // Floor
	for (auto& count : m_platforms)
		addChild(count);
	
	//Marvin
	m_pMarvin = new Marvin();
	addChild(m_pMarvin);
	// Set Marvin's bombs to 10
	m_pMarvin->setNumBombs(100);

	// Player Sprite
	m_pPlayer = new Player();
	//addChild(m_pPlayer);
	m_playerFacingRight = true;

	// Bomb Pickup
	m_pBombPickup = new BombPickup();
	addChild(m_pBombPickup);
	m_pBombPickup->getTransform()->position = glm::vec2(165.0f, 270.0f);

	

	

	//addChild(m_pNextButton);

	/* Instructions Label */
	m_pInstructionsLabel = new Label("Press the backtick (`) character to toggle Debug View", "Consolas");
	m_pInstructionsLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.5f, 500.0f);

	//addChild(m_pInstructionsLabel);
	m_pDoor = new Door();
	m_pDoor->getTransform()->position = glm::vec2(0.0f, 50.0f);
	addChild(m_pDoor);

    // Bomb Count Label
	m_pBombCount = new Label("Bombs: " + std::to_string(m_pMarvin->getNumBombs()), "Consolas");
	m_pBombCount->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.1f, 100.0f);
	addChild(m_pBombCount);
}

void PlayScene::GUI_Function() const
{
	// Always open with a NewFrame
	ImGui::NewFrame();

	// See examples by uncommenting the following - also look at imgui_demo.cpp in the IMGUI filter
	//ImGui::ShowDemoWindow();
	
	ImGui::Begin("Your Window Title Goes Here", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_MenuBar);

	if(ImGui::Button("My Button"))
	{
		std::cout << "My Button Pressed" << std::endl;
	}

	ImGui::Separator();

	static float float3[3] = { 0.0f, 1.0f, 1.5f };
	if(ImGui::SliderFloat3("My Slider", float3, 0.0f, 2.0f))
	{
		std::cout << float3[0] << std::endl;
		std::cout << float3[1] << std::endl;
		std::cout << float3[2] << std::endl;
		std::cout << "---------------------------\n";
	}

	ImGui::SameLine(350);
	

	
	ImGui::End();

	// Don't Remove this
	ImGui::Render();
	ImGuiSDL::Render(ImGui::GetDrawData());
	ImGui::StyleColorsDark();
}
