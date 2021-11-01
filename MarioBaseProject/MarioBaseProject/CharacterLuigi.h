#pragma once
#ifndef _CHARACTERLUIGI_H
#define _CHARACTERLUIGI_H
#include "Character.h"

class CharacterLuigi : public Character
{
public:
	CharacterLuigi(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, Vector2D start_position, LevelMap* map);
	~CharacterLuigi();
	void Update(float deltaTime, SDL_Event e) override;
	int GetLives() { return m_lives; };
	void SetLives(int lives) { m_lives = lives; };
private:
	int m_lives;
	Texture2D* m_lives_texture;

	float m_walk_sound_wait;
};

#endif