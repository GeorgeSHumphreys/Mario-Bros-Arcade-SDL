#include "SoundManager.h"
#include <iostream>

SoundManager* SoundManager::instance = nullptr;

SoundManager::SoundManager()
{
	instance = this; // Set the static instance, so GetInstance will always return this 1 object
	m_background_music = LoadMusic("Music/Mario.mp3");
    m_background_music_underworld = LoadMusic("Music/MarioUnderworld.mp3");
    m_mario_jump = LoadSound("Music/MarioJump.mp3");
    m_coin = LoadSound("Music/Coin.wav");
    m_walk = LoadSound("Music/Walk.mp3");
    m_kick = LoadSound("Music/Kick.wav");
    m_bump = LoadSound("Music/Bump.wav");
    m_damage = LoadSound("Music/PlayerDamage.wav");
    m_menu_move = LoadSound("Music/MenuMove.wav");
}

SoundManager::~SoundManager()
{
	
}

void SoundManager::PlaySound(Mix_Chunk* sound)
{
    Mix_PlayChannel(-1, sound, 0);
}

void SoundManager::PlayMusic(Mix_Music* sound, int loop)
{
    Mix_PlayMusic(sound, loop);
}

Mix_Music* SoundManager::LoadMusic(std::string path)
{
    Mix_Music* g_music;
    g_music = Mix_LoadMUS(path.c_str());
    if (g_music == nullptr)
    {
        std::cout << "Failed to load music. Error: " << Mix_GetError() << std::endl;
    }
    return g_music;
}

Mix_Chunk* SoundManager::LoadSound(std::string path)
{
    Mix_Chunk* g_music;
    g_music = Mix_LoadWAV(path.c_str());
    if (g_music == nullptr)
    {
        std::cout << "Failed to load music. Error: " << Mix_GetError() << std::endl;
    }
    return g_music;
}

SoundManager* SoundManager::getInstance() // If instance is requested, any function can have access
{
	return instance;
}
