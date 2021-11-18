#pragma once

struct CollisonData
{
	int x, y, w, h;
};

float Approach(float goal, float current, float deltaTime)
{
	float difference = goal - current;
	if(difference > deltaTime)
		return current + deltaTime;
	else if(difference < -deltaTime)
		return current - deltaTime;
	return goal;
}


