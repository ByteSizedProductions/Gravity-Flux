#include "Door.h"
#include "TextureManager.h"
#include "SoundManager.h"

Door::Door()
{
	TextureManager::Instance()->load("../Assets/textures/door.png", "door");
	auto size = TextureManager::Instance()->getTextureSize("door");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(300.0f, 300.0f);
	setType(DOOR);
	getRigidBody()->isColliding = false;
}

Door::~Door()
{
}

void Door::draw()
{
	TextureManager::Instance()->draw("door", getTransform()->position.x, getTransform()->position.y, 0, 255, false);
}

void Door::update()
{
}

void Door::clean()
{
}
