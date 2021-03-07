#include "GameObject.h"

GameObject::GameObject():
	m_width(0), m_height(0), m_type(NONE), m_enabled(true)
{
}

GameObject::~GameObject()
= default;

Transform* GameObject::getTransform() 
{
	return &m_transform;
}

RigidBody* GameObject::getRigidBody() 
{
	return &m_rigidBody;
}

int GameObject::getWidth() const
{
	return m_width;
}

int GameObject::getHeight() const
{
	return m_height;
}

GameObjectType GameObject::getType() const
{
	return m_type;
}

void GameObject::setWidth(const int new_width)
{
	m_width = new_width;
}

void GameObject::setHeight(const int new_height)
{
	m_height = new_height;
}

void GameObject::setType(const GameObjectType new_type)
{
	m_type = new_type;
}

void GameObject::setScroll(glm::vec2 ScrollSpeed)
{
	//formatted this way to prevent floating point inaccuracies
	this->getTransform()->position.x -= round((ScrollSpeed.x * 10000.0) / 10000.0);
	this->getTransform()->position.y -= round((ScrollSpeed.y * 10000.0) / 10000.0);
}

void GameObject::setEnabled(const bool state)
{
	m_enabled = state;
}

bool GameObject::isEnabled() const
{
	return m_enabled;
}
