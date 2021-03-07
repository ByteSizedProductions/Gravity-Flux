#include "Tile.h"
#include "Renderer.h"

Tile::Tile() {}

Tile::Tile(TileType type, SDL_Rect src, SDL_Color color)
{
	setWidth(40);
	setHeight(40);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(TILE);
	SetTileType(type);

	m_pRect = new SDL_Rect();
	m_pRect->w = 40;
	m_pRect->h = 40;

	m_color = color;
}

Tile::~Tile() = default;

void Tile::draw()
{
	m_pRect->x = getTransform()->position.x;
	m_pRect->y = getTransform()->position.y;
	SDL_SetRenderDrawColor(Renderer::Instance()->getRenderer(), m_color.r, m_color.g, m_color.b, m_color.a);
	SDL_RenderFillRect(Renderer::Instance()->getRenderer(), m_pRect);
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

SDL_Color Tile::GetColor()
{
	return m_color;
}
