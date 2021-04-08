#include "DestructibleTile.h"

DestructibleTile::DestructibleTile(TileType type, SDL_Rect* src): Tile(type, src)
{
}

DestructibleTile::~DestructibleTile()
= default;

void DestructibleTile::draw()
{
	Tile::draw();
}

void DestructibleTile::update()
{
}

void DestructibleTile::clean()
{
}
