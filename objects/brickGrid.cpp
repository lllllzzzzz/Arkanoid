#include "brickGrid.hpp"

#include <SFML\Graphics.hpp>

const int BrickGrid::NUMBER_BLOCKS_TO_MOVE_DOWN = 1;
const sf::Vector2f BrickGrid::BRICK_SIZE = {40, 20};

BrickGrid::BrickGrid(/*const sf::Vector2f position, const sf::Vector2f size*/)// :
    //m_position(position),
    //m_size(size)
{

}

BrickGrid::~BrickGrid()
{

}

void BrickGrid::Init(GameEngine *game)
{
    m_engine = game;

    m_position = {20, m_engine->getWindowSize().y + 125};
    m_size = {m_engine->getWindowSize().x - 40, m_engine->getWindowSize().y / 3};
}

void BrickGrid::Update()
{
    // Remove destroyed bricks from bricks vector
    m_bricks.erase(
        remove_if(begin(m_bricks), end(m_bricks),
            [](const Brick& mBrick) { return mBrick.isDestroyed(); }),
        end(m_bricks)
    );
}

void BrickGrid::GenerateGrid(const int level)
{
    if (level < 0 || level > levels.size()) {
        return;
    }

    m_bricks.clear();

    for (auto& gridCell : levels.at(level - /*1*/2)) {
        m_bricks.emplace_back(
            m_engine,
            sf::Vector2f{(gridCell.x + 1) * BRICK_SIZE.x, (gridCell.y * BRICK_SIZE.y) + 125},
            gridCell.colour,
            0);
    }
}

void BrickGrid::ResetGrid()
{
    m_bricks.clear();

    for (auto& gridCell : levels.at(m_currentLevel - 1)) {
        m_bricks.emplace_back(
            m_engine,
            sf::Vector2f{(gridCell.x + 1) * BRICK_SIZE.x, (gridCell.y * BRICK_SIZE.y) + 125},
            gridCell.colour,
            0);
    }
}

void BrickGrid::MoveDown(/*const int numCells*/)
{
    //if (!numCells) {
    //    return;
    //}

    for (auto& brick : m_bricks) {
        brick.setPos({brick.x(), brick.y() + (brick.GetSize().y * NUMBER_BLOCKS_TO_MOVE_DOWN)});
    }
}
