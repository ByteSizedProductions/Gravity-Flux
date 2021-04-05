#pragma once
#ifndef __LOADING_SCENE__
#define __LOADING_SCENE__
#include "Scene.h"
#include "Label.h"
#include "Button.h"
#include "FlyingMarvins.h"
#include "StaticSprite.h"

class LoadingScene final : public Scene
{
public:
	LoadingScene();
	~LoadingScene();

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

	StaticSprite* m_pBackground;

	MarvinField* m_pMarvinField;
};

#endif // !__LOADING_SCENE__

