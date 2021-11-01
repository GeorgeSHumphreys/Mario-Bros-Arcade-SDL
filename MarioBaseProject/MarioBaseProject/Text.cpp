#include "Text.h"


Text::Text(SDL_Renderer* renderer, Vector2D position, std::string text, std::string font, int fontSize, SDL_Color color)
{
	// Text Setup
	m_renderer = renderer;
	m_position = position;
	m_text = text;
	m_font = TTF_OpenFont(font.c_str(), fontSize);
	m_color = color;
	TextureSetup();
}

Text::~Text()
{
	// Clean up
	TTF_CloseFont(m_font);
	m_font = nullptr;

	SDL_FreeSurface(m_surface);
	m_surface = nullptr;

	SDL_DestroyTexture(m_texture);
	m_texture = nullptr;

	m_renderer = nullptr;
}

void Text::SetText(std::string text)
{
	// Destroys surface and remakes with new text
	m_text = text;
	SDL_FreeSurface(m_surface);
	SDL_DestroyTexture(m_texture);
	TextureSetup();
}

void Text::TextureSetup()
{
	// Set up texture
	m_surface = TTF_RenderText_Solid(m_font, m_text.c_str(), m_color);
	m_texture = SDL_CreateTextureFromSurface(m_renderer, m_surface);
	SDL_QueryTexture(m_texture, NULL, NULL, &m_tex_w, &m_tex_h);
	m_dest_rect = { (int)m_position.x, (int)m_position.y, m_tex_w, m_tex_h };
}

void Text::Render()
{
	// Draw text to the screen
	SDL_RenderCopy(m_renderer, m_texture, NULL, &m_dest_rect);
}