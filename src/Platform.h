#pragma once
#ifndef __PLATFORM__
#define __PLATFORM__
#include "DisplayObject.h"
#include "SDL.h"

class Platform : public DisplayObject {
public:
	Platform(glm::vec2 position, int width, int height);
	~Platform();

	void draw() override;
	void update() override;
	void clean() override;

private:
	SDL_Rect* m_rect;
};

#endif