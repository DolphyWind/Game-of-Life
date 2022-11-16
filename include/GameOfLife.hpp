#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

class GameOfLife
{
private:
    sf::RenderWindow m_window;

    /* Window defaults */
    const sf::VideoMode m_defaultWindowSize = {1280, 720};
    const std::string m_windowTitle = "Game of Life";
    const sf::Color m_clearColor = sf::Color::White;
    
    /* Game tick */
    const std::uint32_t m_FPS = 60;
    const sf::Time m_timePerFrame = sf::seconds( 1.f / float(m_FPS) );
    sf::Clock m_frameClock;

    void update(sf::Time deltaTime);
    void render();
    void handleEvents();

public:
    GameOfLife();
    ~GameOfLife(){};

    void mainLoop();
};