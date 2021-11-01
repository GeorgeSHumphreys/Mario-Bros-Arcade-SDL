#include "PowBlock.h"
#include "LevelMap.h"
#include <iostream>

PowBlock::PowBlock(SDL_Renderer* renderer, LevelMap* map, Vector2D position)
{
	std::string imagePath = "Images/PowBlock.png";
	m_texture = new Texture2D(renderer);
	if (!m_texture->LoadFromFile(imagePath.c_str()))
	{
		std::cout << "Failed to load texture." << std::endl;
		return;
	}

	m_level_map = map;
	m_single_sprite_w = m_texture->GetWidth() / 3; //there are three images in this sprite sheet in a row
	m_single_sprite_h = m_texture->GetHeight();
	m_num_hits_left = 3;
	m_position = position;
	m_x_tile_pos = position.x / TILE_WIDTH;
	m_y_tile_pos = position.y / TILE_WIDTH;
}

PowBlock::~PowBlock()
{
	m_renderer = nullptr;
	delete m_texture;
	m_texture = nullptr;
	m_level_map = nullptr;
}

void PowBlock::Render()
{
	if (m_num_hits_left > 0)
	{
		// Change rect depending on pow block damage
		SDL_Rect src = { (m_num_hits_left -1) * (m_texture->GetWidth() / 3), 0, m_texture->GetWidth() / 3, m_single_sprite_h }; // Selecting location on spriteSheet
		SDL_Rect dst = { m_position.x, m_position.y, m_single_sprite_w, m_single_sprite_h }; // x and y is position in world;
		m_texture->Render(src, dst, SDL_RendererFlip::SDL_FLIP_NONE, 0.f);
	}
}

void PowBlock::TakeHit()
{
	m_num_hits_left--;
	if (m_num_hits_left <= 0)
	{
		m_num_hits_left = 0;
		m_level_map->ChangeTileAt(m_y_tile_pos, m_x_tile_pos, 0);
	}
}