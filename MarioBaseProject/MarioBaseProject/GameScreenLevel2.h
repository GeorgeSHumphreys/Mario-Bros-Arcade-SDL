#pragma once
#ifndef _GAMESCREENLEVEL2_H
#define _GAMESCREENLEVEL2_H
#include "GameScreenLevel1.h"
class GameScreenLevel2 : GameScreenLevel1
{
public:
	GameScreenLevel2(SDL_Renderer* renderer);
	~GameScreenLevel2() override;
	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
	bool SetUpLevel();
	void SetLevelMap(std::string path);

private:
	void SpawnEnemies(float deltaTime) override;
	SDL_Renderer* m_renderer;
};

#endif