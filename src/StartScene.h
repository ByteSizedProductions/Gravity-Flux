#pragma once
#ifndef __START_SCENE__
#define __START_SCENE__

#include "Scene.h"
#include "Label.h"
#include "ship.h"
#include "Button.h"
#include "Marvin.h"
#include "StaticSprite.h"
#include "FlyingMarvins.h"

class StartScene final : public Scene
{
public:
	StartScene();
	~StartScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;
	
private:
	Button* m_pStartButton;
	Button* m_pSettingsButton;
	Button* m_pExitButton;

	StaticSprite* m_pBackground;
	StaticSprite* m_pLogo;

	MarvinField* m_pMarvinField;
};

#endif /* defined (__START_SCENE__) */