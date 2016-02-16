#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "..\stateman\gameEngine.hpp"
#include "..\objects\paddle.hpp"

class Player
{
public:
    Player();
    ~Player();

    void Init(GameEngine *game);

    void Reset();

    Paddle& GetPaddle() { return m_paddle; }

    void SetPoints(const int points) noexcept { m_points = points; }
    int GetPoints() const noexcept { return m_points; }
    void GainPoints(const int points) noexcept { m_points += points; }

    void SetLives(const int lives) noexcept { m_lives = lives; }
    int GetLives() const noexcept { return m_lives; }
    int GainLives(const int lives) noexcept { return m_lives += lives; }

private:
    GameEngine *m_engine;

    Paddle m_paddle;

    int m_points;
    int m_lives;

    static const int DEFAULT_LIVES;
};

#endif // PLAYER_HPP
