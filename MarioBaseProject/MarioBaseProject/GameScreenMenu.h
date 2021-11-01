#pragma once
#ifndef _GAMESCREENMENU_H
#define _GAMESCREENMENU_H
#include "GameScreen.h"
#include "Commons.h"
#include <vector>

class Texture2D;
class Text;

class GameScreenMenu : GameScreen
{
public:
	GameScreenMenu(SDL_Renderer* renderer);
	~GameScreenMenu() override;

	void Render() override;
	void Update(float deltaTime, SDL_Event e) override;
private:
	bool SetUpMenu();
	void ProceedScreen();

	Texture2D* m_menu_title;
	Text* m_cursor;

	int m_current_choice;
	int m_menu_count;
	float m_cursor_start;
};

#endif