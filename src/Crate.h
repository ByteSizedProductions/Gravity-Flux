#pragma once
#ifndef __CRATE_H__
#define __CRATE_H__
#include "PhysicsTile.h"
#include "SDL.h"

class Crate : public PhysicsTile {
public:
	Crate(TileType type, SDL_Rect* src);
	~Crate();

	void draw() override;
	void update() override;
	void clean() override;

private:
	SDL_Rect* m_pSrc;
};

#endif // !__CRATE_H__
