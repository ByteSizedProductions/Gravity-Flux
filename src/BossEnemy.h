#pragma once
#pragma once
#ifndef __BOSS_ENEMY__
#define __BOSS_ENEMY__

#include "PhysicsObject.h"
#include "PhysicsSprite.h"
#include "BossAnimationState.h"
#include "TextureManager.h"
#include "Fireball.h"

class BossEnemy : public PhysicsSprite
{
public:
	BossEnemy();
	~BossEnemy();

	void draw() override;
	void update() override;
	void clean() override;

	void move();

	glm::vec2 getTargetPosition() const;
	glm::vec2 getCurrentDirection() const;
	bool getDirection() const;
	float getMaxSpeed() const;
	bool isMoving() const;
	void isBossDead();

	void setTargetPosition(glm::vec2 newPosition);
	void setCurrentDirection(glm::vec2 newDirection);
	void setDirection(bool direction);
	void setMaxSpeed(float newSpeed);
	void setAcceleration(glm::vec2 acceleration);
	void setVelocity(glm::vec2 velocity);
	void setAngle(float angle);
	void setIsMoving(bool moving);
	void Attack();
	bool isFloorSpikes();
	int getAttackCoolDown();
	void setIsBossDead(bool isBossDead);

	// Animations
	bool checkAnimationDone(std::string animation);
	int checkAnimationFrame();
	void setAnimationFrame(std::string animation, int frame);

	void setAnimationState(BossAnimationState state);
	BossAnimationState getAnimationState();

private:
	// Animations
	void m_buildAnimations();
	BossAnimationState m_AnimationState;
	
	void m_checkBounds();
	void m_reset();

	int m_attackCoolDown = 0;
	bool m_floorSpikes = true;
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
};

#endif 

