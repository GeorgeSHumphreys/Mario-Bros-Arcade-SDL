#pragma once
#include "Character.h"
#include "CharacterKoopa.h"
#include "Commons.h"
#include "constants.h"

class CharacterCrab : public CharacterKoopa
{
public:
	CharacterCrab(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed);
	void Update(float deltaTime, SDL_Event e) override;
	void TakeDamage() override;
private:
	void FlipRightwayUp() override;
	bool m_mad;
};

