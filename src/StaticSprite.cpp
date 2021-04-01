#include "StaticSprite.h"

#include "TextureManager.h"

StaticSprite::StaticSprite(std::string imagePath, std::string id)
{
	TextureManager::Instance()->load(imagePath, id);
	auto size = TextureManager::Instance()->getTextureSize(id);
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(0.0f, 0.0f);
	setType(MENU);
	getRigidBody()->isColliding = false;
	m_id = id;
}

StaticSprite::~StaticSprite()
= default;

void StaticSprite::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw(m_id, getTransform()->position.x, getTransform()->position.y, 0, 255, false);
}

void StaticSprite::update()
{
}

void StaticSprite::clean()
{
}
