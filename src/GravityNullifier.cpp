#include "GravityNullifier.h"
#include "Util.h"

GravityNullifier::GravityNullifier(TileType type, SDL_Rect* src) : Tile(type, src), m_effectRadius(300), 
m_lifespan_radius1(1.0f), m_lifespan_radius2(0.5f) {}

GravityNullifier::~GravityNullifier() {}

void GravityNullifier::draw()
{
	auto offset = glm::vec2(getWidth() / 2, getHeight() / 2);
	Tile::draw();

	Util::DrawCircle(getTransform()->position + offset, (m_effectRadius - (m_effectRadius * m_lifespan_radius1)), 
		glm::vec4(1.0f, 0.0f, 0.0f, m_lifespan_radius1));
	Util::DrawCircle(getTransform()->position + offset, (m_effectRadius - (m_effectRadius * m_lifespan_radius2)),
		glm::vec4(1.0f, 0.0f, 0.0f, m_lifespan_radius2));
}

void GravityNullifier::update()
{
	m_lifespan_radius1 -= 0.01f;
	m_lifespan_radius2 -= 0.01f;

	if (m_lifespan_radius1 <= 0.0f)
		m_lifespan_radius1 = 1.0f;

	if (m_lifespan_radius2 <= 0.0f)
		m_lifespan_radius2 = 1.0f;
}

void GravityNullifier::clean()
{
}

int GravityNullifier::getEffectRadius()
{
	return m_effectRadius;
}

void GravityNullifier::setEffectRadius(int radius)
{
	m_effectRadius = radius;
}
