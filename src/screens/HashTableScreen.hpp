#pragma once

#include <SFML/Graphics.hpp>

class HashTableScreen
{
private:
    const sf::Font& m_font;

    sf::Text m_title;
    sf::Text m_infoText;
    sf::Text m_backText;

    void centerTextX(sf::Text& text, float x, float y);

public:
    explicit HashTableScreen(const sf::Font& font);

    void handleEvent(const sf::Event& event, bool& goBack);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
};