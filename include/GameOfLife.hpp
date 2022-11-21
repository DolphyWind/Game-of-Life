#pragma once
#include <SFML/Graphics.hpp>
#include <TGUI/TGUI.hpp> // Tgui 0.9
#include <iostream>
#include <cmath>
#include <Cell.hpp>
#include <map>
#include <unordered_map>
#include <vector>
#include <set>

typedef std::pair<int, int> Ipair;

class GameOfLife
{
private:
    sf::RenderWindow m_window;

    /* Window settings */
    const sf::VideoMode m_defaultWindowSize = {1280, 720};
    const std::string m_windowTitle = "Game of Life";
    const sf::Color m_clearColor = sf::Color(160, 160, 160);
    const sf::Color m_cellColor = sf::Color(146, 0, 179);
    sf::Vector2u m_windowSize;
    float m_gridSize = 25.f;
    sf::View m_camera;
    
    /* Game tick */
    const std::uint32_t m_FPS = 60;
    const sf::Time m_timePerFrame = sf::seconds( 1.f / float(m_FPS) );
    sf::Clock m_frameClock;

    /* In game elements */
    sf::Vertex m_verticalPoints[2];
    sf::Vertex m_horizontalPoints[2];
    std::vector<Cell> m_cells;
    std::vector<std::vector<Ipair>> m_previousStates;
    std::vector<Cell> m_cellsNextGen;
    std::map<Ipair, std::uint32_t> m_neighborCounter;
    std::uint32_t m_currentStep = 0;
    bool m_isStarted = false;
    sf::Clock m_stepClock;
    sf::Time m_stepTime = sf::seconds(1);

    /* Movement & clicking with mouse */
    bool m_isLeftDown = false;
    sf::Vector2i m_firstClickPosition;
    sf::Vector2f m_firstClickPixel;
    bool m_hasMovedMouse = false;
    sf::Vector2f m_firstCamCenter;
    const float m_zoomIntensity = .1f;
    float m_scale = 1;

    /* UI */
    tgui::GuiSFML m_gui;
    tgui::Panel::Ptr m_panel = tgui::Panel::create();
    tgui::Button::Ptr m_startButton = tgui::Button::create();
    tgui::Button::Ptr m_stepButton = tgui::Button::create();
    tgui::Button::Ptr m_previousStepButton = tgui::Button::create();
    tgui::Button::Ptr m_clearButton = tgui::Button::create();
    tgui::Slider::Ptr m_speedSlider = tgui::Slider::create();
    tgui::Label::Ptr m_speedLabel = tgui::Label::create();
    tgui::Label::Ptr m_stepLabel = tgui::Label::create();
    const float m_minSliderSpeed = 1.f; // 1 step per second
    const float m_maxSliderSpeed = 20.f; // 20 steps per second
    const sf::Color m_defaultPanelColor = sf::Color(7, 0, 143);
    const sf::Color m_defaultLabelColor = sf::Color(28, 189, 0);
    const sf::Color m_defaultButtonColor = sf::Color(0, 153, 209);
    const sf::Color m_defaultButtonHoverColor = sf::Color(0, 122, 166);
    const sf::Color m_defaultButtonDownColor = sf::Color(0, 103, 140);
    const sf::Color m_defaultSliderColor = sf::Color(250, 233, 42);
    const sf::Color m_defaultSliderHoverColor = sf::Color(230, 213, 37);
    const std::uint32_t m_defaultButtonTextSize = 26;
    const std::uint32_t m_defaultLabelTextSize = 26;

    void update(sf::Time deltaTime);
    void render();
    void handleEvents();
    void updateCamera(std::uint32_t width = 0, std::uint32_t height = 0);
    void handleMouseInputs();
    bool mouseBoundaryCheck(sf::Vector2i ms);
    void updateGridsize(int delta);
    void updateSpeed();
    void step();
    void clearBoard();
    void changeStatus();
    void gotoPreviousStep();

public:
    GameOfLife();
    ~GameOfLife(){};

    void mainLoop();
};