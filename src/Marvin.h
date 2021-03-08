#pragma once
#ifndef __MARVIN__
#define __MARVIN__

#include "PhysicsObject.h"
#include "TextureManager.h"

class Marvin final : public PhysicsObject
{
public:
	Marvin();
	~Marvin();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;

	void jump();
	void move();

	// getters
	glm::vec2 getTargetPosition() const;
	glm::vec2 getCurrentDirection() const;
	bool getDirection() const;
	float getMaxSpeed() const;
	bool isGravityFlipped() const;
	bool isMoving() const;
	int getGravityCooldown() const;

	// setters
	void setTargetPosition(glm::vec2 newPosition);
	void setCurrentDirection(glm::vec2 newDirection);
	void setDirection(bool direction);
	void setMaxSpeed(float newSpeed);
	void setAcceleration(glm::vec2 acceleration);
	void setVelocity(glm::vec2 velocity);
	void setGravityFlipped(bool flipped);
	void setAngle(float angle);
	void setIsMoving(bool moving);
	void setGravityCooldown(int cooldown);
	void ChangeDirection();
	
	//overriden from Physics due to different features
	void updateGravity() override;
	void handleCollisions(GameObject* object) override;

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
	float m_drag;
	bool m_isMoving;
	bool m_direction;
	
	//Gravity stuff
	bool m_isGravityFlipped;
	int m_gravityCooldown;

	// bombs
	int m_numBombs;
	int m_bombCooldown; // Using a cooldown until a keydown without repeat is made

	float m_currentAngle;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
};


#endif
