#include "Timer.h"

Timer::Timer(float time)
{
	_time = time;
	_startTimer = false;
}

Timer::~Timer()
{

}

void Timer::Update(float deltaTime)
{
	if (_startTimer)
	{
		_time -= deltaTime;
	}
}
