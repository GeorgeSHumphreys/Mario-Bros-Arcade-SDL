#pragma once
#ifndef _GAMESCREENMANAGER_H
#define _GAMESCREENMANAGER_H

#include <SDL.h>
#include "Commons.h"

class GameScreen;

class GameScreenManager
{
public:
	GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen);
	~GameScreenManager();

	static GameScreenManager* getInstance(); // Static means it will be able to be called on whichever class has the include

	void Render();
	void Update(float deltaTime, SDL_Event e);

	void ChangeScreen(SCREENS new_screen);
	void SetSceneChange(SCREENS new_screen);

	int num_players;
	int current_score;

private:
	static GameScreenManager* instance; // Static means this is globally set once, if requested it will always bring the same one

	SCREENS m_change_screen;
	SDL_Renderer* m_renderer;
	GameScreen* m_current_screen;
};

#endif