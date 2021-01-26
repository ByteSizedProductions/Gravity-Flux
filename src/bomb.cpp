#include "bomb.h"
#include "Player.h"
#include "TextureManager.h"
#include <string>

Bomb::Bomb(glm::vec2 position)
{
	TextureManager::Instance()->loadSpriteSheet(
		"../Assets/sprites/bomb_explosion.txt",
		"../Assets/sprites/bomb_explosion.png",
		"bombss");

	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("bombss"));

	// set frame width
	setWidth(64);

	// set frame height
	setHeight(64);
	
	getTransform()->position = position;
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(BOMB);

	m_buildAnimations();
}

Bomb::~Bomb()
= default;

void Bomb::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the bomb with explosion animation
	TextureManager::Instance()->playAnimationOnce(
		"bombss", getAnimation("bomb"),
		x, y, 4.0f, 0, 255, true);
}

void Bomb::update()
{
}

void Bomb::clean()
{
}

void Bomb::m_buildAnimations()
{
	Animation bombAnimation = Animation();
	
	bombAnimation.name = "bomb";

	for (int i = 0; i < 10; i++)
	{
		bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb"));
	}

	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion1"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion2"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion3"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion4"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion5"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion6"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion7"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion8"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion9"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion10"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion11"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion12"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion13"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion14"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion15"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion16"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion17"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion18"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion19"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion20"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion21"));
	bombAnimation.frames.push_back(getSpriteSheet()->getFrame("bomb_explosion22"));

	setAnimation(bombAnimation);

	m_totalFrames = bombAnimation.frames.size();
}