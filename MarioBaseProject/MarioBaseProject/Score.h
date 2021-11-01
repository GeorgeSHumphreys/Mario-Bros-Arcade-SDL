#pragma once
#ifndef _SCORE_H
#define _SCORE_H
#include <SDL.h>
#include <iostream>
#include "Commons.h"

class Score
{
public:
	Score();
	~Score();

	void Update(float deltaTime);

	int GetScore() { return m_score; };
	void AddScore(int score) { m_score += score; };
	void SetScore(int score) { m_score = score; };


private:
	int m_score;
};

#endif