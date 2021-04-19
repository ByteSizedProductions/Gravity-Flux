#pragma once
#ifndef __MARVIN__
#define __MARVIN__

#include "PhysicsObject.h"
#include "TextureManager.h"
#include "Health.h"
#include "PhysicsSprite.h"
#include "PlayerAnimationState.h"

class Marvin final : public PhysicsSprite
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
	bool isWithinGravityNullifier() const;
	bool isDead() const;

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
	void setIsWithinGravityNullifier(bool state);
	void setIsDead(bool state);
	
	//overriden from Physics due to different features
	void updateGravity() override;
	void handleCollisions(GameObject* object) override;

	// bomb stuff
	void setNumBombs(int numBombs);
	int getNumBombs() const;
	int getBombCooldown();
	void setBombCooldown(int cooldown);

	//Health
	int getHealthCount();
	void setHealthCount(int health);

	// Animations
	bool checkAnimationDone(std::string animation);
	int checkAnimationFrame();
	void setAnimationFrame(std::string animation, int frame);

	void setAnimationState(PlayerAnimationState state);
	PlayerAnimationState getAnimationState();

private:
	void m_buildAnimations();
	void m_checkBounds();
	void m_reset();

	PlayerAnimationState m_AnimationState;

	SDL_RendererFlip m_flip;

	Health* m_marvinHealth;
	
	bool m_isDead;

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
	bool m_isWithinGravityNullifier;

	// bombs
	int m_numBombs;
	int m_bombCooldown; // Using a cooldown until a keydown without repeat is made

	float m_currentAngle;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
};


#endif
