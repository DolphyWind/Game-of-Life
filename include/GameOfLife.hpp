#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <array>

class GameOfLife
{
private:
    sf::RenderWindow m_window;

    /* Window settings */
    const sf::VideoMode m_defaultWindowSize = {1280, 720};
    const std::string m_windowTitle = "Game of Life";
    const sf::Color m_clearColor = sf::Color::White;
    sf::Vector2u m_windowSize;
    sf::Vector2f m_gridSize = {100, 100};
    sf::View m_camera;
    
    /* Game tick */
    const std::uint32_t m_FPS = 60;
    const sf::Time m_timePerFrame = sf::seconds( 1.f / float(m_FPS) );
    sf::Clock m_frameClock;

    /* In game elements */
    sf::Vertex m_verticalPoints[2];
    sf::Vertex m_horizontalPoints[2];

    /* Movement & clicking with mouse */
    bool m_isLeftDown = false;
    sf::Vector2i m_firstClickPosition;
    bool m_hasMovedMouse = false;
    sf::Vector2f m_firstCamCenter;

    void update(sf::Time deltaTime);
    void render();
    void handleEvents();
    void updateCamera(std::uint32_t width = 0, std::uint32_t height = 0);
    void handleMouseInputs();
    bool mouseBoundaryCheck(sf::Vector2i ms);

public:
    GameOfLife();
    ~GameOfLife(){};

    void mainLoop();
};