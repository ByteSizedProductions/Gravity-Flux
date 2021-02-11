#include "BombPickup.h"

#include "TextureManager.h"

BombPickup::BombPickup()
{
	TextureManager::Instance()->load("../Assets/textures/bombpickup.png", "bombpickup");
	auto size = TextureManager::Instance()->getTextureSize("bombpickup");
	setWidth(size.x);
	setHeight(size.y);

	setType(PICKUP);
}

BombPickup::~BombPickup()
= default;

void BombPickup::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw Marvin
	TextureManager::Instance()->draw("bombpickup", x, y, 0, 255, false); //Ship from the first scene
}

void BombPickup::update()
{
}

void BombPickup::clean()
{
}
