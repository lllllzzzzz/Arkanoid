#ifndef BRICK_GRID_HPP
#define BRICK_GRID_HPP

#include <vector>
#include "brick.hpp"

class BrickGrid
{
public:
    BrickGrid(/*const sf::Vector2f position, const sf::Vector2f size*/);
    ~BrickGrid();

    void Init(GameEngine *game);

    void Update();

    void GenerateGrid(const int level);
    void Clear() noexcept { m_bricks.clear(); }
    void ResetGrid();
    int GetNumberOfBricks() const noexcept { return m_bricks.size(); }
    bool IsEmpty() const noexcept { return m_bricks.empty(); }

    std::vector<Brick>& GetBricks() { return m_bricks; }

    void MoveDown(/*const int numCells*/);

private:
    GameEngine *m_engine;

    std::vector<Brick> m_bricks;

    sf::Vector2f m_position;
    sf::Vector2f m_size;
    sf::Vector2f m_numBricks;

    int m_currentLevel;

    static const int NUMBER_BLOCKS_TO_MOVE_DOWN;
    static const sf::Vector2f BRICK_SIZE;

    template <typename T>
    struct brickData {
        T x;
        T y;
        sf::Color colour;
    };

    std::vector<brickData<double>> currentLevel;

    std::vector<brickData<double>> level1 = {
        {1, 0, sf::Color::Green},
        {2, 0, sf::Color::Green},
        {3, 0, sf::Color::Green},
        {4, 0, sf::Color::Green},
        {5, 0, sf::Color::Green},
        {6, 0, sf::Color::Green},
        {7, 0, sf::Color::Green},
        {8, 0, sf::Color::Green},
        {9, 0, sf::Color::Green},

        {1, 1, sf::Color::Green},
        {2, 1, sf::Color::Green},
        {3, 1, sf::Color::Green},
        {4, 1, sf::Color::Green},
        {5, 1, sf::Color::Green},
        {6, 1, sf::Color::Green},
        {7, 1, sf::Color::Green},
        {8, 1, sf::Color::Green},
        {9, 1, sf::Color::Green},

        {1, 2, sf::Color::Blue},
        {2, 2, sf::Color::Blue},
        {3, 2, sf::Color::Blue},
        {4, 2, sf::Color::Blue},
        {5, 2, sf::Color::Blue},
        {6, 2, sf::Color::Blue},
        {7, 2, sf::Color::Blue},
        {8, 2, sf::Color::Blue},
        {9, 2, sf::Color::Blue},

        {1, 3, sf::Color::Blue},
        {2, 3, sf::Color::Blue},
        {3, 3, sf::Color::Blue},
        {4, 3, sf::Color::Blue},
        {5, 3, sf::Color::Blue},
        {6, 3, sf::Color::Blue},
        {7, 3, sf::Color::Blue},
        {8, 3, sf::Color::Blue},
        {9, 3, sf::Color::Blue},

        {1, 4, sf::Color::Red},
        {2, 4, sf::Color::Red},
        {3, 4, sf::Color::Red},
        {4, 4, sf::Color::Red},
        {5, 4, sf::Color::Red},
        {6, 4, sf::Color::Red},
        {7, 4, sf::Color::Red},
        {8, 4, sf::Color::Red},
        {9, 4, sf::Color::Red},

        {1, 5, sf::Color::Red},
        {2, 5, sf::Color::Red},
        {3, 5, sf::Color::Red},
        {4, 5, sf::Color::Red},
        {5, 5, sf::Color::Red},
        {6, 5, sf::Color::Red},
        {7, 5, sf::Color::Red},
        {8, 5, sf::Color::Red},
        {9, 5, sf::Color::Red},
    };

    std::vector<brickData<double>> level2 = {
        {0, 0, sf::Color::Blue},
        {1, 0, sf::Color::Blue},
        {2, 0, sf::Color::Blue},
        {3, 0, sf::Color::Blue},
        {4, 0, sf::Color::Blue},
        {5, 0, sf::Color::Blue},
        {6, 0, sf::Color::Blue},
        {7, 0, sf::Color::Blue},
        {8, 0, sf::Color::Blue},
        {9, 0, sf::Color::Blue},
        {10, 0, sf::Color::Blue},

        {1, 1, sf::Color::Magenta},
        {2, 1, sf::Color::Magenta},
        {3, 1, sf::Color::Magenta},
        {4, 1, sf::Color::Magenta},
        {5, 1, sf::Color::Magenta},
        {6, 1, sf::Color::Magenta},
        {7, 1, sf::Color::Magenta},
        {8, 1, sf::Color::Magenta},
        {9, 1, sf::Color::Magenta},

        {2, 2, sf::Color::Blue},
        {3, 2, sf::Color::Blue},
        {4, 2, sf::Color::Blue},
        {5, 2, sf::Color::Blue},
        {6, 2, sf::Color::Blue},
        {7, 2, sf::Color::Blue},
        {8, 2, sf::Color::Blue},

        {3, 3, sf::Color::Cyan},
        {4, 3, sf::Color::Cyan},
        {5, 3, sf::Color::Cyan},
        {6, 3, sf::Color::Cyan},
        {7, 3, sf::Color::Cyan},

        {4, 4, sf::Color::White},
        {5, 4, sf::Color::White},
        {6, 4, sf::Color::White},

        {5, 5, sf::Color::Red},
    };

