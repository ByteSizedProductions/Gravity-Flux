#pragma once
#ifndef __HEALTH__
#define __HEALTH
#include "DisplayObject.h"

class Health :public DisplayObject
{
public:
	Health();

	// Life-cycle functions
	void draw() override;
	void update() override;
	void clean() override;
	int getHealthCount();
	void setHealthCount(int health);


private:
	int m_healthCount;

};

#endif // !__HEALTH__
