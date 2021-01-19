#include "Marvin.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"


Marvin::Marvin() : m_maxSpeed(10.0f)
{
	TextureManager::Instance()->load("../Assets/textures/marvin.png", "marvin");

	auto size = TextureManager::Instance()->getTextureSize("marvin");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(SHIP);

	m_currentHeading = 0.0f; // current facing angle
	m_currentDirection = glm::vec2(1.0f, 0.0f); // facing right
	m_turnRate = 5.0f; // 5 degrees per frame
}


Marvin::~Marvin()
= default;

void Marvin::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance()->draw("marvin", x, y, m_currentHeading, 255, true); //Ship from the first scene
}


void Marvin::update()
{
	
	move();
	m_checkBounds();
}



void Marvin::clean()
{
}

void Marvin::turnRight()
{
	m_currentHeading += m_turnRate;
	if (m_currentHeading >= 360)
	{
		m_currentHeading -= 360.0f;
	}
	m_changeDirection();
}

void Marvin::turnLeft()
{
	m_currentHeading -= m_turnRate;
	if (m_currentHeading < 0)
	{
		m_currentHeading += 360.0f;
	}

	m_changeDirection();
}

void Marvin::moveForward()
{
	getRigidBody()->velocity = m_currentDirection * m_maxSpeed;
}

void Marvin::moveBack()
{
	getRigidBody()->velocity = m_currentDirection * -m_maxSpeed;
}

void Marvin::move()
{
	getTransform()->position += getRigidBody()->velocity;
	getRigidBody()->velocity *= 0.9f;
}

glm::vec2 Marvin::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 Marvin::getCurrentDirection() const
{
	return m_currentDirection;
}

float Marvin::getMaxSpeed() const
{
	return m_maxSpeed;
}

void Marvin::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;

}

void Marvin::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void Marvin::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}



void Marvin::m_checkBounds()
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

void Marvin::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}

void Marvin::m_changeDirection()
{
	const auto x = cos(m_currentHeading * Util::Deg2Rad);
	const auto y = sin(m_currentHeading * Util::Deg2Rad);
	m_currentDirection = glm::vec2(x, y);

	glm::vec2 size = TextureManager::Instance()->getTextureSize("marvin");
}