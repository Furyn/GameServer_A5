#pragma once
#include "Player.hpp"
#include "constants.hpp"
#include "math/vector2.hpp"

class Ball
{
public:
	Ball();
	virtual ~Ball();

	Vector2 position;
	Vector2 direction;
	bool hasHitPlayer = false;
	float timeSinceHasHitPlayer = 0.f;

	void UpdatePhysics(Player& player1, Player& player2, float elapsedTime);

private:
	void HitBoundaries();
	void HitPlayer(Player& player);
};