#pragma once
#ifndef __MARVIN__
#define __MARVIN__

#include "bomb.h"
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
	bool getDirection() const;
	float getMaxSpeed() const;
	bool isGravityFlipped() const;
	int getGravityCooldown() const;

	// setters
	void setTargetPosition(glm::vec2 newPosition);
	void setCurrentDirection(glm::vec2 newDirection);
	void setDirection(bool direction);
	void setMaxSpeed(float newSpeed);
	void setAcceleration(glm::vec2 acceleration);
	void setVelocity(glm::vec2 velocity);
	void setJumpForce(float jumpForce);
	void setGravity(float gravity);
	void setGravityFlipped(bool flipped);
	void setAngle(float angle);
	void updateGravity();
	void setIsGrounded(bool grounded);
	void handleCollisions(GameObject* object);
	void setGravityCooldown(int cooldown);
	bool isGrounded();
	
	// bomb stuff
	void setNumBombs(int numBombs);
	int getNumBombs() const;
	int getBombCooldown();
	void setBombCooldown(int cooldown);

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
	
	//Gravity shit
	bool m_isGravityFlipped;
	int m_gravityCooldown;

	// bombs
	int m_numBombs;
	int m_bombCooldown; // Using a cooldown until a keydown without repeat is made

	void m_changeDirection();
	float m_currentAngle;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
};


#endif
