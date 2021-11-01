#include "AnimatedSprite.h"
#include "constants.h"
#include "LevelMap.h"
#include "SoundManager.h"
#include <math.h>


AnimatedSprite::AnimatedSprite(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, Vector2D start_position, float lifetime)
{
	m_renderer = renderer;
	m_position = start_position;
	m_texture = new Texture2D(m_renderer);
	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load texture!" << std::endl;
	}

	// Animation Varias
	m_total_anims = spriteSheetHeight;
	m_single_sprite_w = m_texture->GetWidth() / spriteSheetWidth; // Pass data in
	m_single_sprite_h = m_texture->GetHeight() / spriteSheetHeight; // Pass data in
	m_current_animation = 0;
	m_current_keyframe = 0;
	m_animation_fps = 1 / ((m_texture->GetWidth() / m_single_sprite_w) * keyframeSpeed); // Pass data in
	m_animation_deltatime = 0;

	m_facing_direction = FACING_RIGHT;

	m_lifetime = lifetime;
	m_delete = false;
}

AnimatedSprite::~AnimatedSprite()
{
	m_renderer = nullptr;
	delete m_texture;
	m_texture = nullptr;
}

void AnimatedSprite::Render()
{
	// Set the left and right start pointers on the spritesheet
	int left = m_single_sprite_w * m_current_keyframe;
	int right = m_single_sprite_h * m_current_animation;
	//get the portion of the sprite sheet you want to draw
	//							   {xPos, yPos, width of sprite, height of sprite}
	SDL_Rect portion_of_sprite = { left, right,m_single_sprite_w, m_single_sprite_h };

	//determine where you want it drawn
	SDL_Rect destRect = { (int)(m_position.x), (int)(m_position.y), m_single_sprite_w, m_single_sprite_h };


	//then draw it facing the correct direction
	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_HORIZONTAL);
	}
	else
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);
	}
}

void AnimatedSprite::Update(float deltaTime, SDL_Event e)
{
	// Kill sprite if thats wanted
	if (m_lifetime != -1 || m_lifetime > 0)
	{
		m_lifetime -= deltaTime;
	}

	if(m_lifetime > -1 && m_lifetime <= 0)
	{
		m_delete = true;
	}

	Animation(deltaTime, e);
}

void AnimatedSprite::Animation(float deltaTime, SDL_Event e)
{
	// Increment keyframe once the deltaTime reaches the needed for each swap
	if (m_animation_deltatime >= m_animation_fps)
	{
		m_animation_deltatime -= m_animation_fps;
		m_current_keyframe++;
	}

	// If last keyframe, reset it to 0
	if (m_current_keyframe == m_texture->GetWidth() / m_single_sprite_w)
	{
		m_current_keyframe = 0;
	}

	m_animation_deltatime += deltaTime;
}

void AnimatedSprite::ResetAnimations()
{
	m_current_keyframe = 0;
	m_animation_deltatime = 0;
}
