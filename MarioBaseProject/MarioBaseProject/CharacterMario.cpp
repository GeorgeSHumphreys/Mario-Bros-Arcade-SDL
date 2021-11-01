#include "CharacterMario.h"
#include "Score.h"
#include "Text.h"
#include "GameScreenManager.h"
#include "LevelMap.h"
#include "SoundManager.h"

CharacterMario::CharacterMario(SDL_Renderer* renderer, std::string imagePath, int spriteSheetWidth, int spriteSheetHeight, float keyframeSpeed, Vector2D start_position, LevelMap* map) : Character(renderer, imagePath, spriteSheetWidth, spriteSheetHeight, keyframeSpeed, start_position, map)
{
    m_lives = 3;

    m_lives_texture = new Texture2D(m_renderer);
    if (!m_lives_texture->LoadFromFile("Images/MarioLives.png"))
    {
        std::cout << "Failed to load background texture!" << std::endl;
    }

    m_walk_sound_wait = 0.1;
}

CharacterMario::~CharacterMario()
{
    delete m_lives_texture;
    m_lives_texture = nullptr;
}

void CharacterMario::Update(float deltaTime, SDL_Event e)
{
    // Handle Events
    switch (e.type)
    {
    case SDL_KEYDOWN:
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT:
            m_moving_left = true;
            break;
        case SDLK_RIGHT:
            m_moving_right = true;
            break;
        case SDLK_UP:
            if (m_can_jump) Jump();
            break;
        }
        break;
    case SDL_KEYUP:
        switch (e.key.keysym.sym)
        {
        case SDLK_LEFT:
            m_moving_left = false;
            break;
        case SDLK_RIGHT:
            m_moving_right = false;
            break;
        }
        break;
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

    // HANDLE ANIMATION
    if (m_death_animation)
    {
        m_current_animation = 3;
    }
    else if (m_jumping)
    {
        m_current_animation = 2;
    }
    else if (m_moving_left == true || m_moving_right == true)
    {
        m_current_animation = 1;
    }
    else
    {
        m_current_animation = 0;
    }

    // Handle Death
    if (m_alive == false)
    {
        m_lives -= 1;
        m_alive = true;
        m_death_animation = false;
        m_death_offset = 0;
        m_position.x = 32;
        m_position.y = 144;
        m_jumping = false;
    }

    if (m_lives <= 0)
    {
        m_position.x = -100;
    }

    // Walking sound
    if ((m_moving_left || m_moving_right) && m_walk_sound_wait <= 0 && !m_jumping)
    {
        SoundManager::getInstance()->PlaySound(SoundManager::getInstance()->m_walk);
        m_walk_sound_wait = 0.1;
    }
    m_walk_sound_wait -= deltaTime;
}
