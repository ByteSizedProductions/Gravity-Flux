#include "PhysicsObject.h"
#include "Tile.h"
#include <algorithm>

PhysicsObject::PhysicsObject() : m_gravity(12.0f), m_force(-20.0f), m_isGrounded(false) {}

PhysicsObject::~PhysicsObject() = default;

void PhysicsObject::updateGravity()
{
	getRigidBody()->velocity.y += getRigidBody()->acceleration.y + m_gravity * 0.075f;
	getRigidBody()->velocity.y = std::min(std::max(getRigidBody()->velocity.y, m_force), m_gravity);
	getTransform()->position.y += getRigidBody()->velocity.y;
	getRigidBody()->acceleration.y = 0.0f;
}

void PhysicsObject::handleCollisions(GameObject* object)
{
	if (object->getType() == CRATE || (object->getType() == TILE &&
		(static_cast<Tile*>(object)->GetTileType() == GROUND) || static_cast<Tile*>(object)->GetTileType() == PLATFORM)) {
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

void PhysicsObject::setIsGrounded(bool grounded)
{
	m_isGrounded = grounded;
}

bool PhysicsObject::isGrounded()
{
	return m_isGrounded;
}

float PhysicsObject::getGravity()
{
	return m_gravity;
}

void PhysicsObject::setGravity(float gravity)
{
	m_gravity = gravity;
}

float PhysicsObject::getForce()
{
	return m_force;
}

void PhysicsObject::setForce(float force)
{
	m_force = force;
}