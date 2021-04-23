#include "BackgroundImage.h"
#include "TextureManager.h"
#include "SoundManager.h"

BackgroundImage::BackgroundImage()
{
	TextureManager::Instance()->load("../Assets/textures/background.png", "background");
	auto size = TextureManager::Instance()->getTextureSize("background");
	setWidth(size.x);
	setHeight(size.y);
	getTransform()->position = glm::vec2(0.0f, 0.0f);
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

	TextureManager::Instance()->draw("background", getTransform()->position.x - 320.0f, getTransform()->position.y - 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 480.0f, getTransform()->position.y - 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 1280.0f, getTransform()->position.y - 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2080.0f, getTransform()->position.y - 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2880.0f, getTransform()->position.y - 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 3680.0f, getTransform()->position.y - 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 4480.0f, getTransform()->position.y - 320.0f, 0, 255, false);

	TextureManager::Instance()->draw("background", getTransform()->position.x - 320.0f, getTransform()->position.y + 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x - 320.0f, getTransform()->position.y + 280.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x - 320.0f, getTransform()->position.y + 880.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x - 320.0f, getTransform()->position.y + 1480.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x - 320.0f, getTransform()->position.y + 2080.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x - 320.0f, getTransform()->position.y + 2680.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x - 320.0f, getTransform()->position.y + 3280.0f, 0, 255, false);

	TextureManager::Instance()->draw("background", getTransform()->position.x + 480.0f, getTransform()->position.y + 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 480.0f, getTransform()->position.y + 280.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 480.0f, getTransform()->position.y + 880.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 480.0f, getTransform()->position.y + 1480.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 480.0f, getTransform()->position.y + 2080.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 480.0f, getTransform()->position.y + 2680.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 480.0f, getTransform()->position.y + 3280.0f, 0, 255, false);

	TextureManager::Instance()->draw("background", getTransform()->position.x + 1280.0f, getTransform()->position.y + 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 1280.0f, getTransform()->position.y + 280.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 1280.0f, getTransform()->position.y + 880.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 1280.0f, getTransform()->position.y + 1480.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 1280.0f, getTransform()->position.y + 2080.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 1280.0f, getTransform()->position.y + 2680.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 1280.0f, getTransform()->position.y + 3280.0f, 0, 255, false);

	TextureManager::Instance()->draw("background", getTransform()->position.x + 2080.0f, getTransform()->position.y + 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2080.0f, getTransform()->position.y + 280.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2080.0f, getTransform()->position.y + 880.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2080.0f, getTransform()->position.y + 1480.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2080.0f, getTransform()->position.y + 2080.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2080.0f, getTransform()->position.y + 2680.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2080.0f, getTransform()->position.y + 3280.0f, 0, 255, false);

	TextureManager::Instance()->draw("background", getTransform()->position.x + 2880.0f, getTransform()->position.y + 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2880.0f, getTransform()->position.y + 280.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2880.0f, getTransform()->position.y + 880.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2880.0f, getTransform()->position.y + 1480.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2880.0f, getTransform()->position.y + 2080.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2880.0f, getTransform()->position.y + 2680.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 2880.0f, getTransform()->position.y + 3280.0f, 0, 255, false);

	TextureManager::Instance()->draw("background", getTransform()->position.x + 3680.0f, getTransform()->position.y + 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 3680.0f, getTransform()->position.y + 280.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 3680.0f, getTransform()->position.y + 880.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 3680.0f, getTransform()->position.y + 1480.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 3680.0f, getTransform()->position.y + 2080.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 3680.0f, getTransform()->position.y + 2680.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 3680.0f, getTransform()->position.y + 3280.0f, 0, 255, false);

	TextureManager::Instance()->draw("background", getTransform()->position.x + 4480.0f, getTransform()->position.y + 320.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 4480.0f, getTransform()->position.y + 280.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 4480.0f, getTransform()->position.y + 880.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 4480.0f, getTransform()->position.y + 1480.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 4480.0f, getTransform()->position.y + 2080.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 4480.0f, getTransform()->position.y + 2680.0f, 0, 255, false);
	TextureManager::Instance()->draw("background", getTransform()->position.x + 4480.0f, getTransform()->position.y + 3280.0f, 0, 255, false);
}

void BackgroundImage::update()
{
}

void BackgroundImage::clean()
{
}
