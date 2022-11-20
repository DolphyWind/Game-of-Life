#include "GameOfLife.hpp"

/* Initializes game */
GameOfLife::GameOfLife()
{
    m_camera.setCenter(0, 0);
    
    m_window.create(m_defaultWindowSize, m_windowTitle);
    m_window.setFramerateLimit(m_FPS);
    updateCamera(m_defaultWindowSize.width, m_defaultWindowSize.height);
    m_gui.setTarget(m_window);

    m_panel->setSize("100%", m_windowSize.y / 5);
    m_panel->setPosition("0%", "parent.height - height");
    m_panel->getRenderer()->setBackgroundColor(m_defaultPanelColor);
    m_panel->getRenderer()->setBorders(tgui::Borders(3));

    m_startButton->setText("Start");
    m_startButton->setTextSize(m_defaultButtonTextSize);
    m_startButton->setSize("20%", "50%");
    m_startButton->setOrigin(.5f, .5f);
    m_startButton->setPosition("50%", "50%");
    m_startButton->getRenderer()->setBackgroundColor(m_defaultButtonColor);
    m_startButton->getRenderer()->setTextColor(sf::Color::Black);
    m_startButton->getRenderer()->setBackgroundColorHover(m_defaultButtonHoverColor);
    m_startButton->getRenderer()->setBackgroundColorDown(m_defaultButtonDownColor);

    m_stepButton->setText("Next Step");
    m_stepButton->setTextSize(m_defaultButtonTextSize);
    m_stepButton->setSize("20%", "50%");
    m_stepButton->setOrigin(.5f, .5f);
    m_stepButton->setPosition("20%", "50%");
    m_stepButton->getRenderer()->setBackgroundColor(m_defaultButtonColor);
    m_stepButton->getRenderer()->setTextColor(sf::Color::Black);
    m_stepButton->getRenderer()->setBackgroundColorHover(m_defaultButtonHoverColor);
    m_stepButton->getRenderer()->setBackgroundColorDown(m_defaultButtonDownColor);

    m_speedSlider->setMinimum(m_minSliderSpeed);
    m_speedSlider->setMaximum(m_maxSliderSpeed);
    m_speedSlider->setWidth("20%");
    m_speedSlider->setOrigin(.5f, .5f);
    m_speedSlider->setPosition("80%", "60%");
    m_speedSlider->getRenderer()->setThumbColor(m_defaultSliderColor);
    m_speedSlider->getRenderer()->setThumbColorHover(m_defaultSliderHoverColor);
    m_speedSlider->getRenderer()->setTrackColor(m_defaultSliderColor);
    m_speedSlider->getRenderer()->setTrackColorHover(m_defaultSliderHoverColor);
    m_speedSlider->getRenderer()->setBorderColor(sf::Color::Black);
    m_speedSlider->getRenderer()->setBorderColorHover(sf::Color::Black);
    m_speedSlider->onValueChange(&GameOfLife::updateSpeedLabel, this);

    m_speedLabel->setTextSize(m_defaultLabelTextSize);
    m_speedLabel->setText("Speed: 1x");
    m_speedLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    m_speedLabel->getRenderer()->setTextColor(m_defaultLabelColor);
    m_speedLabel->setWidth("20%");
    m_speedLabel->setOrigin(.5f, .5f);
    m_speedLabel->setPosition("80%", "30%");
    m_speedLabel->getRenderer()->setTextOutlineThickness(2);
    m_speedLabel->getRenderer()->setTextOutlineColor(sf::Color::Black);

    m_panel->add(m_startButton);
    m_panel->add(m_stepButton);
    m_panel->add(m_speedSlider);
    m_panel->add(m_speedLabel);

    m_gui.add(m_panel);


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
        m_gui.handleEvent(e);

        if(e.type == sf::Event::Closed)
            m_window.close();
        else if(e.type == sf::Event::Resized)
            updateCamera(e.size.width, e.size.height);
        else if(e.type == sf::Event::MouseWheelMoved && m_window.hasFocus())
            updateGridsize(e.mouseWheel.delta);
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
    float startX = int(m_horizontalPoints[0].position.x / m_gridSize) * m_gridSize;
    float startY = int(m_verticalPoints[0].position.y / m_gridSize) * m_gridSize;

    for(int x = startX; x < m_horizontalPoints[1].position.x; x += m_gridSize)
    {
        m_verticalPoints[0].position.x = x;
        m_verticalPoints[1].position.x = x;
        m_window.draw(m_verticalPoints, 2, sf::Lines);
    }

    for(int y = startY; y < m_verticalPoints[1].position.y; y += m_gridSize)
    {
        m_horizontalPoints[0].position.y = y;
        m_horizontalPoints[1].position.y = y;
        m_window.draw(m_horizontalPoints, 2, sf::Lines);
    }

    m_gui.draw();

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
                sf::Vector2f delta = {m_firstClickPosition.x - mousePos.x, m_firstClickPosition.y - mousePos.y};
                delta.x *= m_scale;
                delta.y *= m_scale;

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
    return (ms.x >= 0 && ms.x <= m_window.getSize().x && ms.y >= 0 && ms.y <= m_panel->getPosition().y);
}

/* Updates gridsize when zoomed */
void GameOfLife::updateGridsize(int delta)
{
    float zoom = 1 / std::exp(delta * m_zoomIntensity);
    if(m_scale * zoom > 5.f) return;
    else if(m_scale * zoom < 0.3f) return;
    
    m_camera.zoom(zoom);
    updateCamera();
    m_scale *= zoom;
}

void GameOfLife::updateSpeedLabel()
{
    std::string sliderValueStr = std::to_string(int(m_speedSlider->getValue()));
    m_speedLabel->setText("Speed: " + sliderValueStr + "x");
}
