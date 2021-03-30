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

	if (m_pMarvin->getHealthCount() == 0) {
		TheGame::Instance()->changeSceneState(LOSE_SCENE);
		return;
	}
	updateDisplayList();
	updateCollisions();
	scrollObjects();
	checkBombs();
	updateTimer();
	updateInsanity();
	m_AbilityBar->setAbilityCooldown(double(m_pMarvin->getGravityCooldown()));
}

void PlayScene::updateCollisions()
{
	for (auto& tile : m_pTiles) {
		//did collision between player and platform occur?
		if (CollisionManager::AABBCheck(m_pMarvin, tile)) {
			m_pMarvin->handleCollisions(tile);
		}

		for (auto& enemy : m_pFireEnemies)
		{
			if (CollisionManager::AABBCheck(enemy, tile))
				enemy->handleCollisions(tile);
		}

		//did collision between bomb and platforms occur?
		for (auto& bomb : m_pBombs) {
			if (CollisionManager::AABBCheck(bomb, tile)) {
				bomb->handleCollisions(tile);
				if (tile->GetTileType() == CRATE) {
					if (bomb->checkAnimationFrame() < 10)
						bomb->setAnimationFrame(10);
				}
			}
		}
		// did collision between crate and platform occur?
		for (auto& crate : m_pCrates) {
			if (CollisionManager::AABBCheck(crate, tile)) {
				crate->handleCollisions(tile);
			}
		}

		for (auto& bombCrate : m_pBombCrates)
		{
			if (CollisionManager::AABBCheck(bombCrate, tile))
			{
				bombCrate->handleCollisions(tile);
			}
		}

		for (auto& healthCrate : m_pHealthCrates)
		{
			if (CollisionManager::AABBCheck(healthCrate, tile))
			{
				healthCrate->handleCollisions(tile);
			}
		}
	}
	if (CollisionManager::AABBCheck(m_pMarvin, m_pDoor))
		TheGame::Instance()->changeSceneState(END_SCENE);
	
	for (auto& bomb : m_pBombs)
	{
		for (int i = 0; i < m_pCrates.size(); i++) {
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

		for (int i = 0; i < m_pBombCrates.size(); i++) {
			//is BombCrate near bomb when it explodes?
			if ((bomb->checkAnimationFrame() > 10 && bomb->checkAnimationFrame() < 13) &&
				Util::distance(bomb->getTransform()->position + glm::vec2(bomb->getWidth() / 2, bomb->getHeight() / 2),
					m_pBombCrates[i]->getTransform()->position + glm::vec2(m_pBombCrates[i]->getWidth() / 2, m_pBombCrates[i]->getHeight() / 2)) < 60) {
				m_pBombPickups.push_back(new BombPickup());
				m_pBombPickups.back()->getTransform()->position = m_pBombCrates[i]->getTransform()->position;
				addChild(m_pBombPickups.back());
				
				removeChild(m_pBombCrates[i]);
				m_pBombCrates[i] = nullptr;
				m_pBombCrates.erase(m_pBombCrates.begin() + i);
				m_pBombCrates.shrink_to_fit();
				break;
			}
		}

		for (int i = 0; i < m_pHealthCrates.size(); i++) {
			//is BombCrate near bomb when it explodes?
			if ((bomb->checkAnimationFrame() > 10 && bomb->checkAnimationFrame() < 13) &&
				Util::distance(bomb->getTransform()->position + glm::vec2(bomb->getWidth() / 2, bomb->getHeight() / 2),
					m_pHealthCrates[i]->getTransform()->position + glm::vec2(m_pHealthCrates[i]->getWidth() / 2, m_pHealthCrates[i]->getHeight() / 2)) < 60) {
				removeChild(m_pHealthCrates[i]);
				m_pHealthCrates[i] = nullptr;
				m_pHealthCrates.erase(m_pHealthCrates.begin() + i);
				m_pHealthCrates.shrink_to_fit();
				cooldown = 30;
				if (m_pMarvin->getHealthCount() != 3)
					m_pMarvin->setHealthCount(m_pMarvin->getHealthCount() + 1);
				break;
			}
		}
		
		for (int i = 0 ; i < m_pFireEnemies.size(); i++)
		{
			if (CollisionManager::AABBCheck(bomb, m_pFireEnemies[i])) {
				if (bomb->checkAnimationFrame() < 10)
					bomb->setAnimationFrame(10);
			}

			if ((bomb->checkAnimationFrame() > 10 && bomb->checkAnimationFrame() < 13) &&
				Util::distance(bomb->getTransform()->position + glm::vec2(bomb->getWidth() / 2, bomb->getHeight() / 2),
					m_pFireEnemies[i]->getTransform()->position + glm::vec2(m_pFireEnemies[i]->getWidth() / 2, m_pFireEnemies[i]->getHeight() / 2)) < 85)
			{
				//enemy->setEnabled(false);
				removeChild(m_pFireEnemies[i]);
				m_pFireEnemies[i] = nullptr;
				m_pFireEnemies.erase(m_pFireEnemies.begin() + i);
				m_pFireEnemies.shrink_to_fit();
				break;
			}
		}
	}

	for (auto& crate : m_pCrates)
	{
		//Did collision between player and crates occur
		if (CollisionManager::AABBCheck(m_pMarvin, crate))
			m_pMarvin->handleCollisions(crate);
		
		for (auto& otherCrate : m_pCrates) {
			if (crate != otherCrate && CollisionManager::AABBCheck(crate, otherCrate))
				crate->handleCollisions(otherCrate);
		}

		for (auto& otherCrate : m_pBombCrates)
		{

			if (crate != otherCrate && CollisionManager::AABBCheck(crate, otherCrate))
			crate->handleCollisions(otherCrate);
		}

		for (auto& otherCrate : m_pHealthCrates)
		{

			if (crate != otherCrate && CollisionManager::AABBCheck(crate, otherCrate))
			crate->handleCollisions(otherCrate);
		}
	}

	for (auto& bombCrate : m_pBombCrates)
	{
		if (CollisionManager::AABBCheck(m_pMarvin, bombCrate))
			m_pMarvin->handleCollisions(bombCrate);

		for (auto& otherCrate : m_pBombCrates)
		{
			if (bombCrate != otherCrate && CollisionManager::AABBCheck(bombCrate, otherCrate))
				bombCrate->handleCollisions(otherCrate);
		}

		for (auto& otherCrate : m_pCrates) 
		{

			if (bombCrate != otherCrate && CollisionManager::AABBCheck(bombCrate, otherCrate))
				bombCrate->handleCollisions(otherCrate);
		}

		for (auto& otherCrate : m_pHealthCrates)
		{

			if (bombCrate != otherCrate && CollisionManager::AABBCheck(bombCrate, otherCrate))
				bombCrate->handleCollisions(otherCrate);
		}
	}

	for (auto& healthCrate : m_pHealthCrates)
	{
		if (CollisionManager::AABBCheck(m_pMarvin, healthCrate))
			m_pMarvin->handleCollisions(healthCrate);

		for (auto& otherCrate : m_pBombCrates)
		{

			if (healthCrate != otherCrate && CollisionManager::AABBCheck(healthCrate, otherCrate))
				healthCrate->handleCollisions(otherCrate);
		}

		for (auto& otherCrate : m_pCrates) 
		{

			if (healthCrate != otherCrate && CollisionManager::AABBCheck(healthCrate, otherCrate))
				healthCrate->handleCollisions(otherCrate);
		}

		for (auto& otherCrate : m_pHealthCrates)
		{
			if (healthCrate != otherCrate && CollisionManager::AABBCheck(healthCrate, otherCrate))
				healthCrate->handleCollisions(otherCrate);
		}
	}

	--cooldown;
	for (auto i = 0; i < m_pBombs.size(); i++)
	{
		if ((m_pBombs[i]->checkAnimationFrame() > 10 && m_pBombs[i]->checkAnimationFrame() < 13) && CollisionManager::AABBCheck(m_pMarvin, m_pBombs[i]) && cooldown <= -10)
		{
			cooldown = 10;
			m_pMarvin->setHealthCount(m_pMarvin->getHealthCount() - 1);
			std::cout << m_pMarvin->getHealthCount() << std::endl;
		}
	}

	for (auto& enemy : m_pFireEnemies)
	{
		// Did collision between player and ememies occur?
		if (CollisionManager::AABBCheck(m_pMarvin, enemy) && cooldown <= -10)
		{
			m_pMarvin->handleCollisions(enemy);
			cooldown = 30;
			m_pMarvin->setHealthCount(m_pMarvin->getHealthCount() - 1);
		}
	}

	for (auto i = 0; i < m_pBombPickups.size() ; i++)
	{
		// Did collision between player and bombPickup occur?
		if (CollisionManager::AABBCheck(m_pMarvin, m_pBombPickups[i]))
		{
			removeChild(m_pBombPickups[i]);
			m_pBombPickups[i] = nullptr;
			m_pBombPickups.erase(m_pBombPickups.begin() + i);
			m_pBombPickups.shrink_to_fit();
			m_pMarvin->setNumBombs(m_pMarvin->getNumBombs() + 1);
			m_UI->m_setBomb(m_pMarvin->getNumBombs());
			break;
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
		}
	}
}

void PlayScene::scrollObjects()
{

	if (m_pMarvin->getRigidBody()->velocity.x > 0.0f && m_pMarvin->getTransform()->position.x > Config::SCREEN_WIDTH * 0.45)
	{
		glm::vec2 ScrollSpeed;
		ScrollSpeed.x = m_pMarvin->getRigidBody()->velocity.x;
		m_pMarvin->getTransform()->position.x -= m_pMarvin->getRigidBody()->velocity.x;
		scrollAllObjects(ScrollSpeed);
	}

	if (m_pMarvin->getRigidBody()->velocity.x < 0.0f && m_pMarvin->getTransform()->position.x < Config::SCREEN_WIDTH * 0.45)
	{
		glm::vec2 ScrollSpeed;
		ScrollSpeed.x = m_pMarvin->getRigidBody()->velocity.x;
		m_pMarvin->getTransform()->position.x -= m_pMarvin->getRigidBody()->velocity.x;
		scrollAllObjects(ScrollSpeed);
	}

	if (m_pMarvin->getRigidBody()->velocity.y > 0.9f && m_pMarvin->getTransform()->position.y > Config::SCREEN_HEIGHT * 0.6)
	{
		glm::vec2 ScrollSpeed;
		ScrollSpeed.y = m_pMarvin->getRigidBody()->velocity.y;
		m_pMarvin->getTransform()->position.y -= m_pMarvin->getRigidBody()->velocity.y;
		scrollAllObjects(ScrollSpeed);
	}

	if (m_pMarvin->getRigidBody()->velocity.y < -0.9f && m_pMarvin->getTransform()->position.y < Config::SCREEN_HEIGHT * 0.6)
	{
		glm::vec2 ScrollSpeed;
		ScrollSpeed.y = m_pMarvin->getRigidBody()->velocity.y;
		m_pMarvin->getTransform()->position.y -= m_pMarvin->getRigidBody()->velocity.y;
		scrollAllObjects(ScrollSpeed);
	}
}

void PlayScene::buildLevel()
{
	std::ifstream inputFile("../Assets/data/tileData.txt");

	if (inputFile.is_open()) {
		char key;
		TileType type;
		int srcX, srcY, srcW, srcH;

		while (!inputFile.eof()) {
			inputFile >> key >> srcX >> srcY >> srcW >> srcH;

			if (key == '-')
				type = PLATFORM;
			else if (key == 'C' || key == 'B' || key == 'H')
				type = CRATE;
			else if (key == '^' || key == 'v')
				type = DAMAGING;
			else if (key != '.')
				type = GROUND;

			SDL_Rect* src = new SDL_Rect();
			src->x = srcX * 256;
			src->y = srcY * 256;
			src->w = srcW * 4;
			src->h = srcH * 4;

			m_tiles.emplace(key, Tile(type, src));
			src = nullptr;
		}
	}

	inputFile.close();
	inputFile.open("../Assets/data/level_1.txt");

	int numRows, numCols;

	inputFile >> numCols >> numRows;
	std::cout << numRows << " " << numCols << std::endl;

	if (inputFile.is_open()) {
		for (int row = 0; row < numRows; row++) {
			for (int col = 0; col < numCols; col++) {
				char key;
				inputFile >> key;

				if (key == 'p') {
					m_pMarvin = new Marvin();
					m_pMarvin->getTransform()->position = glm::vec2(col * 40, row * 40);
				}
				else if (key == 'd') {
					m_pDoor = new Door();
					m_pDoor->getTransform()->position = glm::vec2(col * 40, row * 40);
					addChild(m_pDoor);
				}
				else if (key == 'C') {
					PhysicsTile* temp = new Crate(m_tiles[key].GetTileType(), m_tiles[key].GetSource());
					temp->getTransform()->position = glm::vec2(col * 40, row * 40);
					m_pTiles.push_back(temp);
					m_pCrates.push_back(temp);
					addChild(temp);
				}
				else if (key == 'E') {
					FireEnemy* e = new FireEnemy();
					e->getTransform()->position = glm::vec2(col * 40, row * 40);
					m_pFireEnemies.push_back(e);
					addChild(e);
				}
				else if (key == 'B')
				{
					PhysicsTile* temp = new Crate(m_tiles[key].GetTileType(), m_tiles[key].GetSource());
					temp->getTransform()->position = glm::vec2(col * 40, row * 40);
					m_pTiles.push_back(temp);
					m_pBombCrates.push_back(temp);
					addChild(temp);
				}
				else if (key == 'H')
				{
					PhysicsTile* temp = new Crate(m_tiles[key].GetTileType(), m_tiles[key].GetSource());
					temp->getTransform()->position = glm::vec2(col * 40, row * 40);
					m_pTiles.push_back(temp);
					m_pHealthCrates.push_back(temp);
					addChild(temp);
				}
				else if (key != '.') {
					Tile* temp = new Tile(m_tiles[key].GetTileType(), m_tiles[key].GetSource());
					temp->getTransform()->position = glm::vec2(col * 40, row * 40);
					m_pTiles.push_back(temp);
					addChild(temp);
				}
			}
		}
	}

	inputFile.close();
}

void PlayScene::updateTimer()
{
	if (TheGame::Instance()->getFrames() % 60 == 0)
	{
		m_UI->m_setTimer();
	}
}

void PlayScene::updateInsanity()
{
	if (TheGame::Instance()->getFrames() % 60 == 0)
	{
		m_UI->m_setInsanity();
		m_insanity = m_UI->m_getInsanity();
		std::cout << "Current sanity is " << m_insanity << std::endl;
		if (m_insanity >= 60)
		{
			m_event1Countdown++;
			std::cout << "Countdown to event " << m_event1Countdown << std::endl;
			if (m_event1Countdown >= 30)
			{
				m_event1Countdown = 0;
				std::cout << "Event triggured" << std::endl;
				//SoundManager::Instance().playSound("scream", 0, 0);
			}
		}
	}
}

void PlayScene::clean()
{
	removeAllChildren();
	m_pBombs.clear();
	m_pTiles.clear();
	m_pCrates.clear();
	m_pFireEnemies.clear();
	m_pLabels.clear();
}

void PlayScene::handleEvents()
{
	EventManager::Instance().update();

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
				m_pMarvin->setForce(-18.0f);
				m_pMarvin->setAngle(0.0f);
				m_pMarvin->setGravityFlipped(false);
				m_pMarvin->setCurrentDirection(glm::vec2(m_pMarvin->getCurrentDirection().x, 1.0f));
				m_pMarvin->ChangeDirection();
			}
			else{
				m_pMarvin->setGravity(-12.0f);
				m_pMarvin->setForce(18.0f);
				m_pMarvin->setAngle(180.0f);
				m_pMarvin->setGravityFlipped(true);
				m_pMarvin->setCurrentDirection(glm::vec2(m_pMarvin->getCurrentDirection().x, -1.0f));
				m_pMarvin->ChangeDirection();
			}
			m_pMarvin->setGravityCooldown(60);
			m_pMarvin->setIsGrounded(false);
		}
	}

	

	if (EventManager::Instance().keyPressed(SDL_SCANCODE_E) && m_pMarvin->getNumBombs() > 0 && m_pMarvin->getBombCooldown() == 0)
	{
		m_pBombs.push_back(new Bomb(m_pMarvin->getTransform()->position, m_pMarvin->getCurrentDirection()));
		addChild(m_pBombs.back());
		m_pBombs.shrink_to_fit();
		if (m_pMarvin->getNumBombs() != 0)
			m_pMarvin->setNumBombs(m_pMarvin->getNumBombs() - 1);
		m_pMarvin->setBombCooldown(75); // Sets bomb cooldown to 1.25 seconds
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

	m_pBackground = new BackgroundImage();
	m_pBackground->setEnabled(true);
	addChild(m_pBackground);

	buildLevel();
	
	//Instruction Labels
	m_pLabels.push_back(new Label("use [A] and [D] to move around", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(320.0f, 240.0f)));
	m_pLabels.push_back(new Label("use [W] to jump", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(320.0f, 280.0f)));
	m_pLabels.push_back(new Label("press [SPACE] to Flip gravity!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(14 * 40.0f, 17.0f * 40.0f)));
	m_pLabels.push_back(new Label("press [E] to throw a bomb.", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(240.0f, 34 * 40.0f)));
	m_pLabels.push_back(new Label("You can throw bombs while gravity is flipped!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(34 * 40.0f, 37 * 40.0f)));
	m_pLabels.push_back(new Label("Watch out for the blast though!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(240.0f, 35 * 40.0f)));
	m_pLabels.push_back(new Label("use bombs to defeat enemies!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(36 * 40.0f, 27 * 40.0f)));
	for (auto label : m_pLabels)
		addChild(label);

	//Marvin
	//marvin is built in buildLevel(). he is added here so that the UI renders in front of the tiles
	addChild(m_pMarvin);

	// Set Marvin's bombs to 100 (temporary)
	m_pMarvin->setNumBombs(100);

	// Bomb Pickup
	//m_pBombPickup = new BombPickup();
	//addChild(m_pBombPickup);
	//m_pBombPickup->getTransform()->position = glm::vec2(165.0f, 270.0f);

	//Insanity Brain
	// Brain needs to follow with the ui layout
	m_pBrain = new Brain();
	m_pBrain->getTransform()->position = glm::vec2(400.0f, 40.0f);
	m_pBrain->setEnabled(true);
	addChild(m_pBrain);

	//Charge Bar
	m_AbilityBar = new AbilityBar();
	m_AbilityBar->getTransform()->position = glm::vec2(50.0f, 75.0f);
	m_AbilityBar->setAbilityCooldown(1.00);
	m_AbilityBar->setEnabled(true);
	addChild(m_AbilityBar);

	SoundManager::Instance().load("../Assets/audio/scream1.mp3", "scream", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/explosion_1.mp3", "explosion1", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/explosion_2.mp3", "explosion2", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/explosion_3.mp3", "explosion3", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/background1.mp3", "background", SOUND_MUSIC);
	SoundManager::Instance().setMusicVolume(5);

    // Bomb Count, Score, Timer Label
	m_UI = new UserInterface();
	m_UI->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_UI);

	m_UI->m_addLabels();

	//Pause Menu
	m_pPauseMenu = new PauseMenu();
	addChild(m_pPauseMenu);

	SoundManager::Instance().playMusic("background", -1, 0);
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
