#include "GameScreenHighscore.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <string>
#include "Texture2D.h"
#include "GameScreenManager.h"
#include "Text.h"

GameScreenHighscore::GameScreenHighscore(SDL_Renderer* renderer) : GameScreen(renderer)
{
    SetUpMenu();
}

GameScreenHighscore::~GameScreenHighscore()
{
    delete m_highscore_background;
    m_highscore_background = nullptr;
    
    for (int i = 0; i < 8; i++)
    {
        delete m_scores_text[i];
        m_scores_text[i] = nullptr;
    }
}

void GameScreenHighscore::Render()
{
    m_highscore_background->Render(Vector2D(0, 0), SDL_FLIP_NONE);

    for (int i = 0; i < 8; i++)
    {
        m_scores_text[i]->Render();
    }
}

void GameScreenHighscore::Update(float deltaTime, SDL_Event e)
{
    // Space pressed = proceed screen
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

void GameScreenHighscore::ProceedScreen()
{
    GameScreenManager::getInstance()->SetSceneChange(SCREEN_MENU);
}

bool GameScreenHighscore::SetUpMenu()
{
    m_highscore_background = new Texture2D(m_renderer);
    if (!m_highscore_background->LoadFromFile("Images/HighscoresBackground.png"))
    {
        std::cout << "Failed to load texture!" << std::endl;
    }

    // Open file
    std::ifstream inFile;

    inFile.open("Highscores.txt");

    if (!inFile.good()) // If file opens
    {
        std::cerr << "Can't open highscores" << std::endl;
        return false;
    }

    int currentNumber;
    int count = 0;
    while (!inFile.eof())
    {
        inFile >> currentNumber; // Load each number

        // Create a new text object from top to bottom for each number
        m_scores_text[count] = new Text(m_renderer, Vector2D(SCREEN_WIDTH / 2 - 64, 148 + (28 * count)), (std::to_string(count+1) + ": " + std::to_string(currentNumber)).c_str(), "Pixel.ttf", 25, { 255, 255, 255 });
        count++;
    }
    inFile.close();

    return true;
}
