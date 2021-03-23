#include "AbilityBar.h"

AbilityBar::AbilityBar()
{
	TextureManager::Instance()->load("../Assets/textures/ChargeBar.png", "AbilityBar");
	TextureManager::Instance()->load("../Assets/textures/ChargeBarOverlay.png", "AbilityCharge");
	auto size = TextureManager::Instance()->getTextureSize("AbilityBar");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(300.0f, 100.0f);

	setType(ABILITYBAR);
	getRigidBody()->isColliding = false;
}

AbilityBar::~AbilityBar() = default;

void AbilityBar::draw()
{
	const auto x = getTransform()->position.x - 45;
	const auto y = getTransform()->position.y - 18;
	
	TextureManager::Instance()->draw("AbilityBar", getTransform()->position.x, getTransform()->position.y, 0, 255, true);
	TextureManager::Instance()->drawFromSheet("AbilityCharge", 0, 0, (90 * getAbilityCooldown()), 36, 90, 90, x, y, 0, 255, false);
}

void AbilityBar::update()
{
}

void AbilityBar::clean()
{
}

double AbilityBar::getAbilityCooldown()
{
	return m_AbilityCooldown;
}

void AbilityBar::setAbilityCooldown(double cooldown)
{
	if (cooldown != 0)
		m_AbilityCooldown = 1.00 - cooldown / 60;
	
	else
		m_AbilityCooldown = 1.00;
}
