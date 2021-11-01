#include "GameScreenManager.h"
#include "GameScreen.h"
#include "GameScreenLevel1.h"
#include "GameScreenLevel2.h"
#include "GameScreenMenu.h"
#include "GameScreenHighscore.h"
#include "GameScreenControls.h"
#include "GameScreenGameOver.h"
#include <iostream>

GameScreenManager* GameScreenManager::instance = nullptr;

GameScreenManager::GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen)
{
	instance = this; // Set the static instance, so GetInstance will always return this 1 object
	m_renderer = renderer;
	m_current_screen = nullptr;
	m_change_screen = SCREEN_NULL; // Sets the screen change variabe to NULL
	ChangeScreen(startScreen);
	num_players = 1;
	current_score = 0;
}

GameScreenManager::~GameScreenManager()
{
	m_renderer = nullptr;
	delete m_current_screen;
	m_current_screen = nullptr;
}

GameScreenManager* GameScreenManager::getInstance() // If instance is requested, any function can have access
{
	return instance;
}

void GameScreenManager::SetSceneChange(SCREENS new_screen)
{
	m_change_screen = new_screen; // Sets the requested screen from an ingame object to this variable
}

void GameScreenManager::Render()
{
	m_current_screen->Render();
}

void GameScreenManager::Update(float deltaTime, SDL_Event e)
{
	if (m_change_screen == SCREEN_NULL) // If the scene to change to is NULL, keep playing
	{
		m_current_screen->Update(deltaTime, e);
	}
	else // If not, change to that screen, ATM theres no way to pass on info
	{
		ChangeScreen(m_change_screen);
		m_change_screen = SCREEN_NULL; // Reset Null Screen
	}
}

void GameScreenManager::ChangeScreen(SCREENS new_screen)
{
	// Clear old screen
	if (m_current_screen != nullptr)
	{
		delete m_current_screen;
	}

	GameScreenLevel1* tempLevel1;
	GameScreenLevel2* tempLevel2;
	// Load new screen depending on which enum is set
	switch (new_screen)
	{
	case SCREEN_LEVEL1:
		tempLevel1 = new GameScreenLevel1(m_renderer);
		tempLevel1->SetLevelMap("Levels/Level1.txt");
		tempLevel1->SetUpLevel();
		m_current_screen = (GameScreen*)tempLevel1;
		tempLevel1 = nullptr;
		break;
	case SCREEN_MENU:
		m_current_screen = (GameScreen*)(new GameScreenMenu(m_renderer));
		break;
	case SCREEN_HIGHSCORES:
		m_current_screen = (GameScreen*)(new GameScreenHighscore(m_renderer));
		break;
	case SCREEN_CONTROLS:
		m_current_screen = (GameScreen*)(new GameScreenControls(m_renderer));
		break;
	case SCREEN_LEVEL2:
		tempLevel2 = new GameScreenLevel2(m_renderer);
		tempLevel2->SetLevelMap("Levels/Level2.txt");
		tempLevel2->SetUpLevel();
		m_current_screen = (GameScreen*)tempLevel2;
		tempLevel2 = nullptr;
		break;
	case SCREEN_GAMEOVER:
		m_current_screen = (GameScreen*)(new GameScreenGameOver(m_renderer));
		break;
	default:;
	}

}