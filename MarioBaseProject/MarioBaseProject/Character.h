#pragma once
#ifndef _CHARACTER_H
#define _CHARACTER_H
#include <SDL.h>
#include <iostream>
#include "Commons.h"
#include "Texture2D.h"

class Texture2D;
class LevelMap;

class Character
{
public:
	Character(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, Vector2D start_position, LevelMap* map);
	virtual ~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	virtual void Animation(float deltaTime, SDL_Event e);
	void SetPosition(Vector2D new_position);
	Vector2D GetPosition();
	float GetCollisionRadius();
	Rect2D GetCollisionBox()
	{
		return Rect2D(m_position.x, m_position.y, m_single_sprite_w, m_single_sprite_h);
	}
	
	Circle2D GetCollisionCircle()
	{
		return Circle2D(m_position.x, m_position.y, m_collision_radius);
	}


	void FlipFacing();
	bool IsJumping() { return m_jumping; };
	bool GetFalling();
	void CancelJump() { m_jumping = false; m_position.y += 1; };
	void SetAlive(bool isAlive) { m_alive = isAlive; };
	bool GetAlive() { return m_alive; };
	void SetDying(bool isDying) { m_death_animation = isDying; };
	bool GetDying() { return m_death_animation; };
	TilePosition GetTilePos();

protected:
	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Texture2D* m_texture;

	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	virtual void AddGravity(float deltaTime);
	virtual void Jump();
	virtual void DeathAnimation(float deltaTime);

	// Death animation variables
	bool m_death_animation;
	float m_death_offset;
	float m_death_time;

	bool m_moving_left;
	bool m_moving_right;

	float vely;
	bool m_jumping;
	bool m_can_jump;
	float m_jump_force;
	FACING m_facing_direction;

	float m_collision_radius;

	bool m_alive;
	int m_current_node;

	float m_single_sprite_w;
	float m_single_sprite_h;

	// Animations
	int m_current_animation;
	int m_current_keyframe;
	float m_animation_fps;
	float m_animation_deltatime;

	LevelMap* m_current_level_map;
private:
	//LevelMap* m_current_level_map;
};

#endif

