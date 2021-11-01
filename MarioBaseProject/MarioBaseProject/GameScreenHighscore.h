#pragma once
#ifndef _GAMESCREENHIGHSCORE_H
#define _GAMESCREENHIGHSCORE_H
#include "GameScreen.h"
#include "Commons.h"
#include "constants.h"
#include <vector>

class Texture2D;
class Text;

class GameScreenHighscore : GameScreen
{
public:
	GameScreenHighscore(SDL_Renderer* renderer);
	~GameScreenHighscore() override;

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
private:
	bool SetUpMenu();
	void ProceedScreen();

	Texture2D* m_highscore_background;
	Text* m_scores_text[8];
};

#endif