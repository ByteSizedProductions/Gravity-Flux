#include "Crate.h"
#include "TextureManager.h"
#include "Tile.h"

Crate::Crate(glm::vec2 position, SDL_Rect* src) : PhysicsObject()
{
	TextureManager::Instance()->load("../Assets/textures/Tile_Sheet.png", "tiles");

	setWidth((src->w * 40) / 256);
	setHeight((src->h * 40) / 256);

	getTransform()->position = position;
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(CRATE);

	m_pSrc = src;
}

Crate::~Crate() = default;

void Crate::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	TextureManager::Instance()->drawFromSheet("tiles", m_pSrc->x, m_pSrc->y, m_pSrc->w, m_pSrc->h, 256, 40, x, y, 0, 255, false);
}

void Crate::update()
{
	updateGravity();
}

void Crate::clean()
{
}
