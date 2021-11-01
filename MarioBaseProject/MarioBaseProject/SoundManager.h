#pragma once
#ifndef _SOUNDMANAGER_H
#define _SOUNDMANAGER_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>
#include "Commons.h"


class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	static SoundManager* getInstance(); // Static means it will be able to be called on whichever class has the include

	void PlayMusic(Mix_Music* sound, int loop);
	void PlaySound(Mix_Chunk* sound);


	Mix_Music* m_background_music;
	Mix_Music* m_background_music_underworld;
	Mix_Chunk* m_mario_jump;
	Mix_Chunk* m_coin;
	Mix_Chunk* m_walk;
	Mix_Chunk* m_kick;
	Mix_Chunk* m_bump;
	Mix_Chunk* m_damage;
	Mix_Chunk* m_menu_move;

private:
	static SoundManager* instance; // Static means this is globally set once, if requested it will always bring the same one

	Mix_Music* LoadMusic(std::string path);
	Mix_Chunk* LoadSound(std::string path);
};

#endif