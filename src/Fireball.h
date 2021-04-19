#pragma once
#ifndef __FIREBALL__
#define __FIREBALL__

#include "PhysicsObject.h"
#include "TextureManager.h"
#include "Sprite.h"

class Fireball : public Sprite
{
public:
	Fireball(glm::vec2 position, glm::vec2 direction, float speed, float currentAngle, bool isBoss = false);
	~Fireball();

	void draw() override;
	void update() override;
	void clean() override;

	void move();

	glm::vec2 getTargetPosition() const;
	glm::vec2 getCurrentDirection() const;
	bool getDirection() const;
	float getMaxSpeed() const;
	

	void setTargetPosition(glm::vec2 newPosition);
	void setCurrentDirection(glm::vec2 newDirection);
	void setDirection(bool direction);
	void setMaxSpeed(float newSpeed);
	void setAcceleration(glm::vec2 acceleration);
	void setVelocity(glm::vec2 velocity);
	void setAngle(float angle);
	



private:
	void m_checkBounds();
	void m_reset();

	// steering behaviours
	float m_maxSpeed;
	float m_turnRate;

	//physics behaviours
	float m_drag;
	bool m_direction;
	bool m_isBoss;

	float m_currentAngle;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;

	
};

#endif 