#pragma once
#ifndef __PHYSICS_OBJECT_H__
#define __PHYSICS_OBJECT_H__
#include "DisplayObject.h"

class PhysicsObject : public DisplayObject {

public:
	PhysicsObject();
	~PhysicsObject();

	virtual void draw() override = 0;
	virtual void update() override = 0;
	virtual void clean() override = 0;
	
	//while CAN be overriden, should usually not. Call the superclass version of these functions in the base classes
	virtual void updateGravity();
	virtual void handleCollisions(GameObject* object);

	//don't need to be overriden, are just getters and setters
	void setIsGrounded(bool grounded);
	bool isGrounded();
	float getGravity();
	void setGravity(float gravity);
	float getForce();
	void setForce(float force);

protected:
	float m_gravity;
	bool m_isGrounded;
	float m_force;
};

#endif // __PHYSICS_OBJECT__
