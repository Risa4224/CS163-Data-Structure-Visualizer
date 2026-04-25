#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "../ui/Button.hpp"

class LinkedListScreen
{
private:
    // pjsk color pallete
    // it's my project i get to do whatever i want
    sf::Color c_mizukiColor = sf::Color(228, 168, 202);
    sf::Color c_mafuyuColor = sf::Color(136, 137, 204);
    sf::Color c_enaColor = sf::Color(204, 170, 135);
    sf::Color c_ichikaColor = sf::Color(51, 170, 238);
    sf::Color c_honamiColor = sf::Color(238, 102, 102);
    sf::Color c_sakiColor = sf::Color(255, 221, 68);
    sf::Color c_shihoColor = sf::Color(187, 221, 34);
    
    // UI colors
    sf::Color c_overallBackgroundColor = c_ichikaColor;
    sf::Color c_menuPanelColor = c_enaColor;
    sf::Color c_styleButtonColor = c_mizukiColor;
    sf::Color c_positionBoxBorderColor = c_sakiColor;
    sf::Color c_valueBoxBorderColor = c_shihoColor;
    sf::Color c_messageTextColor = c_honamiColor;

    sf::Color c_errorColor = c_honamiColor;
    sf::Color c_highlightColor = c_mafuyuColor;


    // UI things
    static constexpr int MAXN = 10;
    static constexpr int MAXA = 99; // limit to 2 digits for easier UI
    const sf::Font& m_font;

    bool m_isPositionInputFocused;
    bool m_isValueInputFocused;

    // UI
    sf::Text m_topInfoText;
    sf::Text m_messageText;
    sf::Text m_inputPositionLabel;
    sf::Text m_inputPositionText;
    sf::Text m_inputValueLabel;
    sf::Text m_inputValueText;
    
    sf::RectangleShape m_background;
    sf::RectangleShape m_menuPanel;
    sf::RectangleShape m_inputPositionBox;
    sf::RectangleShape m_inputValueBox;
    
    // input
    std::string m_inputPosition;
    std::string m_inputValue;
    std::string m_message = "Ready.";

    // action buttons
    Button m_generateButton;
    Button m_pushFrontButton;
    Button m_insertButton;
    Button m_popFrontButton;
    Button m_removeButton;
    Button m_updateButton;
    Button m_searchPosButton;
    Button m_backButton;

    //DS - singly linked list
    //in vector form because i am NOT writing an actual linked list
    std::vector <int> list;

private:
    void centerTextX(sf::Text& text, float x, float y);

    void nextStep();
    void lastStep();

    void generate();
    void pushFront(int value);
    void insert(int pos, int value);
    void popFront();
    void remove(int pos);
    void update(int pos, int value);
    void searchPos(int pos);

    bool parseInput(int& value, std::string m_input) const;
    void appendDigit(char digit, std::string &m_input);
    void removeLastDigit(std::string &m_input);
    void clearInput(std::string &m_input);

public:
    explicit LinkedListScreen(const sf::Font& font);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, bool& goBack);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
};