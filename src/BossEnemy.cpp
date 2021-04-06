#include "BossEnemy.h"
#include "Tile.h"

BossEnemy::BossEnemy()
{
	TextureManager::Instance()->load("../Assets/textures/Bowser.png", "Bowser");
	auto size = TextureManager::Instance()->getTextureSize("Bowser");
	setWidth(size.x);
	setHeight(size.y);

	getTransform()->position = glm::vec2(200.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(ENEMY);

	setIsGrounded(true);
	m_currentAngle = 0.0f; // current facing angle
	m_currentDirection = glm::vec2(1.0f, 1.0f);
	m_direction = 0;
}

BossEnemy::~BossEnemy()
= default;

void BossEnemy::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->draw("Bowser", getTransform()->position.x, getTransform()->position.y, 0, 255, false);
}

void BossEnemy::update()
{
	m_attackCoolDown = (m_attackCoolDown + 1)%300;
	
	if (m_attackCoolDown == 0)
	{
		Attack();
	}
}

void BossEnemy::clean()
{
}

void BossEnemy::move()
{
}

glm::vec2 BossEnemy::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 BossEnemy::getCurrentDirection() const
{
	return m_currentDirection;
}

bool BossEnemy::getDirection() const
{
	return m_direction;
}

float BossEnemy::getMaxSpeed() const
{
	return m_maxSpeed;
}

bool BossEnemy::isMoving() const
{
	return m_isMoving;
}

void BossEnemy::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;
}

void BossEnemy::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void BossEnemy::setDirection(bool direction)
{
	m_direction = direction;
}

void BossEnemy::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void BossEnemy::setAcceleration(glm::vec2 acceleration)
{
	getRigidBody()->acceleration = acceleration;
}

void BossEnemy::setVelocity(glm::vec2 velocity)
{
	getRigidBody()->velocity = velocity;
}

void BossEnemy::setAngle(float angle)
{
	m_currentAngle = angle;
}

void BossEnemy::setIsMoving(bool moving)
{
	m_isMoving = moving;
}

void BossEnemy::Attack()
{
	m_floorSpikes = m_floorSpikes ? false : true;
}

bool BossEnemy::isFloorSpikes()
{
	return m_floorSpikes;
}

int BossEnemy::getAttackCoolDown()
{
	return m_attackCoolDown;
}

bool BossEnemy::checkAnimationDone(std::string animation)
{
	if (TextureManager::Instance()->checkAnimationDone(getAnimation(animation)))
		return true;
	else
		return false;
}

int BossEnemy::checkAnimationFrame()
{
	return TextureManager::Instance()->checkAnimationFrame(getAnimation("Boss"));
}

void BossEnemy::setAnimationFrame(std::string animation, int frame)
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->setAnimationFrame(getAnimation(animation), frame);
}

void BossEnemy::setAnimationState(PlayerAnimationState state)
{
	m_AnimationState = state;
}

PlayerAnimationState BossEnemy::getAnimationState()
{
	return m_AnimationState;
}

void BossEnemy::m_buildAnimations()
{
	
}

void BossEnemy::m_checkBounds()
{
	
}

void BossEnemy::m_reset()
{
}
