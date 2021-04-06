#include "Marvin.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"
#include <algorithm>


Marvin::Marvin() : m_maxSpeed(7) , PhysicsSprite()
{
	TextureManager::Instance()->load("../Assets/textures/marvin.png", "marvin");

	TextureManager::Instance()->loadSpriteSheet("../Assets/sprites/MainCharacter.txt", "../Assets/sprites/MainCharacter.png", "Player");
	setSpriteSheet(TextureManager::Instance()->getSpriteSheet("Player"));

	//auto size = TextureManager::Instance()->getTextureSize("marvin");
	setWidth(54);
	setHeight(100);

	//getTransform()->position = glm::vec2(400.0f, 500.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(PLAYER);

	m_marvinHealth = new Health();
	m_marvinHealth->getTransform()->position = glm::vec2(10.0f, 20.0f);

	setIsGrounded(true);
	m_currentAngle = 0.0f; // current facing angle
	m_currentDirection = glm::vec2(1.0f, 1.0f); // facing right, gravity is down
	m_turnRate = 5.0f; // 5 degrees per frame
	m_force = -18.0f; //override the PhysicsObject() Force
	m_drag = 0.88f;
	m_direction = 0;
	m_gravityCooldown = 0;

	m_buildAnimations();

	setAnimationState(PLAYER_IDLE_RIGHT);
}


Marvin::~Marvin() {
	delete m_marvinHealth;
}

void Marvin::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw Marvin
	//TextureManager::Instance()->draw("marvin", x, y, m_currentAngle, 255, false, static_cast<SDL_RendererFlip>(m_direction)); //Ship from the first scene
	switch(getAnimationState())
	{
	case PLAYER_IDLE_RIGHT:
		TextureManager::Instance()->playAnimation("Player", getAnimation("idle"), x, y, 0.30f, m_currentAngle, 255, false);
		break;
		
	case PLAYER_IDLE_LEFT:
		TextureManager::Instance()->playAnimation("Player", getAnimation("idle"), x, y, 0.10f, m_currentAngle, 255, false, SDL_FLIP_HORIZONTAL);
		break;
		
	case PLAYER_RUN_RIGHT:
		if (m_isGravityFlipped)
			TextureManager::Instance()->playAnimation("Player", getAnimation("run right"), x, y, 0.30f, m_currentAngle, 255, false, SDL_FLIP_HORIZONTAL);
		else
			TextureManager::Instance()->playAnimation("Player", getAnimation("run right"), x, y, 0.30f, m_currentAngle, 255, false);
		break;
		
	case PLAYER_RUN_LEFT:
		if (m_isGravityFlipped)
			TextureManager::Instance()->playAnimation("Player", getAnimation("run left"), x, y, 0.30f, m_currentAngle, 255, false, SDL_FLIP_HORIZONTAL);
		else
			TextureManager::Instance()->playAnimation("Player", getAnimation("run left"), x, y, 0.30f, m_currentAngle, 255, false);
		break;
		
	case PLAYER_BOMB_RIGHT:
		TextureManager::Instance()->playAnimationOnce("Player", getAnimation("bomb right"), x, y, 1.0f, m_currentAngle, 255, false);
		break;
		
	case PLAYER_BOMB_LEFT:
		TextureManager::Instance()->playAnimationOnce("Player", getAnimation("bomb left"), x, y, 1.0f, m_currentAngle, 255, false);

		break;
		
	case PLAYER_MIDAIR:
		TextureManager::Instance()->playAnimation("Player", getAnimation("midair"), x, y, 0.20f, m_currentAngle, 255, false, m_flip);
		break;
		
	case PLAYER_DEATH:
		TextureManager::Instance()->playAnimationOnce("Player", getAnimation("death"), x, y, 0.8f, m_currentAngle, 255, false);
		break;

	case PLAYER_DAMAGE:
		if (m_direction)
			TextureManager::Instance()->playAnimationOnce("Player", getAnimation("damage"), x, y, 0.3f, m_currentAngle, 255, false, SDL_FLIP_HORIZONTAL);
		else
			TextureManager::Instance()->playAnimationOnce("Player", getAnimation("damage"), x, y, 0.3f, m_currentAngle, 255, false);
		if(TextureManager::Instance()->checkAnimationDone(getAnimation("damage")))
		{
			setAnimationFrame("damage", 0);
			if (m_direction)
				setAnimationState(PLAYER_IDLE_LEFT);
			else
				setAnimationState(PLAYER_IDLE_RIGHT);
		}
		break;
		
	default:
		TextureManager::Instance()->playAnimation("Player", getAnimation("idle"), x, y, 0.10f, m_currentAngle, 255, false);
		break;
	}
	m_marvinHealth->draw();
}

