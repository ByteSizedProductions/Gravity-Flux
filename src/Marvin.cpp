#include "Marvin.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"
#include <algorithm>


Marvin::Marvin() : m_maxSpeed(7.5f) , PhysicsObject()
{
	TextureManager::Instance()->load("../Assets/textures/marvin.png", "marvin");

	auto size = TextureManager::Instance()->getTextureSize("marvin");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(400.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER);

	setIsGrounded(true);
	m_currentAngle = 0.0f; // current facing angle
	m_currentDirection = glm::vec2(1.0f, 1.0f); // facing right, gravity is down
	m_turnRate = 5.0f; // 5 degrees per frame
	m_drag = 0.88f;
	m_direction = 0;
	m_gravityCooldown = 0;
}


Marvin::~Marvin()
= default;

void Marvin::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw Marvin
	TextureManager::Instance()->draw("marvin", x, y, m_currentAngle, 255, false, static_cast<SDL_RendererFlip>(m_direction)); //Ship from the first scene
}

void Marvin::update()
{
	if (m_isMoving)
		move();
	updateGravity();
	m_checkBounds();

	//TEMPORARY
	if (m_gravityCooldown > 0)
		m_gravityCooldown--;

	if (getBombCooldown() > 0)
		setBombCooldown(getBombCooldown() - 1);
}

void Marvin::updateGravity()
{
	getRigidBody()->velocity.y += getRigidBody()->acceleration.y + m_gravity * 0.075f;
	if (!m_isGravityFlipped)
		getRigidBody()->velocity.y = std::min(std::max(getRigidBody()->velocity.y, m_force), m_gravity);
	else
		getRigidBody()->velocity.y = std::max(std::min(getRigidBody()->velocity.y, m_force), m_gravity);
	getTransform()->position.y += getRigidBody()->velocity.y;
	getRigidBody()->acceleration.y = 0.0f;
}

void Marvin::handleCollisions(GameObject* object)
{
	if (object->getType() == CRATE || (object->getType() == TILE &&  
		(static_cast<Tile*>(object)->GetTileType() == GROUND) || static_cast<Tile*>(object)->GetTileType() == PLATFORM)) {
		//did player collide with the top of the platform?
		if (round(getTransform()->position.y + getHeight() - getRigidBody()->velocity.y) <= round(object->getTransform()->position.y)) {
			if (!m_isGravityFlipped)
				setIsGrounded(true);
			else
				setIsGrounded(false);
			getRigidBody()->velocity.y = 0.0f;
			getTransform()->position.y = object->getTransform()->position.y - getHeight();
		}
		//did the player collide with the bottom of the platform?
		else if (round(getTransform()->position.y - getRigidBody()->velocity.y) >= round(object->getTransform()->position.y + object->getHeight())) {
			if (m_isGravityFlipped)
				setIsGrounded(true);
			else
				setIsGrounded(false);
			getRigidBody()->velocity.y = 0.0f;
			getTransform()->position.y = object->getTransform()->position.y + object->getHeight();
		}
		//did the player collide with the left side of the platform?
		else if (round(getTransform()->position.x + getWidth() - getRigidBody()->velocity.x) <= round(object->getTransform()->position.x)) {
			getRigidBody()->velocity.x = 0.0f;
			getTransform()->position.x = object->getTransform()->position.x - getWidth() - 1;
		}
		//did the player cllide with the right side of the platform?
		else if (round(getTransform()->position.x - getRigidBody()->velocity.x) >= round(object->getTransform()->position.x + object->getWidth())) {
			getRigidBody()->velocity.x = 0.0f;
			getTransform()->position.x = object->getTransform()->position.x + object->getWidth() + 1;
		}
	}
}

void Marvin::clean()
{
}

void Marvin::jump()
{
	if (m_isGrounded)
	{
		m_isGrounded = false;
		getRigidBody()->acceleration.y = m_force;
	}
	else
		getRigidBody()->acceleration.y = 0.0f;
}

void Marvin::move()
{
	getRigidBody()->velocity.x = m_currentDirection.x * m_maxSpeed;
	getTransform()->position.x += getRigidBody()->velocity.x;
	if (m_currentDirection.x == 1.0f)
		m_direction = m_isGravityFlipped ? 1 : 0;
	else if (m_currentDirection.x == -1.0f)
		m_direction = m_isGravityFlipped ? 0 : 1;
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

bool Marvin::getDirection() const
{
	return m_direction;
}

bool Marvin::isGravityFlipped() const
{
	return m_isGravityFlipped;
}

bool Marvin::isMoving() const
{
	return m_isMoving;
}

int Marvin::getGravityCooldown() const
{
	return m_gravityCooldown;
}

void Marvin::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;
}

void Marvin::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void Marvin::setDirection(bool direction)
{
	m_direction = direction;
}

void Marvin::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Marvin::setAcceleration(glm::vec2 acceleration)
{
	getRigidBody()->acceleration = acceleration;
}

void Marvin::setVelocity(glm::vec2 velocity)
{
	getRigidBody()->velocity = velocity;
}

void Marvin::setGravityFlipped(bool flipped)
{
	m_isGravityFlipped = flipped;
}

void Marvin::setAngle(float angle) {
	m_currentAngle = angle;
}

void Marvin::setIsMoving(bool moving) {
	m_isMoving = moving;
}

void Marvin::setNumBombs(int numBombs)
{
	m_numBombs = numBombs;
}

int Marvin::getNumBombs() const
{
	return m_numBombs;
}

int Marvin::getBombCooldown()
{
	return m_bombCooldown;
}

void Marvin::setBombCooldown(int cooldown)
{
	m_bombCooldown = cooldown;
}

void Marvin::setGravityCooldown(int cooldown)
{
	m_gravityCooldown = cooldown;
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

void Marvin::ChangeDirection()
{
	if (m_currentDirection.x == 1.0f)
		m_direction = m_isGravityFlipped ? 1 : 0;
	else if (m_currentDirection.x == -1.0f)
		m_direction = m_isGravityFlipped ? 0 : 1;
}