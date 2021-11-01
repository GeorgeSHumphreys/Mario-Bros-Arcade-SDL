#include "Coin.h"


Coin::Coin(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed) : Character(renderer, imagePath, spriteSheetWidth, spriteSheetHeight, keyframeSpeed, start_position, map)
{
	m_facing_direction = start_facing;
	m_movement_speed = movement_speed;
	m_position = start_position;
}

void Coin::Update(float deltaTime, SDL_Event e)
{
	if (m_alive == false)
	{
		m_position.x = -100;
	}

	//enemy is not injured so move
	if (m_facing_direction == FACING_LEFT)
	{
		m_moving_left = true;
		m_moving_right = false;
	}
	else if (m_facing_direction == FACING_RIGHT)
	{
		m_moving_right = true;
		m_moving_left = false;
	}

	if (m_can_jump)
	{
		Jump();
	} 
	Character::Update(deltaTime, e);


	

	// Screen Wrapping
	if (m_position.x < 0)
	{
		m_position.x = SCREEN_WIDTH;
	}
	else if (m_position.x >= SCREEN_WIDTH)
	{
		m_position.x = 0;
	}
}

void Coin::TakeDamage()
{
	m_alive = false;
}

void Coin::Jump()
{
	if (!m_jumping)
	{
		m_jump_force = INITIAL_JUMP_FORCE_SMALL;
		m_jumping = true;
		m_can_jump = false;
	}
}

void Coin::MoveLeft(float deltaTime)
{
	m_position.x -= deltaTime * m_movement_speed;
	m_facing_direction = FACING_LEFT;
}
void Coin::MoveRight(float deltaTime)
{
	m_position.x += deltaTime * m_movement_speed;
	m_facing_direction = FACING_RIGHT;
}
