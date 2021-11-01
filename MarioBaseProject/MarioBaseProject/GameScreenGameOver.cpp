#include "GameScreenGameOver.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include "Texture2D.h"
#include "GameScreenManager.h"
#include "Text.h"
#include "AnimatedSprite.h"
#include "SoundManager.h"

GameScreenGameOver::GameScreenGameOver(SDL_Renderer* renderer) : GameScreen(renderer)
{
    Mix_PauseMusic();
	SetUpMenu();
}

GameScreenGameOver::~GameScreenGameOver()
{
	delete m_highscore_background;
	m_highscore_background = nullptr;

    delete m_score;
    m_score = nullptr;
}

void GameScreenGameOver::Render()
{
	m_highscore_background->Render(Vector2D(0, 0), SDL_FLIP_NONE);
    m_score->Render();
}

void GameScreenGameOver::Update(float deltaTime, SDL_Event e)
{
    // Space pressed  = proceed screed
    switch (e.type)
    {
    case SDL_KEYDOWN:
        switch (e.key.keysym.sym)
        {
        case SDLK_SPACE:
            ProceedScreen();
            break;
        }
        break;
    }
}

bool GameScreenGameOver::SetUpMenu()
{
    m_highscore_background = new Texture2D(m_renderer);
    if (!m_highscore_background->LoadFromFile("Images/GameOver.png"))
    {
        std::cout << "Failed to load texture!" << std::endl;
        return false;
    }

    m_score = new Text(m_renderer, Vector2D(124, 242), "Your Score was : " + std::to_string(GameScreenManager::getInstance()->current_score), "Pixel.ttf", 18, { 255, 255, 255 });

    return true;
}
void GameScreenGameOver::ProceedScreen()
{
    GameScreenManager::getInstance()->current_score = 0;
    GameScreenManager::getInstance()->SetSceneChange(SCREEN_MENU);
}