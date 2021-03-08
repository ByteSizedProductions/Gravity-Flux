#pragma once
#ifndef __TILE_H__
#define __TILE_H__
#include "Sprite.h"
#include "TileType.h"
#include "SDL.h"

class Tile : public Sprite {
public:
	Tile();
	Tile(TileType type, SDL_Rect* src);
	~Tile();

	void draw() override;
	void update() override;
	void clean() override;

	TileType GetTileType();
	void SetTileType(TileType type);
	SDL_Rect* GetSource();
private:
	SDL_Rect* m_pSrc;
	TileType m_type;
};

#endif // !__TILE_H__
