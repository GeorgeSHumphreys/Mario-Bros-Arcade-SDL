#pragma once
#ifndef _GAMESCREEN_H
#define _GAMESCREEN_H
#include <SDL.h>
#include "Commons.h"

class GameScreen
{
public:
	GameScreen(SDL_Renderer* renderer);
	virtual ~GameScreen();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);

	SCREENS change_screen; // If this variable is set to a screen ENUM, the GAMESCREENMANAGER, will load it

protected:
	SDL_Renderer* m_renderer;
};

#endif