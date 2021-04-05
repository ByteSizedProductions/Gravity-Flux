#pragma once
#ifndef __END_SCENE__
#define __END_SCENE__

#include "Scene.h"
#include "Label.h"
#include "Button.h"
#include "FlyingMarvins.h"
#include "StaticSprite.h"

class EndScene final : public Scene
{
public:
	EndScene();
	~EndScene();

	// Inherited via Scene
	virtual void draw() override;
	virtual void update() override;
	virtual void clean() override;
	virtual void handleEvents() override;
	virtual void start() override;

private:
	Button* m_pRestartButton;

	StaticSprite* m_pBackground;
	StaticSprite* m_pLogo;

	MarvinField* m_pMarvinField;

};

#endif /* defined (__END_SCENE__) */