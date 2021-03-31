#pragma once
#ifndef __GRAVITY_NULLIFIER__
#define __GRAVITY_NULLIFIER__
#include "Tile.h"

class GravityNullifier : public Tile {
public:
	GravityNullifier(TileType type, SDL_Rect* src);
	~GravityNullifier();

	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;

	int getEffectRadius();
	void setEffectRadius(int radius);

private:
	int m_effectRadius;
	double m_lifespan_radius1;
	double m_lifespan_radius2;
};

#endif // !__GRAVITY_NULLIFIER__
