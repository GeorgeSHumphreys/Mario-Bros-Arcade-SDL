#pragma once
#ifndef _CHARACTERKOOPA_H
#define _CHARACTERKOOPA_H
#include "Character.h"
#include "Commons.h"
#include "constants.h"

class CharacterKoopa : public Character
{
public:
	CharacterKoopa(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed);
	void Update(float deltaTime, SDL_Event e) ;
	void Render();
	void MoveLeft(float deltaTime) override;
	void MoveRight(float deltaTime) override;
	bool GetInjured() { return m_injured; };
	int GetScore() { return m_score_amount; };


	virtual void TakeDamage();
	void Jump();

protected:
	bool m_injured;
	float m_injured_time;
	
	float m_movement_speed;

	int m_score_amount;

	virtual void FlipRightwayUp();
};

#endif