#pragma once
class Timer
{
public:
	Timer(float time);
	~Timer();
	void Update(float deltaTime);
	void StartTime() { _startTimer = true; };
	void StopTime() { _startTimer = false; };
	void SetTime(float time) { _time = time; };
	float GetTime() { return _time; };
private:
	float _time;
	bool _startTimer;
};

