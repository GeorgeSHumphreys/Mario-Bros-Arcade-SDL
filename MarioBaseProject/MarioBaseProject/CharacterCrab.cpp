#include "CharacterCrab.h"
#include <iostream>

CharacterCrab::CharacterCrab(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed) : CharacterKoopa(renderer, imagePath, spriteSheetWidth, spriteSheetHeight, keyframeSpeed, map, start_position, start_facing, movement_speed)
{
    m_mad = false;
    m_score_amount = 1000;
}

void CharacterCrab::Update(float deltaTime, SDL_Event e)
{
	CharacterKoopa::Update(deltaTime, e);

    // If it is mad, change move speed
    if (m_mad)
    {
        m_movement_speed = MAD_CRAB_SPEED;
        m_current_animation = 1;
    }
    else
    {
        m_current_animation = 0;
        m_movement_speed = KOOPA_SPEED;
    }

    // Change animation to row 3 if injured
    if (m_injured)
        m_current_animation = 2;
}

void CharacterCrab::TakeDamage()
{
    // Set to mad if not already, if not injure
    if (!m_mad)
    {
        m_mad = true;
        Jump();
    }
    else
    {
        m_injured = true;
        m_injured_time = INJURED_TIME;
        Jump();
    }
}

void CharacterCrab::FlipRightwayUp()
{
    if (m_facing_direction == FACING_LEFT) m_facing_direction = FACING_RIGHT;
    else m_facing_direction = FACING_LEFT;
    m_injured = false;
    m_mad = false;
}