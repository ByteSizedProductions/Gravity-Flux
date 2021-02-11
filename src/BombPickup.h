#pragma once
#ifndef __BOMB_PICKUP__
#define __BOMB_PICKUP__
#include "Pickup.h"

class BombPickup final : public Pickup
{
public:
	BombPickup();
	~BombPickup();
	
	// Life-Cycle functions
	void draw() override;
	void update() override;
	void clean() override;

private:
	
};

#endif /*DEFINED __BOMB_PICKUP__ */
