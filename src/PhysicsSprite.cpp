#include "PhysicsSprite.h"
#include "Tile.h"

PhysicsSprite::PhysicsSprite() : m_gravity(12.0f), m_force(-20.0f), m_isGrounded(false) {}

PhysicsSprite::~PhysicsSprite() = default;

void PhysicsSprite::updateGravity()
{
	getRigidBody()->velocity.y += getRigidBody()->acceleration.y + m_gravity * 0.075f;
	getRigidBody()->velocity.y = std::min(std::max(getRigidBody()->velocity.y, m_force), m_gravity);
	getTransform()->position.y += getRigidBody()->velocity.y;
	getRigidBody()->acceleration.y = 0.0f;
}

void PhysicsSprite::handleCollisions(GameObject* object)
{
	if ((object->getType() == TILE && (static_cast<Tile*>(object)->GetTileType() == GROUND) || (static_cast<Tile*>(object)->GetTileType() == PLATFORM)
		|| (static_cast<Tile*>(object)->GetTileType() == CRATE) || (static_cast<Tile*>(object)->GetTileType() == DAMAGING))) {
		//did player collide with the top of the platform?
		if (round(getTransform()->position.y + getHeight() - getRigidBody()->velocity.y) <= round(object->getTransform()->position.y)) {
			setIsGrounded(true);
			getRigidBody()->velocity.y = 0.0f;
			getTransform()->position.y = object->getTransform()->position.y - getHeight();
		}
		//did the player collide with the bottom of the platform?
		else if (round(getTransform()->position.y - getRigidBody()->velocity.y) >= round(object->getTransform()->position.y + object->getHeight())) {
			getRigidBody()->velocity.y = 0.0f;
			getTransform()->position.y = object->getTransform()->position.y + object->getHeight();
		}
		//did the player collide with the left side of the platform?
		else if (round(getTransform()->position.x + getWidth() - getRigidBody()->velocity.x) <= round(object->getTransform()->position.x)) {
			getRigidBody()->velocity.x = 0.0f;
			getTransform()->position.x = object->getTransform()->position.x - getWidth();
		}
		//did the player cllide with the right side of the platform?
		else if (round(getTransform()->position.x - getRigidBody()->velocity.x) >= round(object->getTransform()->position.x + object->getWidth())) {
			getRigidBody()->velocity.x = 0.0f;
			getTransform()->position.x = object->getTransform()->position.x + object->getWidth();
		}
	}
}

void PhysicsSprite::setIsGrounded(bool grounded)
{
	m_isGrounded = grounded;
}

bool PhysicsSprite::isGrounded()
{
	return m_isGrounded;
}

float PhysicsSprite::getGravity()
{
	return m_gravity;
}

void PhysicsSprite::setGravity(float gravity)
{
	m_gravity = gravity;
}

float PhysicsSprite::getForce()
{
	return m_force;
}

void PhysicsSprite::setForce(float force)
{
	m_force = force;
}
