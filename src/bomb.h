#pragma once
#ifndef __BOMB__
#define __BOMB__

#include "Sprite.h"

class Bomb final : public Sprite
{
public:
	Bomb(glm::vec2 position);
	~Bomb();

	// Life Cycle Functions
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

private:
	void m_buildAnimations();
};

#endif /* defined (__BOMB__) */
