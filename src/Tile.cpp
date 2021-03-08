#include "Tile.h"
#include "Renderer.h"
#include "TextureManager.h"

Tile::Tile() {}

Tile::Tile(TileType type, SDL_Rect* src)
{
	TextureManager::Instance()->load("../Assets/textures/Tile_Sheet.png", "tiles");

	setWidth((src->w * 40) / 256);
	setHeight((src->h * 40) / 256);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(TILE);
	SetTileType(type);

	m_pSrc = src;
}

Tile::~Tile() = default;

void Tile::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	TextureManager::Instance()->drawFromSheet("tiles", m_pSrc->x, m_pSrc->y, m_pSrc->w, m_pSrc->h, 256, 40, x, y, 0, 255, false);
}

void Tile::update()
{
}

void Tile::clean()
{
}

TileType Tile::GetTileType()
{
	return m_type;
}

void Tile::SetTileType(TileType type)
{
	m_type = type;
}

SDL_Rect* Tile::GetSource()
{
	return m_pSrc;
}
