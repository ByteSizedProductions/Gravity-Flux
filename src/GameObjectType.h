#pragma once
#ifndef __GAME_OBJECT_TYPE__
#define __GAME_OBJECT_TYPE__
enum GameObjectType {
	NONE = -1,
	SHIP,
	TARGET,
	PLAYER,
	DOOR,
	PLANE,
	BOMB,
	PICKUP,
	PLATFORM,
	START_BUTTON,
	RESTART_BUTTON,
	BACK_BUTTON,
	NEXT_BUTTON,
	SETTINGS_BUTTON,
	EXIT_BUTTON,
	RESUME_BUTTON,
	PAUSE_MENU,
	NUM_OF_TYPES
};
#endif /* defined (__GAME_OBJECT_TYPE__) */