#pragma once
#ifndef __LOSE_SCENE__
#define __LOSE__SCENE__
#include "Scene.h"
#include "Label.h"
#include "Button.h"

class LoseScene final : public Scene
{
public:
	LoseScene();
	~LoseScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

private:
	Label* m_label{};
	Label* m_label2{};

	Button* m_pRestartButton;
};
#endif // !__LOSE_SCENE__
