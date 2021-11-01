#pragma once
#ifndef _POWBLOCK_H
#define _POWBLOCK_H

#include <SDL.h>
#include "Commons.h"
#include "Texture2D.h"

class LevelMap;
class Texture2D;

class PowBlock
{
public:
	PowBlock(SDL_Renderer* renderer, LevelMap* map, Vector2D position);
	~PowBlock();
	void Render();
	void TakeHit();
	Rect2D GetCollisionBox() { return Rect2D(m_position.x, m_position.y, m_texture->GetWidth() / 3, m_texture->GetHeight()); };
	bool IsAvailable() { return m_num_hits_left > 0; }

private:
	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Texture2D* m_texture;
	LevelMap* m_level_map;

	float m_single_sprite_w;
	float m_single_sprite_h;
	int m_x_tile_pos;
	int m_y_tile_pos;
	int m_num_hits_left;
	int tilePosX, tilePosY;
};

#endif
