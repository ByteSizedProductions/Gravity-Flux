#include "Health.h"
#include "TextureManager.h"


Health::Health():m_healthCount(3)
{
	TextureManager::Instance()->load("../Assets/textures/heart.png", "heart");
	auto size = TextureManager::Instance()->getTextureSize("heart");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(300.0f, 300.0f);
	setType(HEART);
}

void Health::draw()
{
	if(m_healthCount >= 1)
		TextureManager::Instance()->draw("heart", getTransform()->position.x - 5, getTransform()->position.y, 0, 255, false);
	if (m_healthCount >= 2)
		TextureManager::Instance()->draw("heart", getTransform()->position.x + getWidth(), getTransform()->position.y, 0, 255, false);
	if (m_healthCount == 3)
		TextureManager::Instance()->draw("heart", getTransform()->position.x + (getWidth()* 2) + 5, getTransform()->position.y, 0, 255, false);
}

void Health::update()
{
}

void Health::clean()
{
}

int Health::getHealthCount()
{
	return m_healthCount;
}

void Health::setHealthCount(int health)
{
	m_healthCount = health;
}
