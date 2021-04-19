#include "bomb.h"
#include "Player.h"
#include "TextureManager.h"
#include "Tile.h"
#include <string>
#include <algorithm>

Bomb::Bomb(glm::vec2 position, glm::vec2 direction) : PhysicsSprite()
{
	TextureManager::Instance()->load("../Assets/textures/bomb.png", "bomb");
	TextureManager::Instance()->loadSpriteSheet("../Assets/sprites/bomb_explosion.txt", "../Assets/sprites/bomb_explosion.png", "bombs");
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("bombs"));
	SoundManager::Instance().load("../Assets/audio/explosion_1.mp3", "explosion1", SOUND_SFX);


	// set frame width
	setWidth(32);

	// set frame height
	setHeight(32);
	
	getTransform()->position = position;
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, (-13.0f * direction.y));
	getRigidBody()->isColliding = false;
	m_damage = 1;
	m_turnAngle = 0.0f;
	m_turnRate = 9.0f;
	m_force = -13.0f;
	m_maxSpeed = 7.5f;
	m_currentDirection = direction;
	setType(BOMB);
	
	m_buildAnimations();
}

Bomb::~Bomb()
= default;

void Bomb::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the bomb with explosion animation
	if (abs(getRigidBody()->velocity.x) < 1 && abs(getRigidBody()->velocity.y) < 1)
		TextureManager::Instance()->playAnimationOnce("bombs", getAnimation("bomb"), x, y, 4.0f, m_turnAngle, 255, false);
	else
		TextureManager::Instance()->draw("bomb", x, y, m_turnAngle, 255, false);
}

void Bomb::update()
{
	//if bomb is exploding, gravity and movement don't need to be calculated
	if (checkAnimationFrame() < 10) {
		m_move();
		updateGravity();
	}
	else if (checkAnimationFrame() == 10)
		Explode();

	if (m_isGrounded) {
		m_maxSpeed = std::max(0.0f, m_maxSpeed - 1.0f);
		m_turnRate = std::max(0.0f, m_turnRate - 0.9f);
	}
}

void Bomb::m_move() {
	getRigidBody()->velocity.x = m_maxSpeed * m_currentDirection.x;

	if (m_currentDirection.x == 1.0)
		m_turnAngle += int(m_turnRate) % 360;
	else if (m_currentDirection.x == -1.0) {
		if (m_turnAngle <= 0)
			m_turnAngle += 360;
		m_turnAngle -= m_turnRate;
	}
	getTransform()->position.x += getRigidBody()->velocity.x;
}

void Bomb::clean()
{
}

int Bomb::getDamage() const
{
	return m_damage;
}

bool Bomb::checkAnimationDone()
{
	if (TextureManager::Instance()->checkAnimationDone(getAnimation("bomb")))
		return true;
	else
		return false;
	
}

int Bomb::checkAnimationFrame()
{
	return TextureManager::Instance()->checkAnimationFrame(getAnimation("bomb"));
}

void Bomb::setAnimationFrame(int frame)
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->setAnimationFrame(getAnimation("bomb"), frame);
}

void Bomb::handleCollisions(GameObject* object)
{
	if ((object->getType() == TILE && (static_cast<Tile*>(object)->GetTileType() == GROUND) || (static_cast<Tile*>(object)->GetTileType() == PLATFORM)
		|| (static_cast<Tile*>(object)->GetTileType() == CRATE) || (static_cast<Tile*>(object)->GetTileType() == DAMAGING))) {
		//did bomb collide with the top of the platform?
		if (round(getTransform()->position.y + getHeight() - getRigidBody()->velocity.y) <= round(object->getTransform()->position.y)) {
			setIsGrounded(true);
			getRigidBody()->velocity.y = 0.0f;
			getTransform()->position.y = object->getTransform()->position.y - getHeight();
		}
		//did bomb collide with the bottom of the platform?
		else if (round(getTransform()->position.y - getRigidBody()->velocity.y) >= round(object->getTransform()->position.y + object->getHeight())) {
			getRigidBody()->velocity.y = 0.0f;
			getTransform()->position.y = object->getTransform()->position.y + object->getHeight();
		}
		//did bomb collide with the left side of the platform?
		else if (round(getTransform()->position.x + getWidth() - getRigidBody()->velocity.x) <= round(object->getTransform()->position.x)) {
			getRigidBody()->velocity.x = 0.0f;
			getTransform()->position.x = object->getTransform()->position.x - getWidth();
			m_maxSpeed = 0.0f;
		}
		//did bomb collide with the right side of the platform?
		else if (round(getTransform()->position.x - getRigidBody()->velocity.x) >= round(object->getTransform()->position.x + object->getWidth())) {
			getRigidBody()->velocity.x = 0.0f;
			getTransform()->position.x = object->getTransform()->position.x + object->getWidth();
			m_maxSpeed = 0.0f;
		}
	}
}

void Bomb::setCurrentDirection(glm::vec2 direction)
{
	m_currentDirection = direction;
}

void Bomb::m_buildAnimations()
{
	Animation bombAnimation = Animation();	
	bombAnimation.name = "bomb";

	for (int i = 0; i < 10; i++)
		bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb"));
	for (int i = 1; i <= 22; i++)
		bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion" + std::to_string(i)));

	setAnimation(bombAnimation);
	m_totalFrames = bombAnimation.frames.size();
}

void Bomb::Explode()
{
	getRigidBody()->velocity.x = 0.0f;
	getRigidBody()->velocity.y = 0.0f;
	m_maxSpeed = 0;
	//SoundManager::Instance().setSFXVolume("explosion1", 10);
	Mix_Volume(5, 100);
	int j = rand() % 3;
	if (j == 0)
		SoundManager::Instance().playSound("explosion1", 0, 5);
	else if (j == 1)
		SoundManager::Instance().playSound("explosion1", 0, 5);
	else if (j == 2)
		SoundManager::Instance().playSound("explosion1", 0, 5);
}
