#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "../ui/Button.hpp"
#include "../AVLstructure.h"

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

    AVLTree m_tree;
    std::string m_input;
    std::string m_message = "Ready.";
    int m_currentStep = 0;

    int m_lastStep = 0;
    float m_animProgress = 1.0f;
    sf::Clock m_clock;

    bool m_isAutoPlaying = false;
    float m_pauseTimer = 0.0f;

private:
    void centerTextX(sf::Text &text, float x, float y);
    bool parseInput(int &value) const;
    void appendDigit(char digit);
    void removeLastDigit();
    void clearInput();
    void updateTopInfo();

public:
    explicit AVLTreeScreen(const sf::Font &font);

    void handleEvent(const sf::Event &event, const sf::RenderWindow &window, bool &goBack);
    void update(const sf::RenderWindow &window);
    void draw(sf::RenderWindow &window) const;
};