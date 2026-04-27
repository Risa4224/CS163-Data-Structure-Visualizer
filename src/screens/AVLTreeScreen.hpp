#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include "../ui/Button.hpp"
#include "../AVLstructure.h"

struct Slider
{
    sf::RectangleShape track;
    sf::CircleShape knob;
    float value;

    Slider(sf::Vector2f pos, float width)
    {
        track.setSize(sf::Vector2f(width, 10.f));
        track.setPosition(pos);
        track.setFillColor(sf::Color(180, 180, 180));

        knob.setRadius(12.f);
        knob.setFillColor(sf::Color(41, 128, 185));
        knob.setOrigin(sf::Vector2f(12.f, 12.f));
        knob.setPosition(sf::Vector2f(pos.x + width / 2.f, pos.y + 5.f));
        value = 0.5f;
    }

    void update(sf::Vector2f mousePos, bool isPressed)
    {
        if (isPressed && track.getGlobalBounds().contains(mousePos))
        {
            float trackX = track.getPosition().x;
            float trackWidth = track.getSize().x;
            float newX = std::max(trackX, std::min(mousePos.x, trackX + trackWidth));

            knob.setPosition(sf::Vector2f(newX, knob.getPosition().y));
            value = (newX - trackX) / trackWidth;
        }
    }

    void draw(sf::RenderWindow &window) const
    {
        window.draw(track);
        window.draw(knob);
    }
};

class AVLTreeScreen
{
private:
    const sf::Font &m_font;

    sf::RectangleShape m_background;
    sf::RectangleShape m_leftBar;
    sf::RectangleShape m_menuPanel;
    sf::RectangleShape m_inputBox;
    sf::RectangleShape m_statusBox;

    sf::Text m_topInfoText;
    sf::Text m_messageText;
    sf::Text m_inputLabel;
    sf::Text m_inputText;
    sf::Text m_stepText;

    Button m_insertButton;
    Button m_removeButton;
    Button m_searchButton;
    Button m_clearButton;
    Button m_backButton;

    Button m_prevButton;
    Button m_nextButton;
    Button m_autoButton;
    Button m_randomButton;

    AVLTree m_tree;
    std::string m_input;
    std::string m_message = "Ready.";
    int m_currentStep = 0;

    int m_lastStep = 0;
    float m_animProgress = 1.0f;
    sf::Clock m_clock;

    bool m_isAutoPlaying = false;
    float m_pauseTimer = 0.0f;
    float m_autoDelay = 0.8f;

    Slider m_speedSlider;
    bool m_isDraggingSlider = false;
    sf::Text m_speedLabel;

    std::vector<std::string> m_codeLines;
    mutable std::vector<sf::Text> m_codeTexts;
    sf::RectangleShape m_codeBackground;

private:
    void centerTextX(sf::Text &text, float x, float y);
    bool parseInput(int &value) const;
    void appendDigit(char digit);
    void removeLastDigit();
    void clearInput();
    void updateTopInfo();

private:
    std::map<std::string, std::vector<std::string>> m_codeRepo;

public:
    explicit AVLTreeScreen(const sf::Font &font);

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window, bool &goBack);
    void update(const sf::RenderWindow &window);
    void draw(sf::RenderWindow &window) const;
    void renderSingleCodeLine(sf::RenderWindow &window, const std::string &code) const;
};