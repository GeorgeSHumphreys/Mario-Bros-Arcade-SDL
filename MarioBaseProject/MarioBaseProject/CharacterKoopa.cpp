#include "CharacterKoopa.h"
#include "LevelMap.h"


CharacterKoopa::CharacterKoopa(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, LevelMap* map, Vector2D start_position, FACING start_facing, float movement_speed) : Character(renderer, imagePath, spriteSheetWidth, spriteSheetHeight, keyframeSpeed, start_position, map)
{
	m_facing_direction = start_facing;
	m_movement_speed = movement_speed;
	m_position = start_position;
	m_injured = false;
	m_score_amount = 500;
}

void CharacterKoopa::Render()
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
	if (m_facing_direction == FACING_LEFT)
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_HORIZONTAL);
	}
	else
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);
	}
}

void CharacterKoopa::Update(float deltaTime, SDL_Event e)
{	
	if (!m_injured)
	{
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
	}
	else
	{
		//we should not be moving when injured
		m_moving_right = false;
		m_moving_left = false;

		if (!m_death_animation) // If the death animation isn't in progress, it counts the injured time, AKA, doesnt flip on death animation
		{
			//count down the injured time
			m_injured_time -= deltaTime;

			if (m_injured_time <= 0.0)
				FlipRightwayUp();
		}
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

	if (m_injured)
	{
		m_current_animation = 1;
	}
	else
	{
		m_current_animation = 0;
	}
}

void CharacterKoopa::TakeDamage()
{
	m_injured = true;
	m_injured_time = INJURED_TIME;
	Jump();
}

void CharacterKoopa::Jump()
{
	if (!m_jumping)
	{
		m_jump_force = INITIAL_JUMP_FORCE_SMALL;
		m_jumping = true;
		m_can_jump = false;
	}
}

void CharacterKoopa::MoveLeft(float deltaTime)
{
	m_position.x -= deltaTime * m_movement_speed;
}
void CharacterKoopa::MoveRight(float deltaTime)
{
	m_position.x += deltaTime * m_movement_speed;
}

void CharacterKoopa::FlipRightwayUp()
{
	if (m_facing_direction == FACING_LEFT) m_facing_direction = FACING_RIGHT;
	else m_facing_direction = FACING_LEFT;
	m_injured = false;
}
