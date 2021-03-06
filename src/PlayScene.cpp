#include "PlayScene.h"
#include "Game.h"
#include "EventManager.h"

// required for IMGUI
#include "imgui.h"
#include "imgui_sdl.h"
#include "Renderer.h"

int PlayScene::m_level = 1;

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
	srand((unsigned)time(NULL));

	if (m_pPauseMenu->getPaused())
	{
		m_pPauseMenu->update();
		return;
	}

	m_DetectedPlayer(m_pMarvin);

	for (auto enemies : m_pFireEnemies)
	{
		enemies->setCooldown(enemies->getCooldown() - 1);

		if (enemies->hasDetection())
		{
			if (enemies->getFireBallActive() == false && enemies->getCooldown() <= 0)
			{
				if (m_pMarvin->getTransform()->position.x < enemies->getTransform()->position.x)
				{
					enemies->setCurrentDirection(glm::vec2(-1.0f, enemies->getCurrentDirection().y));
					enemies->setAnimationState(ENEMY_RUN_LEFT);
					SoundManager::Instance().playSound("eThrow", 0, 0);
					enemies->m_pFireballs.push_back(new Fireball(enemies->getTransform()->position, enemies->getCurrentDirection(), 3.5f, 180.0f, false));
					addChild(enemies->m_pFireballs.back());
					enemies->setFireBallActive(true);
					enemies->setCooldown(150 + (rand() % 100));
					std::cout << "Cool Down: " << enemies->getCooldown() << std::endl;
				}
				else if (m_pMarvin->getTransform()->position.x >= enemies->getTransform()->position.x)
				{
					enemies->setCurrentDirection(glm::vec2(1.0f, enemies->getCurrentDirection().y));
					enemies->setAnimationState(ENEMY_RUN_RIGHT);
					SoundManager::Instance().playSound("eThrow", 0, 0);
					enemies->m_pFireballs.push_back(new Fireball(enemies->getTransform()->position, enemies->getCurrentDirection(), 3.5f, 0.0f, false));
					addChild(enemies->m_pFireballs.back());
					enemies->setFireBallActive(true);
					enemies->setCooldown(150 + (rand() % 100));
					std::cout << "Cool Down: " << enemies->getCooldown() << std::endl;
				}
			}
		}
	}

	if (m_pMarvin->getHealthCount() <= 0) {
		if (m_pMarvin->getAnimationState() != PLAYER_DEATH); {
			m_pMarvin->setAnimationState(PLAYER_DEATH);
			m_pMarvin->setIsDead(true);
		}
		if(TextureManager::Instance()->checkAnimationDone(m_pMarvin->getAnimation("death")))
		{
			TheGame::Instance()->changeSceneState(LOSE_SCENE);
			return;
		}
	}
	//m_pBossEnemy->isBossDead();
	//sets to false on every frame, but will be overriden with true as long as he is within 1 gravity nullifier
	m_pMarvin->setIsWithinGravityNullifier(false);

	updateDisplayList();
	updateCollisions();
	scrollObjects();
	checkBombs();
	updateTimer();

	if (m_level == 3)
		BossAttack();

	m_AbilityBar->setAbilityCooldown(double(m_pMarvin->getGravityCooldown()));

	if (CollisionManager::AABBCheck(m_pMarvin, m_pDoor) && !m_pMarvin->isDead())
	{
		if (m_level == 3)
		{
			m_level = 1;
			TheGame::Instance()->changeSceneState(START_SCENE);
		}
		else
		{
			m_level++;
			TheGame::Instance()->changeSceneState(LOADING_SCENE);
		}
	}
}

