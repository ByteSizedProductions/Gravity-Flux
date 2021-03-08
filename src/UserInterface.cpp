#include "UserInterface.h"

UserInterface::UserInterface()
{
	setType(USERINTERFACE);
	
}

void UserInterface::draw()
{
}

void UserInterface::update()
{
}

void UserInterface::clean()
{
}

void UserInterface::m_setBomb(const int numBombs) const
{
	m_bombLabel->setText("Bombs: " + std::to_string(numBombs));
}

void UserInterface::m_setScore(const int numScore, int score)
{
	m_score += score;
	m_scoreLabel->setText("Score: " + std::to_string(m_score));
}

void UserInterface::m_setTimer()
{
	m_time += 1;
	m_timerLabel->setText("Time: " + std::to_string(m_time));
}

void UserInterface::m_setInsanity()
{
	m_insanity += 1;
}

void UserInterface::m_setEvent1Countdown()
{

}

int UserInterface::m_getScore() const
{
	return m_score;
}

int UserInterface::m_getTimer() const
{
	return m_time;
}

int UserInterface::m_getInsanity()
{
	return m_insanity;
}

//int UserInterface::m_getEvent1Countdown()
//{
//}

void UserInterface::m_addLabels()
{
	m_bombLabel = new Label("Bombs: 100", "Consolas", 20);
	m_scoreLabel = new Label("Score: 000", "Consolas", 20);
	m_timerLabel = new Label("Time: 00", "Consolas", 20);
	m_bombLabel->setType(UILABEL);
	m_scoreLabel->setType(UILABEL);
	m_timerLabel->setType(UILABEL);
	m_bombLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.1f, 200.0f);
	m_scoreLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.1f, 225.0f);
	m_timerLabel->getTransform()->position = glm::vec2(Config::SCREEN_WIDTH * 0.1f, 250.0f);
	getParent()->addChild(m_bombLabel);
	getParent()->addChild(m_scoreLabel);
	getParent()->addChild(m_timerLabel);
}

