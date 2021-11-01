#pragma once
#ifndef _COIN_H
#define _COIN_H
#include "Character.h"
#include "Commons.h"
#include "constants.h"

class Coin : public Character
{
public:
	Coin(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed);
	void Update(float deltaTime, SDL_Event e) override;
	void MoveLeft(float deltaTime) override;
	void MoveRight(float deltaTime) override;


	void TakeDamage();
	void Jump();

private:
	float m_movement_speed;
};

#endif