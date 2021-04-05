#include "FlyingMarvins.h"
#include "Config.h"
#include "TextureManager.h"
#include "Util.h"

FlyingMarvin::FlyingMarvin() : m_angle(0.0), m_radius(33.0)
{
	TextureManager::Instance()->load("../Assets/textures/marvin.png", "marvin");
	auto size = TextureManager::Instance()->getTextureSize("marvin");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(25.0f + rand() % 901, ((rand() % 2 + 1) % 2 == 0 ? 25.0f : 600.0f) + (rand() % 76));
	setType(MENU);
	getRigidBody()->isColliding = false;
	
	m_center = { (getTransform()->position.x + getWidth() / 2.0f), (getTransform()->position.y + getHeight() / 2.0f) };
	m_rotSpeed = (1.0 + rand() % 5) * (rand() % 2 * 2.0 - 1.0);
	m_dx = (rand() % 360 - 90.0) * Util::Deg2Rad;
	m_dy = (rand() % 360 - 90.0) * Util::Deg2Rad;
}

void FlyingMarvin::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("marvin", getTransform()->position.x, getTransform()->position.y, m_angle, 255, false);
}

void FlyingMarvin::update()
{
	// rotate and move
	m_angle += m_rotSpeed;
	m_center.x += (float)m_dx;
	m_center.y += (float)m_dy;

	// wrap
	if (m_center.x < -getWidth() / 2) m_center.x = Config::SCREEN_WIDTH + getWidth() / 2;
	else if (m_center.x > Config::SCREEN_WIDTH + getWidth() / 2) m_center.x = 0 - getWidth() / 2;
	else if (m_center.y < -getHeight() / 2) m_center.y = Config::SCREEN_HEIGHT + getHeight() / 2;
	else if (m_center.y > Config::SCREEN_HEIGHT + getHeight() / 2) m_center.y = 0 - getHeight() / 2;

	// update
	getTransform()->position.x = m_center.x - getWidth() / 2;
	getTransform()->position.y = m_center.y - getHeight() / 2;
}

void FlyingMarvin::clean()
{
}

MarvinField::MarvinField(unsigned sz) : m_size(sz)
{
	for (int i = 0; i < m_size; i++)
	{
		m_marvins.push_back(new FlyingMarvin());
		m_marvins[i]->getTransform()->position = glm::vec2(25.0f + rand() % 901, (i % 2 == 0 ? 25.0f : 600.0f) + (rand() % 76));
	}
	m_marvins.shrink_to_fit();
}

MarvinField::~MarvinField()
{
	for (auto a : m_marvins)
	{
		delete a;
		a = nullptr;
	}
	m_marvins.clear();
}

void MarvinField::draw()
{
	for (const auto a : m_marvins)
		a->draw();
}

void MarvinField::update()
{
	for (const auto a : m_marvins)
		a->draw();
}

void MarvinField::clean()
{
}
