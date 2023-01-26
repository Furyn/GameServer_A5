#include "Player.hpp"

Player::Player(){}

Player::~Player(){}

void Player::UpdatePhysics(float elapsedTime)
{
    if (up)
    {
        if (position.y < playerBoundaryValue)
        {
            position.y += playerSpeed * elapsedTime;
        }
    }
    else if (down)
    {
        if (position.y > -playerBoundaryValue)
        {
            position.y -= playerSpeed * elapsedTime;
        }
    }
}