#include "GameScreenLevel1.h"
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
#include "AnimatedSprite.h"
#include "SoundManager.h"

GameScreenLevel1::GameScreenLevel1(SDL_Renderer* renderer) : GameScreen(renderer)
{
}

GameScreenLevel1::~GameScreenLevel1()
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

void GameScreenLevel1::Render()
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

void GameScreenLevel1::Update(float deltaTime, SDL_Event e)
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

	m_timer->Update(deltaTime); // Update Timer
	int time = m_timer->GetTime();
	m_timer_display->SetText(std::to_string(time)); // Update timer display

	if (time <= 0) // If time is over, go to next level
	{
		GameScreenManager::getInstance()->current_score = m_player1_score->GetScore();
		GameScreenManager::getInstance()->SetSceneChange(SCREEN_LEVEL2); // Test to see if the scene changer works
	}
}

void GameScreenLevel1::LivesCheck()
{
	// Lives check for both single and multiplayer
	int livesZero = 0;
	for (int i = 0; i < m_num_players; i++)
	{
		if (m_players[i]->GetLives() == 0)
			livesZero++;
	}

	if (livesZero == m_num_players) // If all players have lost all lives
	{
		StoreHighscore(m_player1_score->GetScore());
		GameScreenManager::getInstance()->current_score = m_player1_score->GetScore();
		GameScreenManager::getInstance()->SetSceneChange(SCREEN_GAMEOVER); // Test to see if the scene changer works
	}

}

void GameScreenLevel1::SpawnEnemies(float deltaTime)
{
	// HANDLE ENEMY SPAWNING BASIC
	m_enemy_respawn_timer -= deltaTime;
	if (m_enemy_respawn_timer <= 0)
	{
		CreateKoopa(Vector2D(32, 32), FACING_RIGHT, KOOPA_SPEED);

		CreateKoopa(Vector2D(SCREEN_WIDTH - 64, 32), FACING_LEFT, KOOPA_SPEED);

		m_enemy_respawn_timer = rand() % 12 + 3;
	}
}

void GameScreenLevel1::CheckScreenshake(float deltaTime)
{
	if (m_screenshake)
	{
		m_shake_time -= deltaTime;
		m_wobble++;
		m_background_yPos = sin(m_wobble);
		m_background_yPos *= 3.0f;

		//end shake after duration
		if (m_shake_time <= 0.0f)
		{
			m_shake_time = false;
			m_background_yPos = 0.0f;
		}
	}
}

void GameScreenLevel1::UpdateSplash(float deltaTime, SDL_Event e)
{
	if (m_splash != nullptr)
	{
		m_splash->Update(deltaTime, e);
		if (m_splash->GetDelete())
		{
			delete m_splash;
			m_splash = nullptr;
		}
	}
}
void GameScreenLevel1::RenderSplash()
{
	if (m_splash != nullptr)
		m_splash->Render();
}

bool GameScreenLevel1::StoreHighscore(int score)
{
	std::ifstream inFile;

	inFile.open("Highscores.txt"); // Open File

	if (!inFile.good())
	{
		std::cerr << "Can't open highscores" << std::endl;
		return false;
	}

	// Variable Setup
	std::vector<int> highscores;
	int currentNumber;
	int count = 0;
	bool found = false;


	while (!inFile.eof())
	{
		count++; // Increase Count
		inFile >> currentNumber; // Get next number

		if (score > currentNumber && !found) // If the score is bigger than the current, and position isn't found
		{
			highscores.push_back(score); // Push back the score  and increase the count
			count++;
			found = true; // Its now found
		}

		if(count <= 8)
			highscores.push_back(currentNumber); // Check the counter isn't over 8 and pushback the next number
												 // If "Found" the final number will always be ignored
	}
	inFile.close(); // Close File
	if (!found) return false; // If found is false, return false, if not continue

	std::ofstream outFile; // Set up output file
	outFile.open("Highscores.txt"); // Open
	if (!outFile)
	{
		std::cout << "File could not be opened!" << std::endl;
		return false;;
	}

	for (int i = 0; i < highscores.size(); i++) // Loop through highscores and add to the file
	{
		outFile << highscores[i];
		if (i != 7) outFile << "\n";
	}
	outFile.close();

	return true;
}

