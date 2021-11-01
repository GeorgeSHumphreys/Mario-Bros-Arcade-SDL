#pragma once
#ifndef _ANIMATEDSPRITE_H
#define _ANIMATEDSPRITE_H
#include <SDL.h>
#include <iostream>
#include "Commons.h"
#include "Texture2D.h"

class Texture2D;

class AnimatedSprite
{
public:
	AnimatedSprite(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, Vector2D start_position, float lifetime);
	~AnimatedSprite();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	void Animation(float deltaTime, SDL_Event e);
	void SetPos(Vector2D position) { m_position = position; };
	void ResetAnimations();
	bool GetDelete() { return m_delete; };
	void SetAnimation(int anim) { m_current_animation = anim; };
private:
	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Texture2D* m_texture;

	FACING m_facing_direction;

	float m_lifetime;
	bool m_delete;

	int m_total_anims;
	float m_single_sprite_w;
	float m_single_sprite_h;

	// Animations
	int m_current_animation;
	int m_current_keyframe;
	float m_animation_fps;
	float m_animation_deltatime;
};

#endif