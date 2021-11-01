#include "Character.h"
#include "constants.h"
#include "LevelMap.h"
#include "SoundManager.h"
#include <math.h>

Character::Character(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, Vector2D start_position, LevelMap* map)
{
	m_renderer = renderer;
    m_current_level_map = map;
	m_position = start_position;
	m_texture = new Texture2D(m_renderer);
	if (!m_texture->LoadFromFile(imagePath))
	{
		std::cout << "Failed to load texture!" << std::endl;
	}

    m_facing_direction = FACING_RIGHT;

    m_moving_left = false;
    m_moving_right = false;

    m_jumping = false;
    m_can_jump = false;
    m_jump_force = 0;

    m_collision_radius = 15.0f;

    m_alive = true;


    // Set Animation Variables
    m_single_sprite_w = m_texture->GetWidth() / spriteSheetWidth; // Pass data in
    m_single_sprite_h = m_texture->GetHeight() / spriteSheetHeight; // Pass data in
    m_current_animation = 0;
    m_current_keyframe = 0;
    m_animation_fps = 1 / ((m_texture->GetWidth() / m_single_sprite_w) * keyframeSpeed); // Pass data in
    m_animation_deltatime = 0;


    m_death_animation = false;
    m_death_offset = 0;
    m_death_time = 0;
}

Character::~Character()
{
	m_renderer = nullptr;
    delete m_texture;
    m_texture = nullptr;
}

void Character::Render()
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
    if (m_facing_direction == FACING_RIGHT)
    {
        m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_HORIZONTAL);
    }
    else
    {
        m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);
    }
}

void Character::Update(float deltaTime, SDL_Event e)
{
    if (!m_death_animation) // Death animation stops all normal movement functions
    {
        // deal with jumps
        if (m_jumping)
        {
            // Change position
            m_position.y -= m_jump_force * deltaTime;

            // reduce jump force
            m_jump_force -= JUMP_FORCE_DECREMENT * deltaTime;

            // is Jump force 0?
            if (m_jump_force <= 0.0f)
                m_jumping = false;

        }
        else
        {
            m_jump_force = 0;
        }

        //collision position variables
        int lPosTile = (int)(m_position.x) / TILE_WIDTH;
        int rPosTile = (int)(m_position.x + m_single_sprite_w) / TILE_WIDTH;

        int foot_position = (int)(m_position.y + m_single_sprite_h) / TILE_HEIGHT;
        int upperfoot_position = (int)(m_position.y - 2 + m_single_sprite_h) / TILE_HEIGHT;
        int middle_position = (int)(m_position.y + (m_single_sprite_h / 2)) / TILE_HEIGHT;
        int head_position = m_position.y / TILE_HEIGHT;

        // Checks for Left + right collision
        int tileMiddleL = m_current_level_map->GetTileAt(middle_position, lPosTile);
        int tileMiddleR = m_current_level_map->GetTileAt(middle_position, rPosTile);
        int tileFootL = m_current_level_map->GetTileAt(upperfoot_position, lPosTile);
        int tileFootR = m_current_level_map->GetTileAt(upperfoot_position, rPosTile);

        // Currently, there is a bug where the Movement will place inside the tile, causing the Snapping feature in the gravity section to snap the player to the top of the tile

        if (m_moving_left)
        {
            m_facing_direction = FACING_LEFT;
            if ((tileMiddleL == 0) && (tileFootL == 0))
            {
                MoveLeft(deltaTime);
            }
            else
            {
                MoveRight(deltaTime);
            }
        }
        else if (m_moving_right)
        {
            m_facing_direction = FACING_RIGHT;
            if ((tileMiddleR == 0) && (tileFootR == 0))
            {
                MoveRight(deltaTime);
            }
            else
            {
                MoveLeft(deltaTime);
            }
        }

        //deal with gravity
        if (m_current_level_map->GetTileAt(foot_position, lPosTile) == 0 && m_current_level_map->GetTileAt(foot_position, rPosTile) == 0)
        {
            AddGravity(deltaTime);
            m_can_jump = false;
        }
        else
        {
            //collided with ground so we can jump again
            m_can_jump = true;
            if (m_jump_force < GRAVITY)
            {
                m_jumping = false;
                m_position.y = (foot_position * 32) - m_single_sprite_h; // Snap to floor to stop clipping
            }
        }
        Animation(deltaTime, e);
    }
    else
    {
        DeathAnimation(deltaTime);
    }
   
}

bool Character::GetFalling()
{
    if (m_jump_force < GRAVITY)
    {
        return true;
    }
    return false;
}

void Character::SetPosition(Vector2D new_position)
{
    m_position = new_position;
}

Vector2D Character::GetPosition()
{
    return m_position;
}

float Character::GetCollisionRadius()
{
    return m_collision_radius;
}

TilePosition Character::GetTilePos()
{
    TilePosition buffer = { m_position.y / TILE_HEIGHT,  (int)(m_position.y + m_single_sprite_h) / TILE_HEIGHT, m_position.x / TILE_WIDTH, (m_position.x + m_single_sprite_w) / TILE_WIDTH };
    return buffer;
}

void Character::MoveLeft(float deltaTime)
{
    m_position.x -= deltaTime * MOVESPEED;
}

void Character::MoveRight(float deltaTime)
{
    m_position.x += deltaTime * MOVESPEED;
}

void Character::AddGravity(float deltaTime)
{
    if (m_position.y + m_texture->GetHeight() / m_single_sprite_h <= SCREEN_HEIGHT)
    {
        m_position.y += deltaTime * GRAVITY;
    }
}

void Character::FlipFacing()
{
    if (m_facing_direction == FACING_LEFT) m_facing_direction = FACING_RIGHT;
    else m_facing_direction = FACING_LEFT;
}

void Character::Jump()
{
    if (!m_jumping)
    {
        SoundManager::getInstance()->PlaySound(SoundManager::getInstance()->m_mario_jump);
        m_jump_force = INITIAL_JUMP_FORCE;
        m_can_jump = false;
        m_jumping = true;
    }
}

void Character::Animation(float deltaTime, SDL_Event e)
{
    // Increment keyframe once the deltaTime reaches the needed for each swap
    if (m_animation_deltatime >= m_animation_fps)
    {
        m_animation_deltatime -= m_animation_fps;
        m_current_keyframe++;
    }

    // If last keyframe, reset it to 0
    if (m_current_keyframe == m_texture->GetWidth() / m_single_sprite_w)
    {
        m_current_keyframe = 0;
    }

    m_animation_deltatime += deltaTime;
}

void Character::DeathAnimation(float deltaTime)
{
    // Bobs the player up then down to the bottom, and then kills itself
    m_position.y += (-250 + m_death_offset) * deltaTime;
    m_death_offset += 500 * deltaTime;
    m_position.x += 30 * deltaTime;
    if (m_position.y > SCREEN_HEIGHT)
        m_alive = false;

    m_death_time += deltaTime;
}

