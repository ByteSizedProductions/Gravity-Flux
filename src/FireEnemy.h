#pragma once
#ifndef __FIREENEMY__
#define __FIREENEMY__

#include "PhysicsObject.h"
#include "TextureManager.h"
#include "Fireball.h"
#include "vector"
#include "glm/vec4.hpp"

class FireEnemy : public PhysicsObject
{
public:
	FireEnemy();
	~FireEnemy();

	void draw() override;
	void update() override;
	void clean() override;

	void move();

	glm::vec2 getTargetPosition() const;
	glm::vec2 getCurrentDirection() const;
	bool getDirection() const;
	float getMaxSpeed() const;
	bool isMoving() const;
	bool getFireBallActive() const;
	float getDetectionDistance() const;
	bool hasDetection() const;
	glm::vec4 getDetectionColor() const;

	void setTargetPosition(glm::vec2 newPosition);
	void setCurrentDirection(glm::vec2 newDirection);
	void setDirection(bool direction);
	void setMaxSpeed(float newSpeed);
	void setAcceleration(glm::vec2 acceleration);
	void setVelocity(glm::vec2 velocity);
	void setAngle(float angle);
	void setIsMoving(bool moving);
	void setFireBallActive(bool state);
	void setDetectionDistance(float distance);
	void setHasDetection(bool state);
	void setDetectionColor(glm::vec4 colour);
	std::vector<Fireball*> m_pFireballs;
	

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

	float m_currentAngle;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
	bool m_fireBallActive;

	//Detection Radius
	float m_DetectionDistance;
	bool m_hasDetection;
	glm::vec4 m_DetectionColour;
};

#endif 