void Marvin::update()
{
	if (m_isMoving)
		move();
	updateGravity();
	m_checkBounds();

	if (!getDirection())
		m_flip = SDL_FLIP_NONE;
	else
		m_flip = SDL_FLIP_HORIZONTAL;
	//TEMPORARY
	if (m_gravityCooldown > 0)
		m_gravityCooldown--;

	if (getBombCooldown() > 0)
		setBombCooldown(getBombCooldown() - 1);
}

void Marvin::updateGravity()
{
	getRigidBody()->velocity.y += getRigidBody()->acceleration.y + m_gravity * 0.075f;
	if (!m_isGravityFlipped)
		getRigidBody()->velocity.y = std::min(std::max(getRigidBody()->velocity.y, m_force), m_gravity);
	else
		getRigidBody()->velocity.y = std::max(std::min(getRigidBody()->velocity.y, m_force), m_gravity);
	getTransform()->position.y += getRigidBody()->velocity.y;
	getRigidBody()->acceleration.y = 0.0f;
}

void Marvin::handleCollisions(GameObject* object)
{
	if ((object->getType() == TILE && (static_cast<Tile*>(object)->GetTileType() == GROUND) || (static_cast<Tile*>(object)->GetTileType() == PLATFORM)
		|| (static_cast<Tile*>(object)->GetTileType() == CRATE) || (static_cast<Tile*>(object)->GetTileType() == DAMAGING))) {
		//did player collide with the top of the platform?
		if (int(getTransform()->position.y + getHeight() - getRigidBody()->velocity.y) <= int(object->getTransform()->position.y)) {
			getRigidBody()->velocity.y = 0.0f;
			getTransform()->position.y = object->getTransform()->position.y - getHeight();
			if (!m_isGravityFlipped) {
				setIsGrounded(true);
				if (static_cast<Tile*>(object)->GetTileType() == DAMAGING && getAnimationState() != PLAYER_DEATH) 
				{
					jump();
					m_marvinHealth->setHealthCount(m_marvinHealth->getHealthCount() - 1);
				}
			}
			else {
				if (static_cast<Tile*>(object)->GetTileType() == DAMAGING && getAnimationState() != PLAYER_DEATH)
				{
					m_marvinHealth->setHealthCount(m_marvinHealth->getHealthCount() - 1);
					setIsGrounded(false);
				}
			}
		}
		//did the player collide with the bottom of the platform?
		else if (int(getTransform()->position.y - getRigidBody()->velocity.y) >= int(object->getTransform()->position.y + object->getHeight())) {
			getRigidBody()->velocity.y = 0.0f;
			getTransform()->position.y = object->getTransform()->position.y + object->getHeight();
			if (m_isGravityFlipped) {
				setIsGrounded(true);
				if (static_cast<Tile*>(object)->GetTileType() == DAMAGING && getAnimationState() != PLAYER_DEATH) 
				{
					jump();
					m_marvinHealth->setHealthCount(m_marvinHealth->getHealthCount() - 1);
				}
			}
			else {
				if (static_cast<Tile*>(object)->GetTileType() == DAMAGING && getAnimationState() != PLAYER_DEATH)
				{
					m_marvinHealth->setHealthCount(m_marvinHealth->getHealthCount() - 1);
					setIsGrounded(false);
				}
			}
		}
		//did the player collide with the left side of the platform?
		else if (int(getTransform()->position.x + getWidth() - getRigidBody()->velocity.x) <= int(object->getTransform()->position.x)) {
			getRigidBody()->velocity.x = 0.0f;
			getTransform()->position.x = object->getTransform()->position.x - getWidth() - 1;
		}
		//did the player cllide with the right side of the platform?
		else if (int(getTransform()->position.x - getRigidBody()->velocity.x) >= int(object->getTransform()->position.x + object->getWidth())) {
			getRigidBody()->velocity.x = 0.0f;
			getTransform()->position.x = object->getTransform()->position.x + object->getWidth() + 1;
		}
	}
}

