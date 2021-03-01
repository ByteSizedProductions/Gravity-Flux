#include "Platform.h"
#include "Renderer.h"

Platform::Platform(glm::vec2 position, int width, int height)
{
	setWidth(width);
	setHeight(height);
	getTransform()->position = position;
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLATFORM);

	m_rect = new SDL_Rect();
	m_rect->x = position.x;
	m_rect->y = position.y;
	m_rect->w = width;
	m_rect->h = height;
}

Platform::~Platform() = default;

void Platform::draw()
{
	m_rect->x = getTransform()->position.x;
	m_rect->y = getTransform()->position.y;
	SDL_RenderFillRect(Renderer::Instance()->getRenderer(), m_rect);
	
}

void Platform::update()
{
}

void Platform::clean()
{
}
