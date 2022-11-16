#include "GameOfLife.hpp"

GameOfLife::GameOfLife()
{
    m_window.create(m_defaultWindowSize, m_windowTitle);
}

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

void GameOfLife::render()
{
    m_window.clear();

    m_window.display();
}

void GameOfLife::handleEvents()
{
    sf::Event e;
    while(m_window.pollEvent(e))
    {
        if(e.type == sf::Event::Closed)
            m_window.close();
    }
}

void GameOfLife::update(sf::Time deltaTime)
{
    
}