void GameScreenLevel1::BumpBlock()
{
	for (int i = 0; i < m_num_players; i++) // Loop for both players
	{
		TilePosition playerPos = m_players[i]->GetTilePos();
		if (!m_players[i]->GetDying()) // When character is dying, it shouldn't be interactable
		{
			if ((m_level_map->GetTileAt(playerPos.headPos, playerPos.lPos) == 1 || m_level_map->GetTileAt(playerPos.headPos, playerPos.rPos) == 1) && !m_players[i]->GetFalling()) // Check at the players head position to see if they hit a tile
			{
				m_players[i]->CancelJump(); // Cancel players jump
				SoundManager::getInstance()->PlaySound(SoundManager::getInstance()->m_bump);
				for (unsigned int i = 0; i < m_enemies.size(); i++) // Loop all of the enemies
				{
					TilePosition enemyPos = m_enemies[i]->GetTilePos(); // Get their tile positions
					if (enemyPos.footPos == playerPos.headPos) // If the player and enemy are on the Y tile level
					{
						if ((enemyPos.lPos == playerPos.lPos || enemyPos.lPos == playerPos.rPos) || (enemyPos.rPos == playerPos.lPos || enemyPos.rPos == playerPos.rPos)) // If enemy is above player
						{
							m_enemies[i]->TakeDamage(); // Deal damage to enemy
						}
					}
				}
			}
		}
	}
}

void GameScreenLevel1::UpdateCoins(float deltaTime, SDL_Event e)
{
	int coinIndexToDelete = -1;
	for (int i = 0; i < m_coins.size(); i++)
	{
		m_coins[i]->Update(deltaTime, e);
		// Player Checks
		for (int j = 0; j < m_num_players; j++)
		{
			if (Collisions::Instance()->Box(m_players[j]->GetCollisionBox(), m_coins[i]->GetCollisionBox()))
			{
				m_coins[i]->SetAlive(false); // Set coin to delete
				SoundManager::getInstance()->PlaySound(SoundManager::getInstance()->m_coin); // Play coin sound
				m_player1_score->AddScore(1000); // Add to score
				m_score_display->SetText("Score:" + std::to_string(m_player1_score->GetScore())); // Update score text
				coinIndexToDelete = i; // Set delete index
			}
		}
	}

	if (coinIndexToDelete != -1)
	{
		m_coins.erase(m_coins.begin() + coinIndexToDelete);
	}
}

void GameScreenLevel1::UpdateEnemies(float deltaTime, SDL_Event e)
{
	if (!m_enemies.empty())
	{
		int enemyIndexToDelete = -1;
		for (unsigned int i = 0; i < m_enemies.size(); i++)
		{
			m_enemies[i]->Update(deltaTime, e);
			if (!m_enemies[i]->GetDying()) // If the enemy is in death animation, they shouldn't interact normally
			{
				//check if the enemy is on the bottom row of tiles
				if (m_enemies[i]->GetPosition().y > 300.0f)
				{
					//is the enemy off screen to the left / right?
					if (m_enemies[i]->GetPosition().x < (float)(-m_enemies[i]->GetCollisionBox().width * 0.5f) || m_enemies[
						i]->GetPosition().x > SCREEN_WIDTH - (float)(m_enemies[i]->GetCollisionBox().width * 0.55f))
						m_enemies[i]->SetAlive(false);
				}

				//check to see if enemy collides with player
				if ((m_enemies[i]->GetPosition().y > 300.0f || m_enemies[i]->GetPosition().y <= 64.0f) && (m_enemies[i]->
					GetPosition().x < 64.0f || m_enemies[i]->GetPosition().x > SCREEN_WIDTH - 96.0f))
				{
					//ignore collisions if behind pipe
				}
				else
				{
					for (int j = 0; j < m_num_players; j++) // Player Collision Checks
					{
						if (Collisions::Instance()->Circle(m_enemies[i]->GetCollisionCircle(), m_players[j]->GetCollisionCircle()) && !m_players[j]->GetDying())
						{
							if (m_enemies[i]->GetInjured())
							{
								m_enemies[i]->SetDying(true); // Set dying animation
								SoundManager::getInstance()->PlaySound(SoundManager::getInstance()->m_kick);
								m_player1_score->AddScore(m_enemies[i]->GetScore()); // Add Enemy score amount to player score
								m_score_display->SetText("Score:" + std::to_string(m_player1_score->GetScore())); // Update score text
							}
							else
							{
								m_players[j]->SetDying(true); // Set dying animation
								SoundManager::getInstance()->PlaySound(SoundManager::getInstance()->m_damage);
							}
						}
					}
				}
			}
			//if the enemy is no longer alive then schedule it for deletion
			if (!m_enemies[i]->GetAlive())
			{
				if (m_splash == nullptr)
				{
					m_splash = new AnimatedSprite(m_renderer, "Images/Splash.png", 4, 1, 2, Vector2D(m_enemies[i]->GetPosition().x, SCREEN_HEIGHT - 32), 0.5);
				}
				enemyIndexToDelete = i;
			}
		}

		//remove dead enemies -1 each update

		if (enemyIndexToDelete != -1)
		{
			m_enemies.erase(m_enemies.begin() + enemyIndexToDelete);
		}
	}

}

