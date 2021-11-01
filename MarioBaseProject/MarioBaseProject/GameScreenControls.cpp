#include "GameScreenControls.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include "Texture2D.h"
#include "GameScreenManager.h"
#include "Text.h"
#include "AnimatedSprite.h"


GameScreenControls::GameScreenControls(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpMenu();
}

GameScreenControls::~GameScreenControls()
{
	delete m_highscore_background;
	m_highscore_background = nullptr;

    delete m_coin;
    m_coin = nullptr;

    delete m_koopa;
    m_koopa = nullptr;
}

void GameScreenControls::Render()
{
	m_highscore_background->Render(Vector2D(0, 0), SDL_FLIP_NONE);
    m_koopa->Render();
    m_coin->Render();
}

void GameScreenControls::Update(float deltaTime, SDL_Event e)
{
    // If Space pressed, move on to menu
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

    // Update animations
    m_koopa->Update(deltaTime, e);
    m_coin->Update(deltaTime, e);
}

bool GameScreenControls::SetUpMenu()
{
    m_highscore_background = new Texture2D(m_renderer);
    if (!m_highscore_background->LoadFromFile("Images/ControlsBackground.png"))
    {
        std::cout << "Failed to load texture!" << std::endl;
    }


    m_koopa = new AnimatedSprite(m_renderer, "Images/Koopa.png", 3, 2, 4, Vector2D(394, 212), -1);
    m_koopa->SetAnimation(1);

    m_coin = new AnimatedSprite(m_renderer, "Images/Coin.png", 4, 1, 2, Vector2D(344, 354), -1);

    return true;
}

void GameScreenControls::ProceedScreen()
{
    GameScreenManager::getInstance()->SetSceneChange(SCREEN_MENU);
}
