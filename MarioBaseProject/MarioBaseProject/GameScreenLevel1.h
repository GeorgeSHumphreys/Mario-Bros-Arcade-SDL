#pragma once
#ifndef _GAMESCREENLEVEL1_H
#define _GAMESCREENLEVEL1_H
#include "GameScreen.h"
#include "Commons.h"
#include <vector>
#include <iostream>


class Texture2D;
class Character;
class CharacterMario;
class CharacterLuigi;
class LevelMap;
class PowBlock;
class CharacterKoopa;
class CharacterCrab;
class Score;
class Text;
class Coin;
class Timer;
class AnimatedSprite;

class GameScreenLevel1 : GameScreen
{
public:
	GameScreenLevel1(SDL_Renderer* renderer);
	~GameScreenLevel1() override;

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	void UpdatePOWBlock();

	bool SetUpLevel();
	void SetLevelMap(std::string path);

protected:
	void DoScreenShake();
	void UpdateEnemies(float deltaTime, SDL_Event e);
	void UpdateCoins(float deltaTime, SDL_Event e);
	void BumpBlock();
	bool StoreHighscore(int score);
	void CreateKoopa(Vector2D position, FACING direction, float speed);
	void CreateCrab(Vector2D position, FACING direction, float speed);
	void UpdateSplash(float deltaTime, SDL_Event e);
	void RenderSplash();
	void CheckScreenshake(float deltaTime);
	void LivesCheck();
	int GetPlayerCount();
	virtual void SpawnEnemies(float deltaTime);

	int m_num_players;
	Texture2D* m_background_texture;
	CharacterMario* m_players[2];
	Score* m_player1_score;
	Text* m_score_display;
	Text* m_top_score;
	LevelMap* m_level_map;
	std::vector<PowBlock*> m_pow_blocks;
	Coin* m_coin;
	Timer* m_timer;
	Text* m_timer_display;
	std::vector<CharacterKoopa*> m_enemies;
	std::vector<Coin*> m_coins;
	Texture2D* m_pipes;
	AnimatedSprite* m_splash;

	bool m_screenshake;
	float m_shake_time;
	float m_wobble;
	float m_background_yPos;

	float m_enemy_respawn_timer;
	float m_coin_respawn_timer;
};

#endif