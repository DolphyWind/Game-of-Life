#include "GameOfLife.hpp"

/* Initializes game */
GameOfLife::GameOfLife()
{
    m_camera.setCenter(0, 0);
    
    m_window.create(m_defaultWindowSize, m_windowTitle);
    m_window.setFramerateLimit(m_FPS);
    updateCamera(m_defaultWindowSize.width, m_defaultWindowSize.height);

    m_verticalPoints[0].color = sf::Color::Black;
    m_verticalPoints[1].color = sf::Color::Black;
    m_horizontalPoints[0].color = sf::Color::Black;
    m_horizontalPoints[1].color = sf::Color::Black;
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

/* Handles sfml events of window */
void GameOfLife::handleEvents()
{
    sf::Event e;
    while(m_window.pollEvent(e))
    {
        if(e.type == sf::Event::Closed)
            m_window.close();
        else if(e.type == sf::Event::Resized)
            updateCamera(e.size.width, e.size.height);
    }
}

/* Updates in-game elements */
void GameOfLife::update(sf::Time deltaTime)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
    
    if(m_window.hasFocus())
    {
        handleMouseInputs();
    }
}

/* Renders in-game elements */
void GameOfLife::render()
{
    m_window.clear(m_clearColor);

    /* Drawing the lines */
    float startX = int(m_horizontalPoints[0].position.x / m_gridSize.x) * m_gridSize.x;
    float startY = int(m_verticalPoints[0].position.y / m_gridSize.y) * m_gridSize.y;

    for(int x = startX; x < m_horizontalPoints[1].position.x; x += m_gridSize.x)
    {
        m_verticalPoints[0].position.x = x;
        m_verticalPoints[1].position.x = x;
        m_window.draw(m_verticalPoints, 2, sf::Lines);
    }

    for(int y = startY; y < m_verticalPoints[1].position.y; y += m_gridSize.y)
    {
        m_horizontalPoints[0].position.y = y;
        m_horizontalPoints[1].position.y = y;
        m_window.draw(m_horizontalPoints, 2, sf::Lines);
    }

    m_window.display();
}

/* Updates camera when camera is being moved or window is being resized */
void GameOfLife::updateCamera(std::uint32_t width, std::uint32_t height)
{
    if(width && height)
    {
        m_windowSize = {width, height};
        m_camera.setSize(width, height);
    }

    m_window.setView(m_camera);
    m_verticalPoints[0].position.y = m_camera.getCenter().y - m_camera.getSize().y / 2;
    m_verticalPoints[1].position.y = m_camera.getCenter().y + m_camera.getSize().y / 2;
    m_horizontalPoints[0].position.x = m_camera.getCenter().x - m_camera.getSize().x / 2;
    m_horizontalPoints[1].position.x = m_camera.getCenter().x + m_camera.getSize().x / 2;
}

/* Handles mouse inputs */
void GameOfLife::handleMouseInputs()
{
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        if(!m_isLeftDown)
        {
            m_isLeftDown = true;
            m_firstClickPosition = sf::Mouse::getPosition(m_window);
        }
        else if(mouseBoundaryCheck(m_firstClickPosition))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
            if(mousePos != m_firstClickPosition && !m_hasMovedMouse)
            {
                m_hasMovedMouse = true;
                m_firstCamCenter = m_camera.getCenter();
            }
            
            if(m_hasMovedMouse)
            {
                sf::Vector2i delta = {m_firstClickPosition.x - mousePos.x, m_firstClickPosition.y - mousePos.y};
                m_camera.setCenter(m_firstCamCenter.x + delta.x, m_firstCamCenter.y + delta.y);
                updateCamera();
            }
   
        }
    }
    else if (m_isLeftDown)
    {
        if(!m_hasMovedMouse && mouseBoundaryCheck(m_firstClickPosition))
        {
            // left click
            std::cout << "Left Click" << std::endl;
        }
        m_isLeftDown = false;
        m_hasMovedMouse = false;
        m_firstClickPosition = sf::Vector2i();
    }
}

/* Checks if given position is inside of the boundary of our window */
bool GameOfLife::mouseBoundaryCheck(sf::Vector2i ms)
{
    return (ms.x >= 0 && ms.x <= m_window.getSize().x && ms.y >= 0 && ms.y <= m_window.getSize().y);
}
