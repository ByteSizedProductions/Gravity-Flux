#include "BackgroundImage.h"
#include "TextureManager.h"
#include "SoundManager.h"

BackgroundImage::BackgroundImage()
{
	TextureManager::Instance()->load("../Assets/textures/background.png", "background");
	auto size = TextureManager::Instance()->getTextureSize("background");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(-350.0f, -250.0f);
	setType(BACKGROUND);
	getRigidBody()->isColliding = false;
}

BackgroundImage::~BackgroundImage()
{
}

void BackgroundImage::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("background", getTransform()->position.x, getTransform()->position.y, 0, 255, false);
	//TextureManager::Instance()->drawFromSheet("background", 0, 0, 3200, 2400, 3200, 2000, x, y, 0, 255, false);
}

void BackgroundImage::update()
{
}

void BackgroundImage::clean()
{
}
