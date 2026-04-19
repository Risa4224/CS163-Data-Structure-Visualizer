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

    // UI Elements
    sf::RectangleShape m_background;
    sf::RectangleShape m_leftBar;
    sf::RectangleShape m_menuPanel;
    sf::RectangleShape m_inputBox;

    sf::Text m_topInfoText;
    sf::Text m_messageText;
    sf::Text m_inputLabel;
    sf::Text m_inputText;
    sf::Text m_stepText; // Hiển thị "Step X / Y"

    // Action Buttons
    Button m_insertButton;
    Button m_removeButton;
    Button m_searchButton;
    Button m_clearButton;
    Button m_backButton;

    // Navigation Buttons (Chuyển bước)
    Button m_prevButton;
    Button m_nextButton;

    // Thuật toán & Trạng thái
    AVLTree m_tree;
    std::string m_input;
    std::string m_message = "Ready.";
    int m_currentStep = 0;

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