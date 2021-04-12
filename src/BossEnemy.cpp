#include "BossEnemy.h"
#include "Tile.h"

BossEnemy::BossEnemy()
{
	TextureManager::Instance()->load("../Assets/sprites/gflux_boss.png", "BossDead");

	TextureManager::Instance()->loadSpriteSheet("../Assets/sprites/gflux_boss.txt", "../Assets/sprites/gflux_boss.png", "Boss");
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("Boss"));
	
	setWidth(359);
	setHeight(417);

	getTransform()->position = glm::vec2(200.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(ENEMY);

	setIsGrounded(true);
	m_currentAngle = 0.0f; // current facing angle
	m_currentDirection = glm::vec2(1.0f, 1.0f);
	m_direction = 0;

	m_buildAnimations();

	setAnimationState(BOSS_IDLE);
}

BossEnemy::~BossEnemy()
= default;

void BossEnemy::draw()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	isBossDead();
	//TextureManager::Instance()->draw("Bowser", getTransform()->position.x, getTransform()->position.y, 0, 255, false);
	switch(getAnimationState())
	{
	case BOSS_IDLE:
		TextureManager::Instance()->playAnimation("Boss", getAnimation("idle"), x, y, 0.30f, m_currentAngle, 255, false);
		break;
		
	case BOSS_SMASH:
		TextureManager::Instance()->playAnimation("Boss", getAnimation("smash"), x, y, 0.30f, m_currentAngle, 255, false);
		break;
		
	case BOSS_SHOOT:
		TextureManager::Instance()->playAnimation("Boss", getAnimation("shoot"), x, y, 0.40f, m_currentAngle, 255, false);
		break;
		
	case BOSS_DEATH:
		TextureManager::Instance()->playAnimationOnce("Boss", getAnimation("death"), x, y, 0.30f, m_currentAngle, 255, false);
		break;
		
	default:
		TextureManager::Instance()->playAnimation("Boss", getAnimation("idle"), x, y, 0.30f, m_currentAngle, 255, false);
		break;
	}
}

void BossEnemy::update()
{
	m_attackCoolDown = (m_attackCoolDown + 1)%300;
	
	if (m_attackCoolDown == 0)
	{
		Attack();
		setAnimationState(BOSS_SMASH);
	}
	else if (checkAnimationDone("smash"))
	{
		setAnimationState(BOSS_IDLE);
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

void BossEnemy::isBossDead()
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;
	if (checkAnimationDone("death"))
	{
		TextureManager::Instance()->drawFromSheet("BossDead", 1466, 1652, 359, 383, 359, 359, x, y, 0, 255, false);
	}
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

void BossEnemy::setIsBossDead(bool isBossDead)
{
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

void BossEnemy::setAnimationState(BossAnimationState state)
{
	m_AnimationState = state;
}

BossAnimationState BossEnemy::getAnimationState()
{
	return m_AnimationState;
}

void BossEnemy::m_buildAnimations()
{
	//idle Animation
	Animation idleAnimation = Animation();
	idleAnimation.name = "idle";

	for (int i = 1; i <= 4; i++)
		idleAnimation.frames.push_back(getSpriteSheet()->getFrame("boss-idle-" + std::to_string(i)));

	setAnimation(idleAnimation);

	//Smash Animation
	Animation smashAnimation = Animation();
	smashAnimation.name = "smash";

	for(int i = 1; i <= 9; i++)
		smashAnimation.frames.push_back(getSpriteSheet()->getFrame("boss-smash-" + std::to_string(i)));

	setAnimation(smashAnimation);

	//Shoot Animation
	Animation shootAnimation = Animation();
	shootAnimation.name = "shoot";

	for(int i = 1; i <= 6; i++)
		shootAnimation.frames.push_back(getSpriteSheet()->getFrame("boss-fire-" + std::to_string(i)));

	setAnimation(shootAnimation);

	//Death Animation
	Animation deathAnimation = Animation();
	deathAnimation.name = "death";

	for(int i = 1; i <= 11; i++)
		deathAnimation.frames.push_back(getSpriteSheet()->getFrame("boss-death-" + std::to_string(i)));

	setAnimation(deathAnimation);
}

void BossEnemy::m_checkBounds()
{
	
}

void BossEnemy::m_reset()
{
}
