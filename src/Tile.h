#pragma once
#ifndef __TILE_H__
#define __TILE_H__
#include "DisplayObject.h"
#include "TileType.h"
#include "SDL.h"

class Tile : public DisplayObject {
public:
	Tile();
	Tile(TileType type, SDL_Rect src, SDL_Color color);
	~Tile();

	void draw() override;
	void update() override;
	void clean() override;

	TileType GetTileType();
	void SetTileType(TileType type);
	SDL_Color GetColor();
private:
	SDL_Rect* m_pRect;
	TileType m_type;
	SDL_Color m_color;
};

#endif // !__TILE_H__
