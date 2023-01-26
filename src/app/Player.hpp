#pragma once
#include "constants.hpp"
#include "math/vector2.hpp"

class Player
{
public :
	Player();
	virtual ~Player();
	bool up = false;
	bool down = false;
	Vector2 position;
	int playerNumber = 0;

	void UpdatePhysics(float elapsedTime);
};