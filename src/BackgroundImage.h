#pragma once
#ifndef __Background_Image__
#define __Background_Image__
#include "DisplayObject.h"

class BackgroundImage final : public DisplayObject
{
public:
	BackgroundImage();
	~BackgroundImage();
	void draw() override;
	void update() override;
	void clean() override;
};
#endif // !__Background_Image__
