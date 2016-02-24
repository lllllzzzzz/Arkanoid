#ifndef LASER_HPP
#define LASER_HPP

#include "../stateman/GameEngine.hpp"

class Laser
{
public:
    Laser();
    ~Laser();

    void Init();

    void Fire();

private:
    GameEngine *m_engine;
};

#endif // LASER_HPP
