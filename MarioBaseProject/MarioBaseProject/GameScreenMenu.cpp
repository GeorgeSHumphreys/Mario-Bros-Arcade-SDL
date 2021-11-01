#include "GameScreenMenu.h"
#include <iostream>
#include <time.h>
#include <string>
#include "Texture2D.h"
#include "GameScreenManager.h"
#include "Text.h"
#include "SoundManager.h"

GameScreenMenu::GameScreenMenu(SDL_Renderer* renderer) : GameScreen(renderer)
{
	SetUpMenu();
}

GameScreenMenu::~GameScreenMenu()
{
    delete m_menu_title;
    m_menu_title = nullptr;
    delete m_cursor;
    m_cursor = nullptr;
}

void GameScreenMenu::Render()
{
	m_menu_title->Render(Vector2D(0, 0), SDL_FLIP_NONE);
    m_cursor->Render();
}

void GameScreenMenu::Update(float deltaTime, SDL_Event e)
{
    switch (e.type)
    {
        // Handle up and down pressed, as well as proceeding screen
    case SDL_KEYDOWN:
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            m_current_choice -= 1;
            SoundManager::getInstance()->PlaySound(SoundManager::getInstance()->m_menu_move);
            break;
        case SDLK_DOWN:
            m_current_choice += 1;
            SoundManager::getInstance()->PlaySound(SoundManager::getInstance()->m_menu_move);
            break;
        case SDLK_SPACE:
            ProceedScreen();
            break;
        }
        break;
    }

    if (m_current_choice == m_menu_count) m_current_choice = 0;
    else if (m_current_choice < 0) m_current_choice = m_menu_count-1;

    m_cursor->SetPosition(Vector2D(118, 172 + (m_current_choice * 28)));
}

void GameScreenMenu::ProceedScreen()
{
    // Proceed screen logic
    if (m_current_choice == 0)
    {
        GameScreenManager::getInstance()->num_players = 1;
        GameScreenManager::getInstance()->SetSceneChange(SCREEN_LEVEL1);
    }
    else if (m_current_choice == 1)
    {
        GameScreenManager::getInstance()->num_players = 2;
        GameScreenManager::getInstance()->SetSceneChange(SCREEN_LEVEL1);
    }
    else if (m_current_choice == 2)
    {
        GameScreenManager::getInstance()->SetSceneChange(SCREEN_HIGHSCORES);
    }
    else if (m_current_choice == 3)
    {
        GameScreenManager::getInstance()->SetSceneChange(SCREEN_CONTROLS);
    }
}

bool GameScreenMenu::SetUpMenu()
{
	m_menu_title = new Texture2D(m_renderer);
	if (!m_menu_title->LoadFromFile("Images/MarioTitle.png"))
	{
		std::cout << "Failed to load texture!" << std::endl;
	}

    m_cursor = new Text(m_renderer, Vector2D(118, 184), ">", "Pixel.ttf", 25, { 255, 255, 255 });

    m_menu_count = 4;

	return true;
}
