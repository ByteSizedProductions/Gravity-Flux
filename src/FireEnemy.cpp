#include "FireEnemy.h"
#include "PlayScene.h"
#include "TextureManager.h"
#include <algorithm>

FireEnemy::FireEnemy() : m_maxSpeed(1.1), PhysicsSprite()
{
	TextureManager::Instance()->loadSpriteSheet("../Assets/sprites/Fire_Enemy.txt", "../Assets/sprites/Fire_Enemy.png", "FlameEnemy");
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("FlameEnemy"));

	setWidth(46);
	setHeight(64);

	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(ENEMY);

	setIsGrounded(true);
	m_currentAngle = 0.0f; // current facing angle
	m_currentDirection = glm::vec2(1.0f, 1.0f); 
	m_direction = 0;
	m_fireBallActive = false;

	m_buildAnimations();

	setAnimationState(ENEMY_RUN_RIGHT);
}

FireEnemy::~FireEnemy()
= default;

void FireEnemy::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw Fire Enemy
	//TextureManager::Instance()->draw("FlameEnemy", x, y, m_currentAngle, 255, false, static_cast<SDL_RendererFlip>(m_direction));
	switch (getAnimationState())
	{
	case ENEMY_RUN_RIGHT:
			TextureManager::Instance()->playAnimation("FlameEnemy", getAnimation("run right"), x, y, 0.2f, m_currentAngle, 255, false);
		break;

	case ENEMY_RUN_LEFT:
			TextureManager::Instance()->playAnimation("FlameEnemy", getAnimation("run left"), x, y, 0.2f, m_currentAngle, 255, false);
		break;

	case ENEMY_DEATH:
		TextureManager::Instance()->playAnimationOnce("FlameEnemy", getAnimation("damage"), x, y, 0.2f, m_currentAngle, 255, false);
		break;

	case ENEMY_DAMAGE:
		if (m_direction)
			TextureManager::Instance()->playAnimationOnce("FlameEnemy", getAnimation("damage"), x, y, 0.3f, m_currentAngle, 255, false, SDL_FLIP_HORIZONTAL);
		else
			TextureManager::Instance()->playAnimationOnce("FlameEnemy", getAnimation("damage"), x, y, 0.3f, m_currentAngle, 255, false);
		if (TextureManager::Instance()->checkAnimationDone(getAnimation("damage")))
		{
			setAnimationFrame("damage", 0);
			if (m_direction == -1.0f)
				setAnimationState(ENEMY_RUN_LEFT);
			else
				setAnimationState(ENEMY_RUN_RIGHT);
		}
		break;

	default:
		TextureManager::Instance()->playAnimation("FlameEnemy", getAnimation("run right"), x, y, 0.10f, m_currentAngle, 255, false);
		break;
	}
}

void FireEnemy::update()
{
	if (m_isMoving)
		move();
	/*m_checkBounds();*/
	updateGravity();
}

void FireEnemy::updateGravity()
{
	getRigidBody()->velocity.y += getRigidBody()->acceleration.y + m_gravity * 0.075f;
	getRigidBody()->velocity.y = std::max(std::min(getRigidBody()->velocity.y, m_force), m_gravity);
	getTransform()->position.y += getRigidBody()->velocity.y;
	getRigidBody()->acceleration.y = 0.0f;
}

void FireEnemy::handleCollisions(GameObject* object)
{
	if ((object->getType() == TILE && (static_cast<Tile*>(object)->GetTileType() == GROUND) || (static_cast<Tile*>(object)->GetTileType() == PLATFORM)
		|| (static_cast<Tile*>(object)->GetTileType() == CRATE) || (static_cast<Tile*>(object)->GetTileType() == DAMAGING))) {
		//did player collide with the top of the platform?
		if (int(getTransform()->position.y + getHeight() - getRigidBody()->velocity.y) <= int(object->getTransform()->position.y)) {
			getRigidBody()->velocity.y = 0.0f;
			getTransform()->position.y = object->getTransform()->position.y - getHeight();
		}
		//did the player collide with the left side of the platform?
		else if (int(getTransform()->position.x + getWidth() - getRigidBody()->velocity.x) <= int(object->getTransform()->position.x)) {
			getRigidBody()->velocity.x = 0.0f;
			getTransform()->position.x = object->getTransform()->position.x - getWidth() - 1;
			setCurrentDirection(glm::vec2(-1.0f, getCurrentDirection().y));
			setAnimationState(ENEMY_RUN_LEFT);
		}
		//did the player cllide with the right side of the platform?
		else if (int(getTransform()->position.x - getRigidBody()->velocity.x) >= int(object->getTransform()->position.x + object->getWidth())) {
			getRigidBody()->velocity.x = 0.0f;
			getTransform()->position.x = object->getTransform()->position.x + object->getWidth() + 1;
			setCurrentDirection(glm::vec2(1.0f, getCurrentDirection().y));
			setAnimationState(ENEMY_RUN_RIGHT);
		}
	}
}

