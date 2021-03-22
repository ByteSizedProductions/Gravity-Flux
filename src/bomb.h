#pragma once
#ifndef __BOMB__
#define __BOMB__

#include "PhysicsSprite.h"
#include "SoundManager.h"

class Bomb final : public PhysicsSprite
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
	void setAnimationFrame(int frame);
	void handleCollisions(GameObject* object) override;
	void Explode();

	//getters and setters
	int getDamage() const;
	void setCurrentDirection(glm::vec2 direction);

private:
	void m_buildAnimations();
	void m_move();
	int m_totalFrames;
	int m_damage;

	//physics attributes
	float m_maxSpeed;
	float m_turnAngle;
	float m_turnRate;
	glm::vec2 m_currentDirection;
};

#endif /* defined (__BOMB__) */
