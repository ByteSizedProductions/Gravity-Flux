#pragma once
#ifndef __GAME_OBJECT_TYPE__
#define __GAME_OBJECT_TYPE__
enum GameObjectType {
	NONE = -1,
	SHIP,
	TARGET,
	PLAYER,
	DOOR,
	HEART,
	PLANE,
	BOMB,
	PICKUP,
	TILE,
	START_BUTTON,
	RESTART_BUTTON,
	BACK_BUTTON,
	NEXT_BUTTON,
	SETTINGS_BUTTON,
	EXIT_BUTTON,
	RESUME_BUTTON,
	BACKGROUND,
	PAUSE_MENU,
	USERINTERFACE,
	UILABEL,
	NUM_OF_TYPES,
	BRAIN,
	ENEMY,
	ABILITYBAR
};
#endif /* defined (__GAME_OBJECT_TYPE__) */