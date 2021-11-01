#include "GameScreenLevel2.h"
#include <iostream>
#include <time.h>
#include <string>
#include <fstream>
#include "GameScreenManager.h"
#include "Texture2D.h"
#include "Character.h"
#include "CharacterMario.h"
#include "CharacterLuigi.h"
#include "PowBlock.h"
#include "Collisions.h"
#include "LevelMap.h"
#include "CharacterKoopa.h"
#include "CharacterCrab.h"
#include "Score.h"
#include "Text.h"
#include "Coin.h"
#include "Timer.h"
#include "SoundManager.h"
#include "AnimatedSprite.h"

GameScreenLevel2::GameScreenLevel2(SDL_Renderer* renderer) : GameScreenLevel1(renderer)
{
	m_renderer = renderer;
}

GameScreenLevel2::~GameScreenLevel2()
{
	delete m_background_texture;
	m_background_texture = nullptr;

	for (int i = 0; i < m_num_players; i++)
	{
		delete m_players[i];
		m_players[i] = nullptr;
	}

	delete m_level_map;
	m_level_map = nullptr;

	m_pow_blocks.clear();

	m_enemies.clear();

	delete m_score_display;
	m_score_display = nullptr;

	delete m_top_score;
	m_top_score = nullptr;

	delete m_timer;
	m_timer = nullptr;

	delete m_timer_display;
	m_timer_display = nullptr;

	if (m_splash != nullptr)
	{
		delete m_splash;
		m_splash = nullptr;
	}
}

void GameScreenLevel2::Render()
{
	// draw the background
	m_background_texture->Render(Vector2D(0, m_background_yPos), SDL_FLIP_NONE);

	for (int i = 0; i < m_pow_blocks.size(); i++)
	{
		m_pow_blocks[i]->Render();
	}

	//draw the enemies
	for (int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->Render();
	}

	for (int i = 0; i < m_coins.size(); i++)
	{
		m_coins[i]->Render();
	}

	m_pipes->Render(Vector2D(0, 0), SDL_FLIP_NONE);
	for (int i = 0; i < m_num_players; i++)
		m_players[i]->Render();

	m_score_display->Render();
	m_top_score->Render();
	m_timer_display->Render();

	RenderSplash();
}

void GameScreenLevel2::Update(float deltaTime, SDL_Event e)
{
	/*
 * do the screen shake if required
 */
	CheckScreenshake(deltaTime);

	SpawnEnemies(deltaTime);

	// Update Players
	for (int i = 0; i < m_num_players; i++)
		m_players[i]->Update(deltaTime, e);

	UpdateSplash(deltaTime, e);
	UpdatePOWBlock();
	UpdateEnemies(deltaTime, e);
	BumpBlock();
	UpdateCoins(deltaTime, e);
	m_player1_score->Update(deltaTime);

	LivesCheck();

	m_timer->Update(deltaTime);
	int time = m_timer->GetTime();
	m_timer_display->SetText(std::to_string(time));
	if (time <= 0)
	{
		GameScreenManager::getInstance()->current_score = m_player1_score->GetScore();
		GameScreenManager::getInstance()->SetSceneChange(SCREEN_LEVEL2); // Test to see if the scene changer works
	}
}

void GameScreenLevel2::SpawnEnemies(float deltaTime)
{
	// HANDLE ENEMY SPAWNING BASIC
	m_enemy_respawn_timer -= deltaTime;
	if (m_enemy_respawn_timer <= 0)
	{
		int num = rand() % 2;
		if (num)
			CreateKoopa(Vector2D(32, 32), FACING_RIGHT, KOOPA_SPEED);
		else
			CreateCrab(Vector2D(32, 34), FACING_RIGHT, KOOPA_SPEED);

		srand(time(NULL));

		num = rand() % 2;
		if (num)
			CreateKoopa(Vector2D(SCREEN_WIDTH - 64, 32), FACING_LEFT, KOOPA_SPEED);
		else
			CreateCrab(Vector2D(SCREEN_WIDTH - 64, 34), FACING_LEFT, KOOPA_SPEED);
		m_enemy_respawn_timer = rand() % 12 + 3;
	}

	// HANDLE COIN SPAWNING BASIC
	m_coin_respawn_timer -= deltaTime;
	if (m_coin_respawn_timer <= 0)
	{
		Coin* coin = new Coin(m_renderer, "Images/Coin.png", 4, 1, 2, m_level_map, Vector2D(128, 30), FACING_RIGHT, 30);
		m_coins.push_back(coin);
		m_coin_respawn_timer = rand() % 15 + 3;
	}
}

void GameScreenLevel2::SetLevelMap(std::string path)
{
	//clear any old maps
	if (m_level_map != nullptr)
	{
		delete m_level_map;
	}
	m_level_map = new LevelMap(path);
}

bool GameScreenLevel2::SetUpLevel()
{
	SoundManager::getInstance()->PlayMusic(SoundManager::getInstance()->m_background_music_underworld, -1);

	m_num_players = GetPlayerCount();

	srand(time(NULL));
	m_enemy_respawn_timer = 0;
	m_coin_respawn_timer = 0;
	// Load Texture
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/BackgroundMB2.png"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}

	m_pipes = new Texture2D(m_renderer);
	if (!m_pipes->LoadFromFile("Images/Pipes.png"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}

	// Set up player character
	CharacterMario* my_character = new CharacterMario(m_renderer, "Images/MarioSpriteSheet.png", 4, 4, 10, Vector2D(64, 336), m_level_map);

	// Set up player 2
	CharacterMario* my_player2 = (CharacterMario*)(new CharacterLuigi(m_renderer, "Images/LuigiSpriteSheet.png", 4, 4, 10, Vector2D(128, 336), m_level_map));

	m_players[0] = my_character;
	m_players[1] = my_player2;

	my_character = nullptr;
	my_player2 = nullptr;


	m_player1_score = new Score();
	m_player1_score->SetScore(GameScreenManager::getInstance()->current_score);
	m_score_display = new Text(m_renderer, Vector2D(72, 0), "Score:" + std::to_string(m_player1_score->GetScore()), "Pixel.ttf", 18, { 255, 255, 255 });
	PowBlock* pow_block = new PowBlock(m_renderer, m_level_map, Vector2D(64, 260));
	PowBlock* pow_block2 = new PowBlock(m_renderer, m_level_map, Vector2D(416, 260));
	m_pow_blocks.push_back(pow_block);
	m_pow_blocks.push_back(pow_block2);
	m_timer = new Timer(91);
	m_timer->StartTime();
	m_timer_display = new Text(m_renderer, Vector2D(SCREEN_WIDTH / 2 - 32, 0), std::to_string(m_timer->GetTime()), "Pixel.ttf", 25, { 255, 255, 255 });

	m_screenshake = false;
	m_background_yPos = 0.0f;

	// Animated Sprite Extras
	m_splash = nullptr;

	std::ifstream inFile;
	inFile.open("Highscores.txt");

	if (!inFile.good())
	{
		std::cerr << "Can't open highscores" << std::endl;
		return false;
	}

	int currentNumber;
	inFile >> currentNumber; // Load top score
	inFile.close();

	m_top_score = new Text(m_renderer, Vector2D(SCREEN_WIDTH / 2 + 32, 0), "Top:" + std::to_string(currentNumber), "Pixel.ttf", 18, { 255, 255, 255 });
}
