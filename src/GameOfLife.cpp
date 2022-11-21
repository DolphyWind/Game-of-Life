#include "GameOfLife.hpp"ü

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
    m_startButton->setSize("15%", "35%");
    m_startButton->setOrigin(.5f, .5f);
    m_startButton->setPosition("42%", "30%");
    m_startButton->getRenderer()->setBackgroundColor(m_defaultButtonColor);
    m_startButton->getRenderer()->setTextColor(sf::Color::Black);
    m_startButton->getRenderer()->setBackgroundColorHover(m_defaultButtonHoverColor);
    m_startButton->getRenderer()->setBackgroundColorDown(m_defaultButtonDownColor);
    m_startButton->onClick(&GameOfLife::changeStatus, this);

    m_stepButton->setText("Next Step");
    m_stepButton->setTextSize(m_defaultButtonTextSize);
    m_stepButton->setSize("15%", "35%");
    m_stepButton->setOrigin(.5f, .5f);
    m_stepButton->setPosition("58%", "30%");
    m_stepButton->getRenderer()->setBackgroundColor(m_defaultButtonColor);
    m_stepButton->getRenderer()->setTextColor(sf::Color::Black);
    m_stepButton->getRenderer()->setBackgroundColorHover(m_defaultButtonHoverColor);
    m_stepButton->getRenderer()->setBackgroundColorDown(m_defaultButtonDownColor);
    m_stepButton->onClick(&GameOfLife::step, this);

    m_previousStepButton->setText("Previous Step");
    m_previousStepButton->setTextSize(m_defaultButtonTextSize);
    m_previousStepButton->setSize("15%", "35%");
    m_previousStepButton->setOrigin(.5f, .5f);
    m_previousStepButton->setPosition("42%", "70%");
    m_previousStepButton->getRenderer()->setBackgroundColor(m_defaultButtonColor);
    m_previousStepButton->getRenderer()->setTextColor(sf::Color::Black);
    m_previousStepButton->getRenderer()->setBackgroundColorHover(m_defaultButtonHoverColor);
    m_previousStepButton->getRenderer()->setBackgroundColorDown(m_defaultButtonDownColor);
    m_previousStepButton->onClick(&GameOfLife::gotoPreviousStep, this);

    m_clearButton->setText("Clear");
    m_clearButton->setTextSize(m_defaultButtonTextSize);
    m_clearButton->setSize("15%", "35%");
    m_clearButton->setOrigin(.5f, .5f);
    m_clearButton->setPosition("58%", "70%");
    m_clearButton->getRenderer()->setBackgroundColor(m_defaultButtonColor);
    m_clearButton->getRenderer()->setTextColor(sf::Color::Black);
    m_clearButton->getRenderer()->setBackgroundColorHover(m_defaultButtonHoverColor);
    m_clearButton->getRenderer()->setBackgroundColorDown(m_defaultButtonDownColor);
    m_clearButton->onClick(&GameOfLife::clearBoard, this);

    m_speedSlider->setMinimum(m_minSliderSpeed);
    m_speedSlider->setMaximum(m_maxSliderSpeed);
    m_speedSlider->setWidth("20%");
    m_speedSlider->setOrigin(.5f, .5f);
    m_speedSlider->setPosition("85%", "60%");
    m_speedSlider->getRenderer()->setThumbColor(m_defaultSliderColor);
    m_speedSlider->getRenderer()->setThumbColorHover(m_defaultSliderHoverColor);
    m_speedSlider->getRenderer()->setTrackColor(m_defaultSliderColor);
    m_speedSlider->getRenderer()->setTrackColorHover(m_defaultSliderHoverColor);
    m_speedSlider->getRenderer()->setBorderColor(sf::Color::Black);
    m_speedSlider->getRenderer()->setBorderColorHover(sf::Color::Black);
    m_speedSlider->onValueChange(&GameOfLife::updateSpeed, this);

    m_speedLabel->setTextSize(m_defaultLabelTextSize);
    m_speedLabel->setText("Speed: 1x");
    m_speedLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    m_speedLabel->getRenderer()->setTextColor(m_defaultLabelColor);
    m_speedLabel->setWidth("20%");
    m_speedLabel->setOrigin(.5f, .5f);
    m_speedLabel->setPosition("85%", "30%");
    m_speedLabel->getRenderer()->setTextOutlineThickness(2);
    m_speedLabel->getRenderer()->setTextOutlineColor(sf::Color::Black);

    m_stepLabel->setTextSize(m_defaultLabelTextSize);
    m_stepLabel->setText("Step: 0");
    m_stepLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
    m_stepLabel->getRenderer()->setTextColor(m_defaultLabelColor);
    m_stepLabel->setWidth("20%");
    m_stepLabel->setOrigin(.5f, .5f);
    m_stepLabel->setPosition("15%", "50%");
    m_stepLabel->getRenderer()->setTextOutlineThickness(2);
    m_stepLabel->getRenderer()->setTextOutlineColor(sf::Color::Black);

    m_panel->add(m_startButton);
    m_panel->add(m_stepButton);
    m_panel->add(m_previousStepButton);
    m_panel->add(m_clearButton);
    m_panel->add(m_speedSlider);
    m_panel->add(m_speedLabel);
    m_panel->add(m_stepLabel);

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
    if(m_window.hasFocus())
    {
        handleMouseInputs();
    }

    if(m_isStarted)
    {
        if(m_stepClock.getElapsedTime() >= m_stepTime)
        {
            m_stepClock.restart();
            step();
        }
    }
}

