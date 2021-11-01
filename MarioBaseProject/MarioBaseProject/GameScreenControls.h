#pragma once
#ifndef _GAMESCREENCONTROLS_H
#define _GAMESCREENCONTROLS_H
#include "GameScreen.h"
#include "Commons.h"
#include "constants.h"
#include <vector>

class AnimatedSprite;
class Texture2D;
class Text;

class GameScreenControls : public GameScreen
{
public:
	GameScreenControls(SDL_Renderer* renderer);
	~GameScreenControls() override;

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
private:
	bool SetUpMenu();
	void ProceedScreen();

	Texture2D* m_highscore_background;
	AnimatedSprite* m_coin;
	AnimatedSprite* m_koopa;
};

#endif