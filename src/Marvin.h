#pragma once
#ifndef __MARVIN__
#define __MARVIN__

#include "DisplayObject.h"
#include "TextureManager.h"

class Marvin final : public DisplayObject
{
public:
	Marvin();
	~Marvin();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void turnRight();
	void turnLeft();
	void moveForward();
	void moveBack();
	void jump();

	void move();

	// getters
	glm::vec2 getTargetPosition() const;
	glm::vec2 getCurrentDirection() const;
	float getMaxSpeed() const;

	// setters
	void setTargetPosition(glm::vec2 newPosition);
	void setCurrentDirection(glm::vec2 newDirection);
	void setMaxSpeed(float newSpeed);
	void setAcceleration(glm::vec2 acceleration);
	void setVelocity(glm::vec2 velocity);
	void setJumpForce(float jumpForce);
	void setGravity(float gravity);
	void updateGravity();
	void setIsGrounded(bool grounded);
	void handleCollisions(GameObject* object);
	bool isGrounded();

private:
	void m_checkBounds();
	void m_reset();

	// steering behaviours
	float m_maxSpeed;
	float m_turnRate;

	//physics behaviours
	float m_jumpForce;
	float m_gravity;
	float m_drag;
	bool m_isGrounded;
	bool m_direction;

	void m_changeDirection();
	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
};


#endif
