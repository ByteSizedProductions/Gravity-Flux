#pragma once
#ifndef __BRAIN__
#define __BRAIN__
#include "DisplayObject.h"
#include "TextureManager.h"

class Brain final : public DisplayObject
{
public:
	//constuctors
	Brain();

	//destructor
	~Brain();

	float getInsanity();

	void setInsanity(float value);

	/*float getCountdown();

	void setCountdown(float value);*/

	// life cycle functions
	void draw() override;
	void update() override;
	void clean() override;
private:
	float m_insanity;
	/*float m_countdown;*/
};


#endif