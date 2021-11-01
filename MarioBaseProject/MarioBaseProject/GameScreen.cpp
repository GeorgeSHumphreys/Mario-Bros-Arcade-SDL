#include "GameScreen.h"
#include <iostream>

GameScreen::GameScreen(SDL_Renderer* renderer)
{
	m_renderer = renderer;
	change_screen = SCREEN_NULL;
}

GameScreen::~GameScreen()
{
	m_renderer = nullptr;
}

void GameScreen::Render(){}

void GameScreen::Update(float deltaTime, SDL_Event e){}