#pragma once
#ifndef __STATIC_SPRITE__
#define __STATIC_SPRITE__

#include "DisplayObject.h"

class StaticSprite final : public DisplayObject
{
public:
	StaticSprite(std::string imagePath, std::string id);
	~StaticSprite();
	
	void draw() override;
	void update() override;
	void clean() override;
private:
	std::string m_id;
};

#endif /* DEFINED (__STATIC_SPRITE__) */
