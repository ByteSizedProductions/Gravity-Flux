#pragma once
#ifndef __PICKUP__
#define __PICKUP__
#include "DisplayObject.h"

class Pickup : public DisplayObject
{
public:
	Pickup();
	~Pickup();

	// Life-cycle functions
	void draw() override;
	void update() override;
	void clean() override;

private:

};

#endif /* DEFINED __PICKUP__ */