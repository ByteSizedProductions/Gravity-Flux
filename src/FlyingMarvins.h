#pragma once
#ifndef __FLYING_MARVINS__
#define __FLYING_MARVINS__
#include <SDL.h>
#include <vector>
#include "DisplayObject.h"

class FlyingMarvin : public DisplayObject
{
public:
	FlyingMarvin();

	void draw() override;
	void update() override;
	void clean() override;

	const SDL_FPoint& GetCenter() { return m_center; }
	const double& GetRadius() { return m_radius; }
private:
	SDL_FPoint m_center;
	double m_angle,
		m_dx, m_dy,
		m_radius,
		m_rotSpeed;
};

class MarvinField : public GameObject
{
public:
	MarvinField(unsigned int sz = 10);
	~MarvinField();

	void draw() override;
	void update() override;
	void clean() override;

	std::vector<FlyingMarvin*>& GetFlyingMarvins() { return m_marvins; }
	const unsigned int GetSize() { return m_size; }
private:
	std::vector<FlyingMarvin*> m_marvins;
	int m_size;
};

#endif /* DEFINED (__FLYING_MARVINS__) */
