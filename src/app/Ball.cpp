#include "Ball.hpp"
#include <iostream>

Ball::Ball() {
    direction = { -1.f, 0.f };
}

Ball::~Ball() {}

void Ball::UpdatePhysics(Player& player1, Player& player2, float elapsedTime)
{
    HitPlayer(player1);
    HitPlayer(player2);
    HitBoundaries();

    position += direction * ballSpeed * elapsedTime;

    if (hasHitPlayer)
    {
        timeSinceHasHitPlayer += elapsedTime;
        if (timeSinceHasHitPlayer >= 0.1f)
        {
            hasHitPlayer = false;
            timeSinceHasHitPlayer = 0.f;
        }
    }
}

void Ball::HitBoundaries()
{
    if (position.y >= ballBoundaryValue || position.y <= -ballBoundaryValue)
    {
        direction.y = -direction.y;
    }
}

void Ball::HitPlayer(Player& player)
{
    if (hasHitPlayer)
        return;

    float distance = position.x - player.position.x;
    if (distance > 0.25f || distance < -0.25f)
        return;

    if (player.playerNumber == 1) {
        std::cout << "PING" << std::endl;
    }
    else {
        std::cout << "PONG" << std::endl;
    }

    // Partie centre Player 
    if (position.y > player.position.y - 0.5f && position.y < player.position.y + 0.5f)
    {
        direction.x = -direction.x;
    }
    // Partie basse Player 
    else if (position.y < player.position.y - 0.5f && position.y > player.position.y - 1.f)
    {
        direction.x = -direction.x;
    }
    // Partie haute Player 
    else if (position.y > player.position.y + 0.5f && position.y < player.position.y + 1.f)
    {
        direction.x = -direction.x;
    }

    hasHitPlayer = true;
}