    std::vector<brickData<double>> level3 = {
        {1, 0, sf::Color::Green},
        {2, 0, sf::Color::Green},
        {3, 0, sf::Color::Green},
        {4, 0, sf::Color::Green},
        {5, 0, sf::Color::Green},
        {6, 0, sf::Color::Green},
        {7, 0, sf::Color::Green},
        {8, 0, sf::Color::Green},
        {9, 0, sf::Color::Green},

        {1, 1, sf::Color::Green},
        {9, 1, sf::Color::Green},

        {1, 2, sf::Color::Blue},
        {3, 2, sf::Color::Blue},
        {4, 2, sf::Color::Blue},
        {5, 2, sf::Color::Blue},
        {6, 2, sf::Color::Blue},
        {7, 2, sf::Color::Blue},
        {9, 2, sf::Color::Blue},

        {1, 3, sf::Color::Blue},
        {3, 3, sf::Color::Blue},
        {4, 3, sf::Color::Blue},
        {5, 3, sf::Color::Blue},
        {6, 3, sf::Color::Blue},
        {7, 3, sf::Color::Blue},
        {9, 3, sf::Color::Blue},

        {1, 4, sf::Color::Red},
        {9, 4, sf::Color::Red},

        {1, 5, sf::Color::Red},
        {9, 5, sf::Color::Red},

        {1, 5, sf::Color::Green},
        {9, 5, sf::Color::Green},

        {1, 5, sf::Color::Cyan},
        {2, 5, sf::Color::Cyan},
        {3, 5, sf::Color::Cyan},
        {4, 5, sf::Color::Cyan},
        {5, 5, sf::Color::Cyan},
        {6, 5, sf::Color::Cyan},
        {7, 5, sf::Color::Cyan},
        {8, 5, sf::Color::Cyan},
        {9, 5, sf::Color::Cyan},
    };

    std::vector<brickData<double>> level4 = {
        {0, 0, sf::Color::Red},
        {1, 0, sf::Color::Red},
        {3, 0, sf::Color::Red},
        {4, 0, sf::Color::Red},
        {6, 0, sf::Color::Red},
        {7, 0, sf::Color::Red},
        {9, 0, sf::Color::Red},
        {10, 0, sf::Color::Red},

        {5, 2, sf::Color::Red},

        {4, 3, sf::Color::White},
        {5, 3, sf::Color::White},
        {6, 3, sf::Color::White},

        {3, 4, sf::Color::Cyan},
        {4, 4, sf::Color::Cyan},
        {5, 4, sf::Color::Cyan},
        {6, 4, sf::Color::Cyan},
        {7, 4, sf::Color::Cyan},

        {2, 5, sf::Color::Blue},
        {3, 5, sf::Color::Blue},
        {4, 5, sf::Color::Blue},
        {5, 5, sf::Color::Blue},
        {6, 5, sf::Color::Blue},
        {7, 5, sf::Color::Blue},
        {8, 5, sf::Color::Blue},

        {1, 6, sf::Color::Magenta},
        {2, 6, sf::Color::Magenta},
        {3, 6, sf::Color::Magenta},
        {4, 6, sf::Color::Magenta},
        {5, 6, sf::Color::Magenta},
        {6, 6, sf::Color::Magenta},
        {7, 6, sf::Color::Magenta},
        {8, 6, sf::Color::Magenta},
        {9, 6, sf::Color::Magenta},

        {0, 7, sf::Color::Blue},
        {1, 7, sf::Color::Blue},
        {2, 7, sf::Color::Blue},
        {3, 7, sf::Color::Blue},
        {4, 7, sf::Color::Blue},
        {5, 7, sf::Color::Blue},
        {6, 7, sf::Color::Blue},
        {7, 7, sf::Color::Blue},
        {8, 7, sf::Color::Blue},
        {9, 7, sf::Color::Blue},
        {10, 7, sf::Color::Blue},
    };

    std::vector<brickData<double>> level5 = {
        {1, 0, sf::Color::Blue},
        {2, 0, sf::Color::Blue},
        {3, 0, sf::Color::Blue},
        {4, 0, sf::Color::Blue},
        {5, 0, sf::Color::Blue},
        {6, 0, sf::Color::Blue},
        {7, 0, sf::Color::Blue},
        {8, 0, sf::Color::Blue},
        {9, 0, sf::Color::Blue},

        {2, 1, sf::Color::Green},
        {3, 1, sf::Color::Green},
        {4, 1, sf::Color::Green},
        {5, 1, sf::Color::Green},
        {6, 1, sf::Color::Green},
        {7, 1, sf::Color::Green},
        {8, 1, sf::Color::Green},

        {3, 2, sf::Color::Cyan},
        {4, 2, sf::Color::Cyan},
        {5, 2, sf::Color::Cyan},
        {6, 2, sf::Color::Cyan},
        {7, 2, sf::Color::Cyan},

        {4, 3, sf::Color::Red},
        {5, 3, sf::Color::Red},
        {6, 3, sf::Color::Red},

        {3, 4, sf::Color::Cyan},
        {4, 4, sf::Color::Cyan},
        {5, 4, sf::Color::Cyan},
        {6, 4, sf::Color::Cyan},
        {7, 4, sf::Color::Cyan},

        {2, 5, sf::Color::Green},
        {3, 5, sf::Color::Green},
        {4, 5, sf::Color::Green},
        {5, 5, sf::Color::Green},
        {6, 5, sf::Color::Green},
        {7, 5, sf::Color::Green},
        {8, 5, sf::Color::Green},

        {1, 6, sf::Color::Blue},
        {2, 6, sf::Color::Blue},
        {3, 6, sf::Color::Blue},
        {4, 6, sf::Color::Blue},
        {5, 6, sf::Color::Blue},
        {6, 6, sf::Color::Blue},
        {7, 6, sf::Color::Blue},
        {8, 6, sf::Color::Blue},
        {9, 6, sf::Color::Blue},
    };

    std::vector<std::vector<brickData<double>>> levels = {
        level1, level2, level3, level4, level5
    };
};

#endif // BRICK_GRID_HPP
