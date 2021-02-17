#pragma once
#ifndef __PAUSEMENU__
#define __PAUSEMENU__

#include "Button.h"
#include "DisplayObject.h"
#include "TextureManager.h"

class PauseMenu final : public DisplayObject
{
public:
	PauseMenu();
	~PauseMenu();
	void draw() override;
	void update() override;
	void clean() override;

	void setPaused(bool paused);
	bool getPaused() const;
private:
	bool m_paused = false;

	Button* m_pResumeButton;
	Button* m_pSettingsButton;
	Button* m_pExitButton;
};


#endif /* DEFINED (__PAUSEMENU__) */