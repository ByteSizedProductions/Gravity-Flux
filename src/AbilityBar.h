#pragma once
#ifndef __ABILITY_BAR__
#define __ABILITY_BAR__
#include  "DisplayObJect.h"
#include "TextureManager.h"

class AbilityBar : public DisplayObject
{
public:
	AbilityBar();

	~AbilityBar();

	void draw() override;
	void update() override;
	void clean() override;

	double getAbilityCooldown();

	void setAbilityCooldown(double cooldown);
private:
	double m_AbilityCooldown;
};



#endif