/* Renders in-game elements */
void GameOfLife::render()
{
    m_window.clear(m_clearColor);

    /* Draw cells */
    for(auto &c : m_cells)
        c.draw(&m_window);

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
        m_camera.zoom(m_scale);
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
            float distance = std::hypot(m_firstClickPosition.x - mousePos.x, m_firstClickPosition.y - mousePos.y);

            if(distance > 10 && !m_hasMovedMouse)
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
            m_firstClickPixel = m_window.mapPixelToCoords(m_firstClickPosition);
            sf::Vector2f gridPositionF = m_firstClickPixel / m_gridSize;
            sf::Vector2i gridPosition;
            gridPosition.x = std::floor(gridPositionF.x);
            gridPosition.y = std::floor(gridPositionF.y);

            bool removed = false;
            for(int i = 0; i < m_cells.size(); i++)
            {
                if(gridPosition == m_cells[i].getGridPosition())
                {
                    m_cells.erase(m_cells.begin() + i);
                    removed = true;
                    break;
                }
            }
            if(!removed)
            {
                m_cells.push_back(Cell(m_gridSize, m_cellColor));
                m_cells[m_cells.size() - 1].setGridPosition(gridPosition);
            }
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

    if(m_scale * zoom > 3.f) return;
    else if(m_scale * zoom < 0.2f) return;
    
    m_camera.zoom(zoom);
    updateCamera();
    m_scale *= zoom;
}

void GameOfLife::updateSpeed()
{
    std::string sliderValueStr = std::to_string(int(m_speedSlider->getValue()));
    m_speedLabel->setText("Speed: " + sliderValueStr + "x");
    m_stepTime = sf::seconds(1 / m_speedSlider->getValue());
}

void GameOfLife::step()
{
    if(m_cells.empty()) return;
    
    // Save current positions to previous states
    m_previousStates.push_back({});
    std::size_t lastIndex = m_previousStates.size() - 1;

    for(auto &c : m_cells)
    {
        sf::Vector2i coordsVec = c.getGridPosition();
        Ipair coords(coordsVec.x, coordsVec.y);
        m_previousStates[lastIndex].push_back(coords);
    }

    m_neighborCounter.clear();

    for(int i = 0; i < m_cells.size(); i++)
    {
        for(auto &n : m_cells[i].getNeighborCoordinates())
        {
            m_neighborCounter[{n.x, n.y}] ++;
        }
    }

    std::set<Ipair> nextGenCoords;
    // Add surviving cells to nextGenCoords
    for(auto &c : m_cells)
    {
        sf::Vector2i gridCoord = c.getGridPosition();
        Ipair gridPair(gridCoord.x, gridCoord.y);
        try
        {
            std::uint32_t neighbors = m_neighborCounter.at(gridPair);
            if(neighbors == 2 || neighbors == 3) nextGenCoords.insert(gridPair);
        }
        catch(const std::exception& e) {};
    }

    m_cells.clear();
    
    // Add newborn cells to nextGenCoords
    for(auto &p : m_neighborCounter)
    {
        if(p.second == 3)
        {
            Ipair coords(p.first.first, p.first.second);
            nextGenCoords.insert(coords);
        }
    }

    for(auto &p : nextGenCoords)
    {
        m_cells.push_back(Cell(m_gridSize, m_cellColor));
        m_cells[m_cells.size() - 1].setGridPosition({p.first, p.second});
    }

    m_currentStep ++;
    m_stepLabel->setText("Step: " + std::to_string(m_currentStep));
}

void GameOfLife::clearBoard()
{
    m_cells.clear();
    m_currentStep = 0;
    m_stepLabel->setText("Step: 0");
    if(m_isStarted)
    {
        m_isStarted = false;
        m_startButton->setText("Start");
    }
}

void GameOfLife::changeStatus()
{
    if(m_cells.empty()) return;
    m_isStarted = !m_isStarted;
    if(m_isStarted)
    {
        m_startButton->setText("Stop");
    }
    else
    {
        m_startButton->setText("Start");
    }
}

void GameOfLife::gotoPreviousStep()
{
    if(m_previousStates.empty()) return;
    std::vector<Ipair>& coords = m_previousStates[m_previousStates.size() - 1];
    m_cells.clear();

    for(auto &c : coords)
    {
        m_cells.push_back(Cell(m_gridSize, m_cellColor));
        m_cells[m_cells.size() - 1].setGridPosition({c.first, c.second});
    }
    m_previousStates.erase(m_previousStates.end() - 1);
    m_currentStep --;
    m_stepLabel->setText("Step: " + std::to_string(m_currentStep));
}
