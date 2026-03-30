#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "../ui/Button.hpp"

class HashTableScreen
{
private:
    static constexpr int FIXED_N = 10;

    const sf::Font& m_font;

    // UI
    sf::Text m_topInfoText;
    sf::Text m_messageText;
    sf::Text m_inputLabel;
    sf::Text m_inputText;

    sf::RectangleShape m_background;
    sf::RectangleShape m_leftBar;
    sf::RectangleShape m_menuPanel;
    sf::RectangleShape m_inputBox;

    // Hash table by chaining
    std::vector<std::vector<int>> m_table;
    int m_elementCount = 0;

    // input
    std::string m_input;
    std::string m_message = "Ready.";

    // action buttons
    Button m_createButton;
    Button m_searchButton;
    Button m_insertButton;
    Button m_removeButton;
    Button m_backButton;


private:
    void centerTextX(sf::Text& text, float x, float y);

    int hashFunction(int value) const;
    bool parseInput(int& value) const;
    void appendDigit(char digit);
    void removeLastDigit();
    void clearInput();

    void createTable();
    bool searchValue(int value, int& bucketIndex, int& posIndex) const;
    bool insertValue(int value);
    bool removeValue(int value);

    float getAlpha() const;
    void updateTopInfo();

public:
    explicit HashTableScreen(const sf::Font& font);

    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, bool& goBack);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
};