#include "Fireball.h"
#include "PlayScene.h"
#include "TextureManager.h"
#include <algorithm>

Fireball::Fireball(glm::vec2 position, glm::vec2 direction, float speed, float currentAngle, bool isBoss) : Sprite()
{
	TextureManager::Instance()->load("../Assets/textures/fire_ball2.png", "Fireball");
	auto size = TextureManager::Instance()->getTextureSize("Fireball");
	setWidth(size.x / 2);
	setHeight(size.y);

	getTransform()->position = position;
	getRigidBody()->velocity = glm::vec2(3.0f, 3.0f);
	getRigidBody()->acceleration = glm::vec2(2.0f, 2.0f);
	getRigidBody()->isColliding = false;
	setType(ENEMY);
	setMaxSpeed(speed);

	m_currentAngle = currentAngle; // current facing angle
	m_currentDirection = direction;
	m_isBoss = isBoss;
}

Fireball::~Fireball()
= default;

void Fireball::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y + 20;

	// draw Fire Enemy
	//TextureManager::Instance()->draw("FlameEnemy", x, y, m_currentAngle, 255, false, static_cast<SDL_RendererFlip>(m_direction
	if (!m_isBoss)
	{
		TextureManager::Instance()->drawFromSheet("Fireball", 0, 0, 41, 24, 161, 150, x, y, m_currentAngle, 255, false);
	}
	else
	{
		TextureManager::Instance()->drawFromSheet("Fireball", 0, 0, 64, 37, 161, 150, x, y, m_currentAngle, 255, false);
	}
	
}

void Fireball::update()
{
	move();
}

void Fireball::clean()
{
}

void Fireball::move()
{
	getRigidBody()->velocity.x = m_maxSpeed * m_currentDirection.x;
	getTransform()->position.x += getRigidBody()->velocity.x;
}

glm::vec2 Fireball::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 Fireball::getCurrentDirection() const
{
	return m_currentDirection;
}

bool Fireball::getDirection() const
{
	return m_direction;
}

float Fireball::getMaxSpeed() const
{
	return m_maxSpeed;
}

void Fireball::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;
}

void Fireball::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void Fireball::setDirection(bool direction)
{
	m_direction = direction;
}

void Fireball::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Fireball::setAcceleration(glm::vec2 acceleration)
{
	getRigidBody()->acceleration = acceleration;
}

void Fireball::setVelocity(glm::vec2 velocity)
{
	getRigidBody()->velocity = velocity;
}

void Fireball::setAngle(float angle)
{
	m_currentAngle = angle;
}

void Fireball::m_checkBounds()
{
	if (getTransform()->position.x > Config::SCREEN_WIDTH)
	{
		getTransform()->position = glm::vec2(0.0f, getTransform()->position.y);
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position = glm::vec2(800.0f, getTransform()->position.y);
	}

	if (getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 0.0f);
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 600.0f);
	}
}

void Fireball::m_reset()
{
	getRigidBody()->isColliding = false;
	getTransform()->position = glm::vec2(600.0f, 500.0f);
}