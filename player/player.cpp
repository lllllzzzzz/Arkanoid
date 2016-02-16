#include "player.hpp"

const int Player::DEFAULT_LIVES = 2;

Player::Player() :
    m_points(0),
    m_lives(0)
{

}

Player::~Player()
{

}

void Player::Init(GameEngine *game)
{
    m_engine = game;

    m_paddle.Init(m_engine);
}

void Player::Reset()
{
    m_paddle.Reset();
    SetPoints(0);
    SetLives(DEFAULT_LIVES);
}
