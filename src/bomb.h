#pragma once
#ifndef __BOMB__
#define __BOMB__

#include "Sprite.h"

class Bomb final : public Sprite
{
public:
	Bomb(glm::vec2 position, glm::vec2 direction);
	~Bomb();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	
	bool checkAnimationDone();
	int checkAnimationFrame();
	void handleCollisions(GameObject* object);

	//getters and setters
	int getDamage() const;
	void setCurrentDirection(glm::vec2 direction);
	void setIsGrounded(bool grounded);

private:
	void m_buildAnimations();
	void m_move();
	void m_updateGravity();
	int m_totalFrames;
	int m_damage;

	//physics attributes
	float m_throwForce;
	float m_gravity;
	float m_maxSpeed;
	bool m_isGrounded;
	float m_turnAngle;
	float m_turnRate;
	glm::vec2 m_currentDirection;
};

#endif /* defined (__BOMB__) */
