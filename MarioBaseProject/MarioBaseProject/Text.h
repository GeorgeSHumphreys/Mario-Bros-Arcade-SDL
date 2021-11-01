#pragma once
#ifndef _TEXT_H
#define _TEXT_H
#include <SDL.h>
#include <SDL_ttf.h>
#include "Commons.h"
#include <iostream>

class Text
{
public:
	Text(SDL_Renderer* renderer, Vector2D position, std::string text, std::string font, int fontSize, SDL_Color color);
	~Text();

	void Render();
	void SetText(std::string text);
	void SetPosition(Vector2D position) { m_position = position; m_dest_rect = { (int)m_position.x, (int)m_position.y, m_tex_w, m_tex_h };
	};

private:
	void TextureSetup();

	std::string m_text;
	SDL_Renderer* m_renderer;
	SDL_Color m_color;
	SDL_Surface* m_surface;
	SDL_Texture* m_texture;
	Vector2D m_position;

	int m_tex_w;
	int m_tex_h;

	SDL_Rect m_dest_rect;

	TTF_Font* m_font;
};

#endif