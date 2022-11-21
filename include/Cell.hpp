#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

class Cell
{
private:
    sf::Vector2i m_gridPosition;
    sf::RectangleShape m_shape;
    std::uint32_t m_cellSize;
public:
    Cell(std::uint32_t cellSize, sf::Color cellColor);
    void setGridPosition(sf::Vector2i gridPos);
    sf::Vector2i getGridPosition();
    void draw(sf::RenderWindow* window);
    std::vector<sf::Vector2i> getNeighborCoordinates();
    ~Cell() {};
};