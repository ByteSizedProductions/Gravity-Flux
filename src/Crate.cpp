#include "Crate.h"
#include "TextureManager.h"
#include "Tile.h"

Crate::Crate(TileType type, SDL_Rect* src) : PhysicsTile(type, src) {}

Crate::~Crate() = default;

void Crate::draw()
{
	Tile::draw();
}

void Crate::update()
{
	Tile::update();
	updateGravity();
}

void Crate::clean()
{
	Tile::clean();
}
