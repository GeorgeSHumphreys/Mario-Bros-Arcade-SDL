#pragma once
#ifndef _GAMESCREENGAMEOVER_H
#define _GAMESCREENGAMEOVER_H
#include "GameScreen.h"
#include "Commons.h"
#include "constants.h"
#include <vector>

class AnimatedSprite;
class Texture2D;
class Text;


class GameScreenGameOver : public GameScreen
{
public:
	GameScreenGameOver(SDL_Renderer* renderer);
	~GameScreenGameOver() override;

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
private:
	bool SetUpMenu();
	void ProceedScreen();

	Texture2D* m_highscore_background;
	Text* m_score;
};

#endif