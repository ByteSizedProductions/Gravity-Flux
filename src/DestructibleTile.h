#pragma once
#ifndef __DESTRUCTIBLE_TILE__
#define __DESTRUCTIBLE_TILE__
#include "Tile.h"

class DestructibleTile : public Tile {
public:
	DestructibleTile(TileType type, SDL_Rect* src);
	~DestructibleTile();

	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
private:
};

#endif // !__DESTRUCTIBLE_TILE__