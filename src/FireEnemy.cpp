#include "FireEnemy.h"
#include "PlayScene.h"
#include "TextureManager.h"
#include <algorithm>

FireEnemy::FireEnemy() : PhysicsObject()
{
	TextureManager::Instance()->load("../Assets/textures/Flame_enemy.png", "FlameEnemy");
	auto size = TextureManager::Instance()->getTextureSize("FlameEnemy");
	setWidth((size.x * 111.5) / 223);
	setHeight((size.y * 111.5) / 223);

	getTransform()->position = glm::vec2(200.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(ENEMY);

	setIsGrounded(true);
	m_currentAngle = 0.0f; // current facing angle
	m_currentDirection = glm::vec2(1.0f, 1.0f); 
	m_direction = 0;
}

FireEnemy::~FireEnemy()
= default;

void FireEnemy::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw Fire Enemy
	//TextureManager::Instance()->draw("FlameEnemy", x, y, m_currentAngle, 255, false, static_cast<SDL_RendererFlip>(m_direction));
	TextureManager::Instance()->drawFromSheet("FlameEnemy", 0, 0, 161, 223, 223, 111.5, x, y, 0, 255, false);
}

void FireEnemy::update()
{
	if (m_isMoving)
		move();
	updateGravity();
	//m_checkBounds();
}

void FireEnemy::clean()
{
}

void FireEnemy::move()
{
}

glm::vec2 FireEnemy::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 FireEnemy::getCurrentDirection() const
{
	return m_currentDirection;
}

bool FireEnemy::getDirection() const
{
	return m_direction;
}

float FireEnemy::getMaxSpeed() const
{
	return m_maxSpeed;
}

bool FireEnemy::isMoving() const
{
	return m_isMoving;
}

void FireEnemy::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;
}

void FireEnemy::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void FireEnemy::setDirection(bool direction)
{
	m_direction = direction;
}

void FireEnemy::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void FireEnemy::setAcceleration(glm::vec2 acceleration)
{
	getRigidBody()->acceleration = acceleration;
}

void FireEnemy::setVelocity(glm::vec2 velocity)
{
	getRigidBody()->velocity = velocity;
}

void FireEnemy::setAngle(float angle)
{
	m_currentAngle = angle;
}

void FireEnemy::setIsMoving(bool moving)
{
	m_isMoving = moving;
}



void FireEnemy::m_checkBounds()
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

void FireEnemy::m_reset()
{
	getRigidBody()->isColliding = false;
	getTransform()->position = glm::vec2(600.0f, 500.0f);
}
