#include "Cell.hpp"

Cell::Cell(std::uint32_t cellSize, sf::Color cellColor)
{
    m_cellSize = cellSize;
    m_shape.setSize({cellSize, cellSize});
    m_shape.setFillColor(cellColor);
}

void Cell::setGridPosition(sf::Vector2i gridPos)
{
    m_gridPosition = gridPos;
    m_shape.setPosition((float)gridPos.x * m_cellSize, (float)gridPos.y * m_cellSize);
}

sf::Vector2i Cell::getGridPosition()
{
    return m_gridPosition;
}

void Cell::draw(sf::RenderWindow* window)
{
    window->draw(m_shape);
}

std::vector<sf::Vector2i> Cell::getNeighborCoordinates()
{
    std::vector<sf::Vector2i> output;
    for(int y = -1; y <= 1; y++)
    {
        for(int x = -1; x <= 1; x++)
        {
            if(x || y) // Skip x=0 and y=0
                output.push_back({m_gridPosition.x + x, m_gridPosition.y + y});
        }
    }
    return std::move(output);
}