void FireEnemy::clean()
{
}

void FireEnemy::move()
{
	getRigidBody()->velocity.x = m_currentDirection.x * m_maxSpeed;
	getTransform()->position.x += getRigidBody()->velocity.x;
	if (m_currentDirection.x == 1.0f)
		m_direction = 1;
	else if (m_currentDirection.x == -1.0f)
		m_direction = 0;
}

glm::vec2 FireEnemy::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 FireEnemy::getCurrentDirection() const
{
	return m_currentDirection;
}

bool FireEnemy::getDirection() const
{
	return m_direction;
}

float FireEnemy::getMaxSpeed() const
{
	return m_maxSpeed;
}

bool FireEnemy::getFireBallActive() const
{
	return m_fireBallActive;
}

bool FireEnemy::isMoving() const
{
	return m_isMoving;
}

void FireEnemy::setFireBallActive(bool state)
{
	m_fireBallActive = state;
}

void FireEnemy::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;
}

void FireEnemy::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void FireEnemy::setDirection(bool direction)
{
	m_direction = direction;
}

void FireEnemy::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void FireEnemy::setAcceleration(glm::vec2 acceleration)
{
	getRigidBody()->acceleration = acceleration;
}

void FireEnemy::setVelocity(glm::vec2 velocity)
{
	getRigidBody()->velocity = velocity;
}

void FireEnemy::setAngle(float angle)
{
	m_currentAngle = angle;
}

void FireEnemy::setIsMoving(bool moving)
{
	m_isMoving = moving;
}

//void FireEnemy::m_checkBounds()
//{
//	if (getTransform()->position.x > Config::SCREEN_WIDTH)
//	{
//		getTransform()->position = glm::vec2(0.0f, getTransform()->position.y);
//	}
//
//	if (getTransform()->position.x < 0)
//	{
//		getTransform()->position = glm::vec2(800.0f, getTransform()->position.y);
//	}
//
//	if (getTransform()->position.y > Config::SCREEN_HEIGHT)
//	{
//		getTransform()->position = glm::vec2(getTransform()->position.x, 0.0f);
//	}
//
//	if (getTransform()->position.y < 0)
//	{
//		getTransform()->position = glm::vec2(getTransform()->position.x, 600.0f);
//	}
//}

bool FireEnemy::checkAnimationDone(std::string animation)
{
	return TextureManager::Instance()->checkAnimationFrame(getAnimation("FireEnemy"));
}

int FireEnemy::checkAnimationFrame()
{
	return TextureManager::Instance()->checkAnimationFrame(getAnimation("FireEnemy"));
}

void FireEnemy::setAnimationFrame(std::string animation, int frame)
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->setAnimationFrame(getAnimation(animation), frame);
}

void FireEnemy::setAnimationState(EnemyAnimationState state)
{
	m_AnimationState = state;
}

EnemyAnimationState FireEnemy::getAnimationState()
{
	return m_AnimationState;
}

void FireEnemy::ChangeDirection()
{
	if (m_currentDirection.x == 1.0f)
		m_direction = 1;
	else if (m_currentDirection.x == -1.0f)
		m_direction = 0;
}

void FireEnemy::m_buildAnimations()
{
	//running right Animation
	Animation rightRunAnimation = Animation();
	rightRunAnimation.name = "run right";

	for (int i = 1; i <= 4; i++)
		rightRunAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-runright-" + std::to_string(i)));

	setAnimation(rightRunAnimation);

	//running left Animation
	Animation leftRunAnimation = Animation();
	leftRunAnimation.name = "run left";

	for (int i = 1; i <= 4; i++)
		leftRunAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-runleft-" + std::to_string(i)));

	setAnimation(leftRunAnimation);

	//Death Animation
	Animation deathAnimation = Animation();
	deathAnimation.name = "death";

	for (int i = 1; i <= 4; i++)
		deathAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-death-" + std::to_string(i)));

	setAnimation(deathAnimation);

	//Damage Animation
	Animation damageAnimation = Animation();
	damageAnimation.name = "damage";

	for (int i = 1; i <= 3; i++)
		damageAnimation.frames.push_back(getSpriteSheet()->getFrame("enemy-hurt-" + std::to_string(i)));

	setAnimation(damageAnimation);
}
