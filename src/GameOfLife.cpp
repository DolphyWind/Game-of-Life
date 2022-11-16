#include "GameOfLife.hpp"

/* Initializes game */
GameOfLife::GameOfLife()
{
    m_window.create(m_defaultWindowSize, m_windowTitle);
}

/* Main game logic */
void GameOfLife::mainLoop()
{
    m_frameClock.restart();
    sf::Time frameTime;
    sf::Time deltaTime;
    while(m_window.isOpen())
    {
        handleEvents();
        deltaTime = m_frameClock.restart();
        frameTime += deltaTime;
        while(frameTime >= m_timePerFrame)
        {
            frameTime -= m_timePerFrame;
            handleEvents();
            update(deltaTime);
        }
        
        render();
    }
}

/* Renders in-game elements */
void GameOfLife::render()
{
    m_window.clear(m_clearColor);

    m_window.display();
}

/* Handles sfml events of window */
void GameOfLife::handleEvents()
{
    sf::Event e;
    while(m_window.pollEvent(e))
    {
        if(e.type == sf::Event::Closed)
            m_window.close();
    }
}

/* Updates in-game elements */
void GameOfLife::update(sf::Time deltaTime)
{
    
}
