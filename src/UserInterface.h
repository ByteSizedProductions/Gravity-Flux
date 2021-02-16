#pragma once
#ifndef __USER_INTERFACE__
#define __USER_INTERFACE__
#include "DisplayObject.h"
#include "Label.h"

class UserInterface : public DisplayObject
{
public:
	UserInterface();
	
	// Life-cycle functions
	void draw() override;
	void update() override;
	void clean() override;

	void m_setBomb(int numBombs) const;
	void m_setScore(int numScore, int score);
	void m_setTimer();

	int m_getScore() const;
	int m_getTimer() const;

	void m_addLabels();
	
private:
	Label* m_bombLabel;
	Label* m_scoreLabel;
	Label* m_timerLabel;

	int m_score;
	int m_time;
};

#endif /* DEFINED (__USER_INTERFACE__) */