void Marvin::clean()
{
}

void Marvin::jump()
{
	if (m_isGrounded)
	{
		m_isGrounded = false;
		getRigidBody()->acceleration.y = m_force;
	}
	else
		getRigidBody()->acceleration.y = 0.0f;
}

void Marvin::move()
{
	getRigidBody()->velocity.x = m_currentDirection.x * m_maxSpeed;
	getTransform()->position.x += getRigidBody()->velocity.x;
	if (m_currentDirection.x == 1.0f)
		m_direction = m_isGravityFlipped ? 1 : 0;
	else if (m_currentDirection.x == -1.0f)
		m_direction = m_isGravityFlipped ? 0 : 1;
}

glm::vec2 Marvin::getTargetPosition() const
{
	return m_targetPosition;
}

glm::vec2 Marvin::getCurrentDirection() const
{
	return m_currentDirection;
}

float Marvin::getMaxSpeed() const
{
	return m_maxSpeed;
}

bool Marvin::getDirection() const
{
	return m_direction;
}

bool Marvin::isGravityFlipped() const
{
	return m_isGravityFlipped;
}

bool Marvin::isMoving() const
{
	return m_isMoving;
}

int Marvin::getGravityCooldown() const
{
	return m_gravityCooldown;
}

bool Marvin::isWithinGravityNullifier() const
{
	return m_isWithinGravityNullifier;
}

void Marvin::setTargetPosition(glm::vec2 newPosition)
{
	m_targetPosition = newPosition;
}

void Marvin::setCurrentDirection(glm::vec2 newDirection)
{
	m_currentDirection = newDirection;
}

void Marvin::setDirection(bool direction)
{
	m_direction = direction;
}

void Marvin::setMaxSpeed(float newSpeed)
{
	m_maxSpeed = newSpeed;
}

void Marvin::setAcceleration(glm::vec2 acceleration)
{
	getRigidBody()->acceleration = acceleration;
}

void Marvin::setVelocity(glm::vec2 velocity)
{
	getRigidBody()->velocity = velocity;
}

void Marvin::setGravityFlipped(bool flipped)
{
	m_isGravityFlipped = flipped;
}

void Marvin::setAngle(float angle) {
	m_currentAngle = angle;
}

void Marvin::setIsMoving(bool moving) {
	m_isMoving = moving;
}

void Marvin::setNumBombs(int numBombs)
{
	m_numBombs = numBombs;
}

int Marvin::getNumBombs() const
{
	return m_numBombs;
}

int Marvin::getBombCooldown()
{
	return m_bombCooldown;
}

void Marvin::setBombCooldown(int cooldown)
{
	m_bombCooldown = cooldown;
}

int Marvin::getHealthCount()
{
	return m_marvinHealth->getHealthCount();
}

void Marvin::setHealthCount(int health)
{
	m_marvinHealth->setHealthCount(health);
}

bool Marvin::checkAnimationDone(std::string animation)
{
	if (TextureManager::Instance()->checkAnimationDone(getAnimation(animation)))
		return true;
	else
		return false;
}

