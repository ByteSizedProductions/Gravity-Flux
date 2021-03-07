#include "Crate.h"
#include "TextureManager.h"
#include <algorithm>

Crate::Crate(glm::vec2 position)
{
	TextureManager::Instance()->load("../Assets/textures/Crate.png", "crate");
	
	auto size = TextureManager::Instance()->getTextureSize("crate");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = position;
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	m_isGrounded = false;
	m_turnAngle = 0.0f;
	m_turnRate = 9.0f;
	m_gravity = 12.0f;
	m_force = -20.0f;
	m_maxSpeed = 7.5f;
	setType(CRATE);
}

Crate::~Crate() = default;

void Crate::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("crate", x, y, 0, 255, false);
}

void Crate::update()
{
	updateGravity();
}

void Crate::clean()
{
}

void Crate::updateGravity()
{
	getRigidBody()->velocity.y += getRigidBody()->acceleration.y + m_gravity * 0.075f;
	getRigidBody()->velocity.y = std::min(std::max(getRigidBody()->velocity.y, m_force), m_gravity);
	getTransform()->position.y += getRigidBody()->velocity.y;
	getRigidBody()->acceleration.y = 0.0f;
}

void Crate::handleCollisions(GameObject* object)
{
	if (object->getType() == PLATFORM || object->getType() == CRATE) {
		//did crate collide with the top of the platform?
		if (round(getTransform()->position.y + getHeight() - getRigidBody()->velocity.y) <= round(object->getTransform()->position.y)) {
			setIsGrounded(true);
			getRigidBody()->velocity.y = 0.0f;
			getTransform()->position.y = object->getTransform()->position.y - getHeight();
		}
		else
			setIsGrounded(false);
		//did crate collide with the bottom of the platform?
		if ((getTransform()->position.y - getRigidBody()->velocity.y) >= (object->getTransform()->position.y + object->getHeight())) {
			getRigidBody()->velocity.y = 0.0f;
			getTransform()->position.y = object->getTransform()->position.y + object->getHeight();
		}
		//did crate collide with the left side of the platform?
		if ((getTransform()->position.x + getWidth() - getRigidBody()->velocity.x) <= object->getTransform()->position.x) {
			getRigidBody()->velocity.x = 0.0f;
			getTransform()->position.x = object->getTransform()->position.x - getWidth();
		}
		//did crate collide with the right side of the platform?
		if ((getTransform()->position.x - getRigidBody()->velocity.x) >= (object->getTransform()->position.x + object->getWidth())) {
			getRigidBody()->velocity.x = 0.0f;
			getTransform()->position.x = object->getTransform()->position.x + object->getWidth();
		}
	}
}

void Crate::setIsGrounded(bool grounded)
{
	m_isGrounded = grounded;
}

bool Crate::isGrounded()
{
	return m_isGrounded;
}
