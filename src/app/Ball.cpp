#include "Ball.hpp"
#include <iostream>

Ball::Ball() {
    RandDirection();
}

Ball::~Ball() {}

void Ball::UpdatePhysics(Player& player1, Player& player2, float elapsedTime)
{
    HitPlayer(player1);
    HitPlayer(player2);
    HitBoundaries();

    position += direction * ballSpeed * elapsedTime;

    if (position.x < -4 || position.x > 18) {
        
        position = { 7 , 0 };
        RandDirection();
    }

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

void Ball::RandDirection()
{
    srand(time(NULL));

    int random1 = rand() % 2;
    float r1 = 1.f;

    if (random1 == 0) {
        r1 = -1.f;
    }

    float random2 = ((float)rand()) / (float)RAND_MAX;
    float diff = 1 - (-1);
    float r2 = random2 * diff;

    direction = { r1 , r2 };
}