void GameScreenLevel1::UpdatePOWBlock()
{
	for (int i = 0; i < m_pow_blocks.size(); i++)
	{

		for (int j = 0; j < m_num_players; j++) // Player Collision Checks
		{
			if (Collisions::Instance()->Box(m_players[j]->GetCollisionBox(), m_pow_blocks[i]->GetCollisionBox()) && !m_players[j]->GetDying()) // Only passes if player isn't dying
			{
				if (m_pow_blocks[i]->IsAvailable())
				{
					//collided while jumping
					if (m_players[j]->IsJumping())
					{
						DoScreenShake();
						SoundManager::getInstance()->PlaySound(SoundManager::getInstance()->m_bump); // Play Bump Sound
						m_pow_blocks[i]->TakeHit();
						m_players[j]->CancelJump();
					}
				}
			}
		}
	}
}

void GameScreenLevel1::DoScreenShake()
{
	m_screenshake = true;
	m_shake_time = SHAKE_DURATION;
	m_wobble = 0.0f;

	for (unsigned int i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->TakeDamage();
	}
}

void GameScreenLevel1::CreateKoopa(Vector2D position, FACING direction, float speed)
{
	// Setup and spawn Koopa
	CharacterKoopa* koopa = new CharacterKoopa(m_renderer, "Images/Koopa.png", 3, 2, 5, m_level_map, position, direction, speed);
	m_enemies.push_back(koopa);
}

void GameScreenLevel1::CreateCrab(Vector2D position, FACING direction, float speed)
{
	// Setup and spawn crab
	CharacterCrab* crab = new CharacterCrab(m_renderer, "Images/Crab.png", 2, 3, 5, m_level_map, position, direction, speed);
	m_enemies.push_back((CharacterKoopa*)crab);
}

void GameScreenLevel1::SetLevelMap(std::string path)
{
	//clear any old maps
	if (m_level_map != nullptr)
	{
		delete m_level_map;
	}
	m_level_map = new LevelMap(path);
}

int GameScreenLevel1::GetPlayerCount()
{
	return GameScreenManager::getInstance()->num_players;
}

bool GameScreenLevel1::SetUpLevel()
{
	SoundManager::getInstance()->PlayMusic(SoundManager::getInstance()->m_background_music, -1); // Start Level Music

	m_num_players = GetPlayerCount();// Find how many players

	srand(time(NULL));
	m_enemy_respawn_timer = 0;
	m_coin_respawn_timer = 0;
	// Load Background
	m_background_texture = new Texture2D(m_renderer);
	if (!m_background_texture->LoadFromFile("Images/BackgroundMB.png"))
	{
		std::cout << "Failed to load background texture!" << std::endl;
		return false;
	}

	m_pipes = new Texture2D(m_renderer); // Load Pipes
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

	m_player1_score = new Score(); // Setup Combined Score
	m_score_display = new Text(m_renderer, Vector2D(72, 0), "Score:" + std::to_string(m_player1_score->GetScore()), "Pixel.ttf", 18, {255, 255, 255}); // Score Display text

	// Starter Pow Blocks
	PowBlock* pow_block = new PowBlock(m_renderer, m_level_map, Vector2D(224, 260));
	PowBlock* pow_block2 = new PowBlock(m_renderer, m_level_map, Vector2D(256, 260));
	m_pow_blocks.push_back(pow_block);
	m_pow_blocks.push_back(pow_block2);

	// Set up timer
	m_timer = new Timer(31);
	m_timer->StartTime();
	m_timer_display = new Text(m_renderer, Vector2D(SCREEN_WIDTH / 2 - 32, 0), std::to_string(m_timer->GetTime()), "Pixel.ttf", 25, { 255, 255, 255 });

	// Animated Sprite Extras
	m_splash = nullptr;

	m_screenshake = false;
	m_background_yPos = 0.0f;

	// Load highscore
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