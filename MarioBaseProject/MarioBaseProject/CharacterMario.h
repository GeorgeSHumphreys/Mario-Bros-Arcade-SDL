#pragma once
#ifndef _CHARACTERMARIO_H
#define _CHARACTERMARIO_H
#include "Character.h"
#include "constants.h"

class Text;
class Score;

class CharacterMario : public Character
{
public:
	CharacterMario(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, Vector2D start_position, LevelMap* map);
	~CharacterMario();
	void Update(float deltaTime, SDL_Event e) override;
	int GetLives() { return m_lives; };
	bool GetJump() { return m_jumping; };
	void SetLives(int lives) { m_lives = lives; };
private:
	int m_lives;
	float m_walk_sound_wait;
	Texture2D* m_lives_texture;
};

#endif
