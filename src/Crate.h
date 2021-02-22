#pragma once
#ifndef __CRATE_H__
#define __CRATE_H__
#include "DisplayObject.h"

class Crate : public DisplayObject {
public:
	Crate(glm::vec2 position);
	~Crate();

	void draw() override;
	void update() override;
	void clean() override;

	void updateGravity();
	void handleCollisions(GameObject* object);
	void setIsGrounded(bool grounded);
	bool isGrounded();
private:
	float m_gravity;
	float m_maxSpeed;
	bool m_isGrounded;
	float m_turnAngle;
	float m_turnRate;
	float m_force;
	glm::vec2 m_currentDirection;
};

#endif // !__CRATE_H__
