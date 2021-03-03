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
	if (m_pPauseMenu->getPaused())
	{
		m_pPauseMenu->update();
		return;
	}

	updateCollisions();
	scrollObjects();
	updateDisplayList();
	checkBombs();
	updateTimer();
}



void PlayScene::updateCollisions()
{

	for (auto& platform : m_platforms) {
		//did collision between player and platform occur?
		if (CollisionManager::AABBCheck(m_pMarvin, platform) ) {
			m_pMarvin->handleCollisions(platform);
		}
		//did collision between bomb and platforms occur?
		for (auto& bomb : m_pBombs) {
			if (CollisionManager::AABBCheck(bomb, platform) ) {
				bomb->handleCollisions(platform);
			}
		}
		// did collision between crate and platform occur?
		for (auto& crate : m_pCrates) {
			if (CollisionManager::AABBCheck(crate, platform) ) {
				crate->handleCollisions(platform);
			}
		}
	}
	if (CollisionManager::AABBCheck(m_pMarvin, m_pDoor))
	{
		TheGame::Instance()->changeSceneState(END_SCENE);
	}
	
	for (auto& bomb : m_pBombs)
	{
		for (int i = 0; i < m_pCrates.size(); i++) {
			// Check collision between bombs and player
			if (CollisionManager::AABBCheck(bomb, m_pCrates[i]))
			{
				bomb->handleCollisions(m_pCrates[i]);
			}

			//is crate near bomb when it explodes?
			if ((bomb->checkAnimationFrame() > 10 && bomb->checkAnimationFrame() < 13) &&
				Util::distance(bomb->getTransform()->position + glm::vec2(bomb->getWidth() / 2, bomb->getHeight() / 2),
					m_pCrates[i]->getTransform()->position + glm::vec2(m_pCrates[i]->getWidth() / 2, m_pCrates[i]->getHeight() / 2)) < 60) {
				// TODO: subtract bomb damage(currently 1) from Marvin's health, can't do it since Marvin doesn't have health yet
				removeChild(m_pCrates[i]);
				m_pCrates[i] = nullptr;
				m_pCrates.erase(m_pCrates.begin() + i);
				m_pCrates.shrink_to_fit();
				break;
			}
		}
	}

	for (auto& crate : m_pCrates)
	{
		//Did collision between player and crates occur
		if (CollisionManager::AABBCheck(m_pMarvin, crate) ) {
			m_pMarvin->handleCollisions(crate);
		}
		
		for (auto& otherCrate : m_pCrates) {
			if (crate != otherCrate && CollisionManager::AABBCheck(crate, otherCrate) ) {
				crate->handleCollisions(otherCrate);
			}
		}
	}

	// check collision with BombPickup and player
	if (m_pBombPickup->isEnabled())
	{
		if (CollisionManager::AABBCheck(m_pMarvin, m_pBombPickup))
		{
			m_pBombPickup->setEnabled(false);
			m_pMarvin->setNumBombs(m_pMarvin->getNumBombs() + 1);
			m_UI->m_setBomb(m_pMarvin->getNumBombs()); // This can go in an 'updateHud() or something similar if we make one'
			m_UI->m_setScore(m_UI->m_getScore(), 100);
		}
	}
	--cooldown;
	for (auto i = 0; i < m_pBombs.size(); i++)
	{
		if (m_marvinHealth->getHealthCount() == 0 )
		{
			TheGame::Instance()->changeSceneState(END_SCENE);
			break;
		}
		if ((m_pBombs[i]->checkAnimationFrame() > 10 && m_pBombs[i]->checkAnimationFrame() < 13) && CollisionManager::AABBCheck(m_pMarvin, m_pBombs[i]) && cooldown <= -10)
		{
			cooldown = 10;
			m_marvinHealth->setHealthCount(m_marvinHealth->getHealthCount() - 1);
			std::cout << m_marvinHealth->getHealthCount() << std::endl;
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
			//std::cout << "Bomb Deleted.";
		}
	}
}