int Marvin::checkAnimationFrame()
{
	return TextureManager::Instance()->checkAnimationFrame(getAnimation("Player"));
}

void Marvin::setAnimationFrame(std::string animation, int frame)
{
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	TextureManager::Instance()->setAnimationFrame(getAnimation(animation), frame);
}

void Marvin::setAnimationState(PlayerAnimationState state)
{
	m_AnimationState = state;
}

PlayerAnimationState Marvin::getAnimationState()
{
	return m_AnimationState;
}

void Marvin::setGravityCooldown(int cooldown)
{
	m_gravityCooldown = cooldown;
}

void Marvin::m_checkBounds()
{

	if (getTransform()->position.x > Config::SCREEN_WIDTH)
	{
		getTransform()->position = glm::vec2(0.0f, getTransform()->position.y);
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position = glm::vec2(800.0f, getTransform()->position.y);
	}

	if (getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 0.0f);
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 600.0f);
	}

}

void Marvin::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}

void Marvin::ChangeDirection()
{
	if (m_currentDirection.x == 1.0f)
		m_direction = m_isGravityFlipped ? 1 : 0;
	else if (m_currentDirection.x == -1.0f)
		m_direction = m_isGravityFlipped ? 0 : 1;
}

void Marvin::setIsWithinGravityNullifier(bool state)
{
	m_isWithinGravityNullifier = state;
}

void Marvin::m_buildAnimations()
{
	//idle Animation
	Animation idleAnimation = Animation();
	idleAnimation.name = "idle";

	for (int i = 2; i <= 12; i++)
		idleAnimation.frames.push_back(getSpriteSheet()->getFrame("character-idle-" + std::to_string(i)));
	
	setAnimation(idleAnimation);

	//running right Animation
	Animation rightRunAnimation = Animation();
	rightRunAnimation.name = "run right";

	for (int i = 1; i <= 6; i++)
		rightRunAnimation.frames.push_back(getSpriteSheet()->getFrame("character-runright-" + std::to_string(i)));

	setAnimation(rightRunAnimation);

	//running left Animation
	Animation leftRunAnimation = Animation();
	leftRunAnimation.name = "run left";

	for (int i = 1; i <= 6; i++)
		leftRunAnimation.frames.push_back(getSpriteSheet()->getFrame("character-runleft-" + std::to_string(i)));

	setAnimation(leftRunAnimation);

	//midair Animation
	Animation midairAnimation = Animation();
	midairAnimation.name = "midair";

	for (int i = 1; i <= 4; i++)
		midairAnimation.frames.push_back(getSpriteSheet()->getFrame("character-midair-" + std::to_string(i)));

	setAnimation(midairAnimation);

	//Bomb right Animation
	Animation bombRightAnimation = Animation();
	bombRightAnimation.name = "bomb right";

	for(int i = 1; i <= 7; i++)
		bombRightAnimation.frames.push_back(getSpriteSheet()->getFrame("character-bombright-" + std::to_string(i)));
	
	setAnimation(bombRightAnimation);

	//Bomb left Animation
	Animation bombLeftAnimation = Animation();
	bombLeftAnimation.name = "bomb left";

	for (int i = 1; i <= 7; i++)
		bombLeftAnimation.frames.push_back(getSpriteSheet()->getFrame("character-bombleft-" + std::to_string(i)));

	setAnimation(bombLeftAnimation);

	//Death Animation
	Animation deathAnimation = Animation();
	deathAnimation.name = "death";

	for(int i = 1; i <= 12; i++)
		deathAnimation.frames.push_back(getSpriteSheet()->getFrame("character-death-" + std::to_string(i)));

	setAnimation(deathAnimation);

	//Damage Animation
	Animation damageAnimation = Animation();
	damageAnimation.name = "damage";

	for(int i = 1; i <= 4; i++)
		damageAnimation.frames.push_back(getSpriteSheet()->getFrame("character-damage-" + std::to_string(i)));

	setAnimation(damageAnimation);
	
}