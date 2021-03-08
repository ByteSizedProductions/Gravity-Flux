#include "Brain.h"

Brain::Brain()
{
	TextureManager::Instance()->load("../Assets/textures/brain1.png", "brain");
	auto size = TextureManager::Instance()->getTextureSize("brain");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 300.0f);

	setType(BRAIN);
	getRigidBody()->isColliding = false;
}

Brain::~Brain()
= default;

float Brain::getInsanity()
{
	return m_insanity;
}

void Brain::setInsanity(float value)
{
	m_insanity = value;
}

//float Brain::getCountdown()
//{
//	return m_countdown;
//}
//
//void Brain::setCountdown(float value)
//{
//	m_countdown = value;
//}

void Brain::draw()
{
	TextureManager::Instance()->draw("brain", getTransform()->position.x, getTransform()->position.y, 0, 255, true);
}

void Brain::update()
{
}

void Brain::clean()
{
}