void PlayScene::scrollObjects()
{

	if (m_pMarvin->getRigidBody()->velocity.x > 0 && m_pMarvin->getTransform()->position.x > Config::SCREEN_WIDTH * 0.7)
	{
		glm::vec2 ScrollSpeed;
		ScrollSpeed.x = m_pMarvin->getRigidBody()->velocity.x;
		m_pMarvin->getTransform()->position.x -= m_pMarvin->getRigidBody()->velocity.x;
		std::cout << "Moving Platforms Left" << std::endl;

		scrollAllObjects(ScrollSpeed);

	}

	if (m_pMarvin->getRigidBody()->velocity.x < 0 && m_pMarvin->getTransform()->position.x < Config::SCREEN_WIDTH * 0.3)
	{
		glm::vec2 ScrollSpeed;
		ScrollSpeed.x = m_pMarvin->getRigidBody()->velocity.x;
		m_pMarvin->getTransform()->position.x -= m_pMarvin->getRigidBody()->velocity.x;
		std::cout << "Moving Platforms Right" << std::endl;
		scrollAllObjects(ScrollSpeed);
	}

	if (m_pMarvin->getRigidBody()->velocity.y > 0 && m_pMarvin->getTransform()->position.y > Config::SCREEN_HEIGHT * 0.9)
	{
		glm::vec2 ScrollSpeed;
		ScrollSpeed.y = m_pMarvin->getRigidBody()->velocity.y;
		m_pMarvin->getTransform()->position.y -= m_pMarvin->getRigidBody()->velocity.y;
		std::cout << "Moving Platforms Up" << std::endl;
		scrollAllObjects(ScrollSpeed);


	}

	if (m_pMarvin->getRigidBody()->velocity.y < 0 && m_pMarvin->getTransform()->position.y < Config::SCREEN_HEIGHT * 0.3)
	{
		glm::vec2 ScrollSpeed;
		ScrollSpeed.y = m_pMarvin->getRigidBody()->velocity.y;
		m_pMarvin->getTransform()->position.y -= m_pMarvin->getRigidBody()->velocity.y;
		std::cout << "Moving Platforms Down" << std::endl;
		scrollAllObjects(ScrollSpeed);
	}
}

void PlayScene::updateTimer()
{
	if (TheGame::Instance()->getFrames() % 60 == 0)
	{
		m_UI->m_setTimer();
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
		m_UI->m_setBomb(m_pMarvin->getNumBombs()); // This can go in an 'updateHud() or something similar if we make one'
	}
	
	if (EventManager::Instance().keyPressed(SDL_SCANCODE_ESCAPE))
	{
		//TheGame::Instance()->quit();
		std::cout << "Paused" << std::endl;
		if (m_pPauseMenu->getPaused())
		{
			std::cout << " NOT Paused" << std::endl;
			m_pPauseMenu->setPaused(false);
		}
		else
		{
			std::cout << "Paused" << std::endl;
			m_pPauseMenu->setPaused(true);
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

	//std::cout << "LOL" << std::endl;
	//Platforms
	m_platforms.push_back(new Platform(glm::vec2(350.0f, 200.0f), 100, 20));
	m_platforms.push_back(new Platform(glm::vec2(150.0f, 350.0f), 100, 20));
	m_platforms.push_back(new Platform(glm::vec2(550.0f, 350.0f), 100, 20));
	m_platforms.push_back(new Platform(glm::vec2(-100.0f, 0.0f), 1000, 50)); // Ceiling
	m_platforms.push_back(new Platform(glm::vec2(-100.0f, 550.0f), 1000, 50)); // Floor
	m_platforms.push_back(new Platform(glm::vec2(1100.0f, 550.0f), 2000, 50)); // Floor 2
	m_platforms.push_back(new Platform(glm::vec2(1200.0f, 350.0f), 200, 20)); // 
	m_platforms.push_back(new Platform(glm::vec2(1400.0f, 200.0f), 200, 20)); // 
	m_platforms.push_back(new Platform(glm::vec2(1600.0f, 100.0f), 200, 20)); //
	m_platforms.push_back(new Platform(glm::vec2(1800.0f, 0.0f), 200, 20)); // 
	for (auto& count : m_platforms)
		addChild(count);

	m_pCrates.push_back(new Crate(glm::vec2(350.0f, 136.0f)));
	m_pCrates.push_back(new Crate(glm::vec2(675.0f, 300.0f)));
	m_pCrates.push_back(new Crate(glm::vec2(675.0f, 400.0f)));
	for (auto& crate : m_pCrates)
		addChild(crate);
	
	//Marvin
	m_pMarvin = new Marvin();
	addChild(m_pMarvin);
	m_marvinHealth = new Health();
	m_marvinHealth->getTransform()->position = glm::vec2(25.0f, 275.0f);
	addChild(m_marvinHealth);

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

    // Bomb Count, Score, Timer Label
	m_UI = new UserInterface();
	m_UI->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_UI);

	m_UI->m_addLabels();

	//Pause Menu
	m_pPauseMenu = new PauseMenu();
	addChild(m_pPauseMenu);

	
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