void PlayScene::updateCollisions()
{
	for (auto& tile : m_pTiles) 
	{
		//did collision between player and platform occur?
		if (CollisionManager::AABBCheck(m_pMarvin, tile)) 
		{
			m_pMarvin->handleCollisions(tile);
		}

		for (auto& enemy : m_pFireEnemies)
		{
			if (CollisionManager::AABBCheck(enemy, tile))
			{
				enemy->handleCollisions(tile);
			}
				
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

		if (tile->GetTileType() == GRAVITY_NULLIFIER) {
			if (Util::distance(m_pMarvin->getTransform()->position, tile->getTransform()->position) <= static_cast<GravityNullifier*>(tile)->getEffectRadius()) {
				m_pMarvin->setIsWithinGravityNullifier(true);
			}
		}
	}

	for (int i = 0; i < m_pHadesFlamingOrb.size(); i++)
	{
		if (CollisionManager::AABBCheck(m_pMarvin, m_pHadesFlamingOrb[i]) && cooldown <= -20)
		{
			m_pMarvin->setHealthCount(m_pMarvin->getHealthCount() - 1);
			m_pMarvin->setAnimationState(PLAYER_DAMAGE);
			cooldown = 10;
		}
	}

	if (m_level == 3)
	{
		if (CollisionManager::AABBCheck(m_pMarvin, m_pBossEnemy) && !m_pBossEnemy->checkAnimationDone("death"))
		{
			m_pMarvin->handleCollisions(m_pBossEnemy);
			if (cooldown <= -10) {
				m_pMarvin->setHealthCount(m_pMarvin->getHealthCount() - 1);
				cooldown = 10;
			}
		}

		if (m_pBossEnemy->isFloorSpikes() && !m_pBossEnemy->checkAnimationDone("death")) {
			for (auto spike : m_pFloorSpikes) {
				if (CollisionManager::AABBCheck(m_pMarvin, spike) && m_pMarvin->getAnimationState() != PLAYER_DEATH && cooldown <= -10) {
					cooldown = 10;
					m_pMarvin->handleCollisions(spike);
					break;
				}
			}
		}
		else {
			for (auto spike : m_pRoofSpikes) {
				if (CollisionManager::AABBCheck(m_pMarvin, spike) && m_pMarvin->getAnimationState() != PLAYER_DEATH && cooldown <= -10) {
					cooldown = 10;
					m_pMarvin->handleCollisions(spike);
					break;
				}
			}

		}

		for (int i = 0; i < m_pCrates.size(); i++)
		{
			if (CollisionManager::AABBCheck(m_pCrates[i], m_pBossEnemy))
			{
				m_pBossEnemy->setAnimationState(BOSS_DEATH);
			}
		}
	}
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
				m_UI->m_setScore(10);
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
				m_UI->m_setScore(25);
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
				if (m_pMarvin->getHealthCount() != 3 && !m_pMarvin->isDead())
					m_pMarvin->setHealthCount(m_pMarvin->getHealthCount() + 1);
				m_UI->m_setScore(25);
				break;
			}
		}
		
		for (int i = 0 ; i < m_pFireEnemies.size(); i++)
		{
			if (CollisionManager::AABBCheck(bomb, m_pFireEnemies[i])) {
				if (bomb->checkAnimationFrame() < 10)
					bomb->setAnimationFrame(10);
			}

			if ((bomb->checkAnimationFrame() >= 10 && bomb->checkAnimationFrame() <= 13) &&
				Util::distance(bomb->getTransform()->position + glm::vec2(bomb->getWidth() / 2, bomb->getHeight() / 2),
					m_pFireEnemies[i]->getTransform()->position + glm::vec2(m_pFireEnemies[i]->getWidth() / 2, m_pFireEnemies[i]->getHeight() / 2)) < 85)
			{
				m_pFireEnemies[i]->setAnimationState(ENEMY_DEATH);
				m_pFireEnemies[i]->setMaxSpeed(0);
				if (bomb->checkAnimationFrame() == 13) {
					SoundManager::Instance().playSound("eDeath", 0, 0);
					removeChild(m_pFireEnemies[i]);
					m_pFireEnemies[i] = nullptr;
					m_pFireEnemies.erase(m_pFireEnemies.begin() + i);
					m_pFireEnemies.shrink_to_fit();
					m_UI->m_setScore(100);
				}
				break;
			}
		}
		if (m_level == 3)
		{
			for (int i = 0; i < m_pCrates.size(); i++)
			{
				if (CollisionManager::AABBCheck(m_pCrates[i], m_pBossEnemy))
				{
					m_pBossEnemy->setAnimationState(BOSS_DEATH);
				}
			}
		}
		for (auto& DestructibleTile : m_pDestructibleTile)
		{
			if (CollisionManager::AABBCheck(bomb, DestructibleTile)) {
				if (bomb->checkAnimationFrame() < 10)
					bomb->setAnimationFrame(10);
			}

			if ((bomb->checkAnimationFrame() > 10 && bomb->checkAnimationFrame() < 13) &&
				Util::distance(bomb->getTransform()->position + glm::vec2(bomb->getWidth() / 2, bomb->getHeight() / 2),
					DestructibleTile->getTransform()->position + glm::vec2(DestructibleTile->getWidth() / 2, DestructibleTile->getHeight() / 2)) < 60)
			{
				for (auto& DestructibleTile1 : m_pDestructibleTile)
				{
					removeChild(DestructibleTile1);
					DestructibleTile1 = nullptr;
				}
				m_pDestructibleTile.clear();
				m_pDestructibleTile.shrink_to_fit();
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
		if ((m_pBombs[i]->checkAnimationFrame() > 10 && m_pBombs[i]->checkAnimationFrame() < 13) && CollisionManager::AABBCheck(m_pMarvin, m_pBombs[i]) && cooldown <= -10 
			&& !m_pMarvin->isDead())
		{
			cooldown = 10;
			m_pMarvin->setHealthCount(m_pMarvin->getHealthCount() - 1);
			m_pMarvin->setAnimationState(PLAYER_DAMAGE);
			std::cout << m_pMarvin->getHealthCount() << std::endl;
		}
	}

	for (auto& enemy : m_pFireEnemies)
	{
		// Did collision between player and ememies occur?
		if (CollisionManager::AABBCheck(m_pMarvin, enemy) && cooldown <= -10 && !m_pMarvin->isDead())
		{
			m_pMarvin->handleCollisions(enemy);
			cooldown = 30;
			m_pMarvin->setHealthCount(m_pMarvin->getHealthCount() - 1);
			m_pMarvin->setAnimationState(PLAYER_DAMAGE);
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
			m_pMarvin->setNumBombs(m_pMarvin->getNumBombs() + 2);
			m_UI->m_setBomb(m_pMarvin->getNumBombs());
			break;
		}
	}

	for (auto& fire_enemy : m_pFireEnemies)
	{
		for (int i = 0; i < fire_enemy->m_pFireballs.size(); i++)
		{
			if (fire_enemy->m_pFireballs[i] != nullptr)
			{
				if (CollisionManager::AABBCheck(fire_enemy->m_pFireballs[i], m_pMarvin) && !m_pMarvin->isDead())
				{
					removeChild(fire_enemy->m_pFireballs[i]);
					fire_enemy->m_pFireballs[i] = nullptr;
					fire_enemy->m_pFireballs.erase(fire_enemy->m_pFireballs.begin() + i);
					fire_enemy->m_pFireballs.shrink_to_fit();
					fire_enemy->setFireBallActive(false);
					m_pMarvin->setHealthCount(m_pMarvin->getHealthCount() - 1);
					m_pMarvin->setAnimationState(PLAYER_DAMAGE);
					break;
				}
			}

			for (auto& tiles : m_pTiles)
			{

				if (fire_enemy->m_pFireballs[i] != nullptr)
				{
					if (CollisionManager::AABBCheck(fire_enemy->m_pFireballs[i], tiles))
					{
						removeChild(fire_enemy->m_pFireballs[i]);
						fire_enemy->m_pFireballs[i] = nullptr;
						fire_enemy->m_pFireballs.erase(fire_enemy->m_pFireballs.begin() + i);
						fire_enemy->m_pFireballs.shrink_to_fit();
						fire_enemy->setFireBallActive(false);
						break;

					}
				}
			}//end tile check
			
			
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
			break;
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
			else if (key == 'D')
				type = DESTRUCTIBLE_TILE;
			else if (key == 'C' || key == 'B' || key == 'H')
				type = CRATE;
			else if (key == '^' || key == 'v')
				type = DAMAGING;
			else if (key == 'N' || key == 'n')
				type = GRAVITY_NULLIFIER;
			else if (key != '.')
				type = GROUND;
			

			SDL_Rect* src = new SDL_Rect();
			src->x = srcX * 64;
			src->y = srcY * 64;
			src->w = srcW;
			src->h = srcH;

			m_tiles.emplace(key, Tile(type, src));
			src = nullptr;
		}
	}

	inputFile.close();
	inputFile.open("../Assets/data/level_"+ std::to_string(m_level) +".txt");

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
					temp = nullptr;
				}
				else if (key == 'N' || key == 'n') {
					GravityNullifier* temp = new GravityNullifier(m_tiles[key].GetTileType(), m_tiles[key].GetSource());
					temp->getTransform()->position = glm::vec2(col * 40, row * 40);
					m_pTiles.push_back(temp);
					addChild(temp);
					temp = nullptr;
				}
				else if (key == 'E') {
					FireEnemy* e = new FireEnemy();
					e->getTransform()->position = glm::vec2(col * 40, row * 40);
					m_pFireEnemies.push_back(e);
					addChild(e);
					e = nullptr;
				}
				else if(key == 'Z')
				{
					m_pBossEnemy = new BossEnemy();
					m_pBossEnemy->getTransform()->position = glm::vec2(col * 40, row * 40);
					addChild(m_pBossEnemy);
				}
				else if (key == 'B')
				{
					PhysicsTile* temp = new Crate(m_tiles[key].GetTileType(), m_tiles[key].GetSource());
					temp->getTransform()->position = glm::vec2(col * 40, row * 40);
					m_pTiles.push_back(temp);
					m_pBombCrates.push_back(temp);
					addChild(temp);
					temp = nullptr;
				}
				else if (key == 'H')
				{
					PhysicsTile* temp = new Crate(m_tiles[key].GetTileType(), m_tiles[key].GetSource());
					temp->getTransform()->position = glm::vec2(col * 40, row * 40);
					m_pTiles.push_back(temp);
					m_pHealthCrates.push_back(temp);
					addChild(temp);
					temp = nullptr;
				}
				else if (key == 'D')
				{
					DestructibleTile* temp = new DestructibleTile(m_tiles[key].GetTileType(), m_tiles[key].GetSource());
					temp->getTransform()->position = glm::vec2(col * 40, row * 40);
					m_pTiles.push_back(temp);
					m_pDestructibleTile.push_back(temp);
					addChild(temp);
				}
				else if (key != '.') {
					Tile* temp = new Tile(m_tiles[key].GetTileType(), m_tiles[key].GetSource());
					temp->getTransform()->position = glm::vec2(col * 40, row * 40);
					m_pTiles.push_back(temp);
					addChild(temp);
					temp = nullptr;
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


void PlayScene::BossAttack()
{
	if (m_pBossEnemy->getAttackCoolDown() == 0)
	{
		if (m_pBossEnemy->isFloorSpikes())
		{
			for (int i = 0; i < m_pFloorSpikes.size(); i++)
			{
				m_pFloorSpikes[i]->setEnabled(true);
				m_pRoofSpikes[i]->setEnabled(false);

				if (CollisionManager::AABBCheck(m_pMarvin, m_pFloorSpikes[i])) {
					m_pMarvin->getTransform()->position.y = m_pFloorSpikes[i]->getTransform()->position.y - m_pMarvin->getHeight() - 2;
					m_pMarvin->setHealthCount(m_pMarvin->getHealthCount() - 1);
					if (!m_pMarvin->isGravityFlipped())
						m_pMarvin->jump();
				}
			}
		}
		else
		{
			for (int i = 0; i < m_pFloorSpikes.size(); i++)
			{
				m_pFloorSpikes[i]->setEnabled(false);
				m_pRoofSpikes[i]->setEnabled(true);

				if (CollisionManager::AABBCheck(m_pMarvin, m_pRoofSpikes[i])) {
					m_pMarvin->getTransform()->position.y = m_pRoofSpikes[i]->getTransform()->position.y + m_pRoofSpikes[i]->getHeight() + 2;
					m_pMarvin->setHealthCount(m_pMarvin->getHealthCount() - 1);
					if (m_pMarvin->isGravityFlipped())
						m_pMarvin->jump();
				}
			}
		}
	}

	m_pBossEnemy->setTargetPosition(m_pMarvin->getTransform()->position);

	if(m_pBossEnemy->getTransform()->position.x > 800 || m_pBossEnemy->getTransform()->position.x < 0 || m_pBossEnemy->getTransform()->position.y > 600)
	{
		if(m_hadesFireTimer == 0)
		{
			m_pHadesFlamingOrb.push_back(new Fireball(glm::vec2(900, m_pMarvin->getTransform()->position.y), m_pBossEnemy->getCurrentDirection(), -5.0f, 180.0f, true));
			addChild(m_pHadesFlamingOrb.back());
			m_hadesFireTimer = 200;
		}
	}

	else if (m_hadesFireTimer == 0 && m_pBossEnemy->getAnimationState() != BOSS_DEATH)
	{
		m_pHadesFlamingOrb.push_back(new Fireball(glm::vec2(m_pBossEnemy->getTransform()->position.x, m_pBossEnemy->getTargetPosition().y), m_pBossEnemy->getCurrentDirection(), -5.0f, 180.0f, true));
		addChild(m_pHadesFlamingOrb.back());
		m_pBossEnemy->setAnimationState(BOSS_SHOOT);
		m_hadesFireTimer = 200;
	}
	else if (m_hadesFireTimer == 125 && m_pBossEnemy->getAnimationState() != BOSS_DEATH)
	{
		m_pBossEnemy->setAnimationState(BOSS_IDLE);
	}

	m_hadesFireTimer--;
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

	// Enemy Movement
	for (int i = 0; i < m_pFireEnemies.size(); i++)
	{
		m_pFireEnemies[i]->move();
	}

	if (!m_pMarvin->isGrounded() && m_pMarvin->getAnimationState() != PLAYER_DAMAGE && m_pMarvin->getAnimationState() != PLAYER_BOMB_RIGHT && m_pMarvin->getAnimationState() != PLAYER_BOMB_LEFT && !m_pMarvin->isDead())
		m_pMarvin->setAnimationState(PLAYER_MIDAIR);
	else
	{
		if (m_pMarvin->getAnimationState() != PLAYER_BOMB_RIGHT && m_pMarvin->getAnimationState() != PLAYER_BOMB_LEFT && m_pMarvin->getAnimationState() != PLAYER_DAMAGE)
		{
			if (!m_pMarvin->getDirection())
				m_pMarvin->setAnimationState(PLAYER_IDLE_RIGHT);
			else
				m_pMarvin->setAnimationState(PLAYER_IDLE_LEFT);
		}
	}

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_A) && !m_pMarvin->isDead()) {
		m_footsteps--;
		if (m_footsteps <= 0)
		{
			m_footsteps = 30;
			SoundManager::Instance().playSound("walking", 0, 1);
		}
		m_pMarvin->setIsMoving(true);
		if (m_pMarvin->isGrounded() && m_pMarvin->getAnimationState() != PLAYER_DAMAGE && m_pMarvin->getAnimationState() != PLAYER_DEATH)
			m_pMarvin->setAnimationState(PLAYER_RUN_LEFT);
		m_pMarvin->setCurrentDirection(glm::vec2(-1.0f, m_pMarvin->getCurrentDirection().y));
	}
	else if (EventManager::Instance().isKeyDown(SDL_SCANCODE_D) && !m_pMarvin->isDead()) {
		m_footsteps--;
		if (m_footsteps <= 0)
		{
			m_footsteps = 30;
			SoundManager::Instance().playSound("walking", 0, 1);
		}
		m_pMarvin->setIsMoving(true);
		if (m_pMarvin->isGrounded() && m_pMarvin->getAnimationState() != PLAYER_DAMAGE && !m_pMarvin->isDead())
			m_pMarvin->setAnimationState(PLAYER_RUN_RIGHT);
		m_pMarvin->setCurrentDirection(glm::vec2(1.0f, m_pMarvin->getCurrentDirection().y));
	}
	else
		m_pMarvin->setIsMoving(false);

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_W) && !m_pMarvin->isDead())
		{
			m_pMarvin->jump();
		}
		
	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_SPACE) && !m_pMarvin->isWithinGravityNullifier() &&
		m_pMarvin->getGravityCooldown() == 0 && m_pMarvin->isGrounded() && !m_pMarvin->isDead())
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
		else {
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

	if (EventManager::Instance().keyPressed(SDL_SCANCODE_E) && m_pMarvin->getNumBombs() > 0 && m_pMarvin->getBombCooldown() == 0 && !m_pMarvin->isDead())
	{
		
		if (!m_pMarvin->getDirection())
			m_pMarvin->setAnimationState(PLAYER_BOMB_RIGHT);
		else
			m_pMarvin->setAnimationState(PLAYER_BOMB_LEFT);
	}

	if(m_pMarvin->getAnimationState() == PLAYER_BOMB_RIGHT)
	{
		if (TextureManager::Instance()->checkAnimationDone(m_pMarvin->getAnimation("bomb right")))
		{
			if(m_pMarvin->isGravityFlipped())
				m_pBombs.push_back(new Bomb(m_pMarvin->getTransform()->position + glm::vec2{0.0f, 90.0f }, m_pMarvin->getCurrentDirection()));
			else
				m_pBombs.push_back(new Bomb(m_pMarvin->getTransform()->position + glm::vec2{0.0f, 10.0f}, m_pMarvin->getCurrentDirection()));
			addChild(m_pBombs.back());
			m_pBombs.shrink_to_fit();
			if (m_pMarvin->getNumBombs() != 0)
				m_pMarvin->setNumBombs(m_pMarvin->getNumBombs() - 1);
			m_pMarvin->setBombCooldown(75);
			m_UI->m_setBomb(m_pMarvin->getNumBombs());
			m_pMarvin->setAnimationFrame("bomb right", 0);
			m_pMarvin->setAnimationState(PLAYER_IDLE_RIGHT);
		}
	}
	
	if(m_pMarvin->getAnimationState() == PLAYER_BOMB_LEFT)
	{
		if (TextureManager::Instance()->checkAnimationDone(m_pMarvin->getAnimation("bomb left")))
		{
			if(m_pMarvin->isGravityFlipped())
				m_pBombs.push_back(new Bomb(m_pMarvin->getTransform()->position + glm::vec2{ 0.0f, 90.0f }, m_pMarvin->getCurrentDirection()));
			else
				m_pBombs.push_back(new Bomb(m_pMarvin->getTransform()->position - glm::vec2{ 0.0f, 10.0f }, m_pMarvin->getCurrentDirection()));
			addChild(m_pBombs.back());
			m_pBombs.shrink_to_fit();
			if (m_pMarvin->getNumBombs() != 0)
				m_pMarvin->setNumBombs(m_pMarvin->getNumBombs() - 1);
			m_pMarvin->setBombCooldown(75);
			m_UI->m_setBomb(m_pMarvin->getNumBombs());
			m_pMarvin->setAnimationFrame("bomb left", 0);
			m_pMarvin->setAnimationState(PLAYER_IDLE_LEFT);
		}
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
		TheGame::Instance()->changeSceneState(START_SCENE);

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_2))
		TheGame::Instance()->changeSceneState(END_SCENE);

	if (EventManager::Instance().isKeyDown(SDL_SCANCODE_3))
	{
		m_level++;
		TheGame::Instance()->changeSceneState(LOADING_SCENE);
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
	if (m_level == 1)
	{
		m_pLabels.push_back(new Label("use [A] and [D] to move around", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(320.0f, 240.0f)));
		m_pLabels.push_back(new Label("use [W] to jump", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(320.0f, 280.0f)));
		m_pLabels.push_back(new Label("press [SPACE] to Flip gravity!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(14 * 40.0f, 17.0f * 40.0f)));
		m_pLabels.push_back(new Label("press [E] to throw a bomb.", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(240.0f, 34 * 40.0f)));
		m_pLabels.push_back(new Label("You can throw bombs while gravity is flipped!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(34 * 40.0f, 37 * 40.0f)));
		m_pLabels.push_back(new Label("Watch out for the blast though!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(240.0f, 35 * 40.0f)));
		m_pLabels.push_back(new Label("use bombs to defeat enemies!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(36 * 40.0f, 27 * 40.0f)));
		for (auto label : m_pLabels)
			addChild(label);
	}
	
	if (m_level == 2)
	{
		m_pLabels.push_back(new Label("Look out for the gravity nullifiers!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(19.0f * 40.0f, 33.0f * 40.0f)));
		m_pLabels.push_back(new Label("You won't be able to flip gravity", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(20.0f * 40.0f, 34.0f * 40.0f)));
		for (auto label : m_pLabels)
			addChild(label);
	}

	//Boss Labels
	if (m_level == 3)
	{
		m_pLabels.push_back(new Label("!", "BLOODY", 35, { 255, 0, 0, 255 }, glm::vec2(62.0f, 690.0f)));
		m_pLabels.push_back(new Label("!", "BLOODY", 35, { 255, 0, 0, 255 }, glm::vec2(62.0f, 800.0f)));
		m_pLabels.push_back(new Label("!", "BLOODY", 35, { 255, 0, 0, 255 }, glm::vec2(142.0f, 1340.0f)));
		m_pLabels.push_back(new Label("!", "BLOODY", 35, { 255, 0, 0, 255 }, glm::vec2(142.0f, 1450.0f)));
		m_pLabels.push_back(new Label("Try Walking Off....", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(900.0f, 440.0f)));
		m_pLabels.push_back(new Label("Then Flip!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(900.0f, 490.0f)));
		m_pLabels.push_back(new Label("Danger! Boss Ahead!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(450.0f, 1890.0f)));
		m_pLabels.push_back(new Label("Falling Crates May Cause Harm!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(1650.0f, 1340.0f)));
		m_pLabels.push_back(new Label("Freedom!", "BLOODY", 20, { 255, 0, 0, 255 }, glm::vec2(2120.0f, 1760.0f)));
		for (auto label : m_pLabels)
			addChild(label);
	}

	//Marvin
	//marvin is built in buildLevel(). he is added here so that the UI renders in front of the tiles
	addChild(m_pMarvin);

	m_pMarvin->setNumBombs(5);

	// Bomb Pickup
	//m_pBombPickup = new BombPickup();
	//addChild(m_pBombPickup);
	//m_pBombPickup->getTransform()->position = glm::vec2(165.0f, 270.0f);


	//Charge Bar
	m_AbilityBar = new AbilityBar();
	m_AbilityBar->getTransform()->position = glm::vec2(50.0f, 75.0f);
	m_AbilityBar->setAbilityCooldown(1.00);
	m_AbilityBar->setEnabled(true);
	addChild(m_AbilityBar);

	SoundManager::Instance().load("../Assets/audio/scream1.mp3", "scream", SOUND_SFX);
	//SoundManager::Instance().load("../Assets/audio/explosion_1.mp3", "explosion1", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/explosion_2.mp3", "explosion2", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/explosion_3.mp3", "explosion3", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/enemydeath.wav", "eDeath", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/enemythrow.wav", "eThrow", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/walking.wav", "walking", SOUND_SFX);
	SoundManager::Instance().load("../Assets/audio/background1.mp3", "background", SOUND_MUSIC);
	SoundManager::Instance().load("../Assets/audio/FinalBoss.mp3", "FinalBoss", SOUND_MUSIC);
	SoundManager::Instance().setMusicVolume(5);
    // Bomb Count, Score, Timer Label
	m_UI = new UserInterface();
	m_UI->getTransform()->position = glm::vec2(400.0f, 300.0f);
	addChild(m_UI);

	m_UI->m_addLabels();

	//Pause Menu
	m_pPauseMenu = new PauseMenu();
	addChild(m_pPauseMenu);

	if(m_level < 3)
		SoundManager::Instance().playMusic("background", -1, 0);

	if(m_level == 3)
		SoundManager::Instance().playMusic("FinalBoss", -1, 0);

	if (m_level == 3)
	{
		for (int i = 1; i < 48; i++)
		{
			Tile* temp = new Tile(m_tiles['^'].GetTileType(), m_tiles['^'].GetSource());
			temp->getTransform()->position = glm::vec2(i * 40, 49 * 40);
			m_pFloorSpikes.push_back(temp);
			addChild(temp);
			temp->setEnabled(true);
			temp = nullptr;
		}
		for (int i = 1; i < 49; i++)
		{
			Tile* temp = new Tile(m_tiles['v'].GetTileType(), m_tiles['v'].GetSource());
			temp->getTransform()->position = glm::vec2(i * 40, 1 * 40);
			m_pRoofSpikes.push_back(temp);
			addChild(temp);
			temp->setEnabled(false);
			temp = nullptr;
		}

	}
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

void PlayScene::m_DetectedPlayer(DisplayObject* object)
{
		// if enemy to player distance is less than or equal to detection Distance
	for (auto enemies : m_pFireEnemies)
	{
		auto EnemyToPlayerDistance = Util::distance(enemies->getTransform()->position, object->getTransform()->position);
		if (EnemyToPlayerDistance <= enemies->getDetectionDistance())
		{
			std::vector<DisplayObject*> contactList;
			for (auto object : getDisplayList())
			{
				// check if player is farther than than the enemy
				auto PlayerToEnemyDistance = Util::distance(m_pMarvin->getTransform()->position, enemies->getTransform()->position);

				if (PlayerToEnemyDistance >= EnemyToPlayerDistance)
				{
					if ((object->getType() != enemies->getType()) && (object->getType() != object->getType()))
					{
						contactList.push_back(object);
					}
				}
			}
			contactList.push_back(object); // add the target to the end of the list
			auto hasDetection = CollisionManager::detectionCheck(enemies->getTransform()->position, enemies->getDetectionDistance(), contactList, object);

			enemies->setHasDetection(hasDetection);
		}
		else
		{
			enemies->setHasDetection(false);
		}
	}
		
}
/*
55
51
1 2 < > 1 3 2 1 4 2 1 2 1 3 1 2 4 2 1 1 1 3 3 4 2 1 1 3 1 2 1 4 3 < > 1 1 2 3 1 2 1 1 4 3 1 2 < > 1 . . . . .
2 . . . P . . . v . . . . . . . . . . . . v . . . . . . . . . v . . . . . . . . . . . v . . . . . 3 . . . . .
2 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 3 . . . . .
1 . . p P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 2 . . . . .
1 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 4 . . . . .
3 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . < . . . . .
1 . . - P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1 . . . . .
2 . . . P . . . . . . . . . . B . . E . . E . E . . E . E . . E . . E . E . . . . B . . . . . . . 2 . . . . .
1 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1 . . . . .
1 . . . P 1 1 1 1 4 - . . . - 2 1 4 1 1 3 1 2 1 1 4 2 3 1 2 2 1 3 < > < > T 1 2 < > 2 3 1 1 . . - T . . . . .
1 . . . P . . . . . . . . . . . . . P . . . . . . . . . . . . . . . . . P v v . . . . . . v . . . 1 . . . . .
4 . . . P . . . . . . . . . . . . . P . . . . . . . . . . . . . . . . . P . . . . . . . . . . . . 3 . . . . .
1 . . . P . . . . . . . . . . . . . P . . . . . . . . . . . . . . . . . P . . . . . . . . . . . . 1 . . . . .
3 . . . P . . . . . . . . . . . . . P . . . . . . . . . . . 2 3 P . . . P . . . . . . . . . . . . 2 . . . . .
1 . . . P . . . . . . . . . . . . . P . . . . . . . . . 3 2 T . P . . . P . . . . . . . . . . . . 1 . . . . .
1 . . . P . . . . . . - - - . . . . P . . . . . . . 3 2 T . . . P . . . P . . . . . . . . . . . . 2 . . . . .
4 . . . P . . . . . . . . . . . . . P . . . . . . 2 T . . . . . P . . . P . . . . . . . . . . . . 2 . . . . .
2 . . . P . . . . . . . . . . . . . P . . . . . . . . . . . . . P . . . T ^ 2 . . ^ 3 < ^ 1 > 2 ^ > . . . . .
1 . . . P B H . . . . . . . . . . B P . . . . . . . . . . . . . P . . . P v v . . 3 2 1 v v v v v T . . . . .
3 . . . P H B . . . . . . . . . B B P . . . . . . . . . . . . . P . . . P . . . . . . . . . . . . 4 . . . . .
1 . . . P 1 1 1 1 4 - . . . - 2 1 4 T . . . . . . . . . . . . . P . . . P . . . . . . . . . . . . 1 . . . . .
1 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . P . . . P . . . . . . . . . . . . 3 . . . . .
1 ^ . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . P . . . P . . . . . . . . . . . . 1 . . . . .
1 3 . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . P . . . P . . . . . . . . . . . . 1 . . . . .
1 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . P . . . P . . . . . . . . . . . . P . . . . .
3 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . P . . . P . . . . . . . . . . . . 1 . . . . .
3 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . P . . . P ^ ^ < > ^ ^ ^ 2 . . . ^ T . . . . .
4 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . P . . . P . . . . . . . . . . . . 1 . . . . .
1 . . . P ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ P . . . P . . . . . . . . . . . . 1 . . . . .
< . . . P 1 4 2 3 1 2 1 2 1 1 4 2 3 1 2 1 2 1 1 4 2 3 1 2 1 3 2 T . . . P . . . . . . . . . . . . 2 . . . . .
2 . . . P . . . . < > . . . . . . . < > . . . . . . . < > . . . . . . . P . . - - - - - - - - - - 4 . . . . .
1 . . . P . . . . v v . . . . . . . v v . . . . . . . v v . . . . . . . P . . . . . . . . . . . . 1 . . . . .
3 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . P . . . . . . . . . . . . 3 . . . . .
1 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . P . . . . . . . . . . . . 2 . . . . .
2 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . P - - - - - - - - - - . . 1 . . . . .
4 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . P D D B B C B B B B D D D 1 . . . . .
1 . . . P . . B . . . . . . . . . E . . . . . . . E . . . . . E . . . . P D B B B B B H B B B B D 1 . . . . .
2 . . . P . B B . . . . . . . < > . . . . < > . . . . . . < > . . . . . P D B C B B B B B H B B D 2 . . . . .
> . . ^ P 1 4 2 3 1 2 . . . . T 4 1 1 3 1 2 1 1 4 2 3 1 2 2 1 3 < > < > T D D D D D D D D D D D D 3 . . . . .
3 . . 3 P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1 . . . . .
1 . . . P . . E . . . . . E . . . E . . E . E . . . . . . . . . . . . . . Z . . . . . . . . . . . T . . . . .
2 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1 < > < > P
3 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . H H H P
4 . . . P N . . . . . H . . . . . . . . . . . . . . . . < > . . . . . . . . . . . . . . . . . . . . . - - - P
1 . . . P 1 4 2 3 1 2 1 2 1 1 4 2 3 1 2 1 2 1 4 2 3 1 2 1 T . . . . . . . . . . . . . . . . . . . . . . . . P
1 . . . P . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . d . P
1 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . P
2 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . P
4 - . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1 P 2 3 < > P
3 . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . 1 . . . . .
1 1 3 2 4 1 < P P P P P P 3 1 2 2 4 1 > < > 1 3 2 4 1 < > ^ ^ ^ ^ ^ ^ ^ ^ P 1 3 2 1 < > 1 2 3 4 2 T . . . . .
																			  

*/