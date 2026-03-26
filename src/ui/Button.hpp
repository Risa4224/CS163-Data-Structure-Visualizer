#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class Button
{
private:
    sf::RectangleShape shape;
    sf::Text text;

    sf::Color normalColor;
    sf::Color hoverColor;
    sf::Color selectedColor;

    bool selected = false;

public:
    Button(const sf::Vector2f& size,
           const sf::Vector2f& position,
           const sf::Font& font,
           const std::string& label,
           unsigned int characterSize = 24);

    void setPosition(const sf::Vector2f& position);
    void setSize(const sf::Vector2f& size);
    void setText(const std::string& label);

    void setNormalColor(const sf::Color& color);
    void setHoverColor(const sf::Color& color);
    void setSelectedColor(const sf::Color& color);

    void setSelected(bool value);
    bool isSelected() const;

    bool contains(const sf::Vector2f& point) const;
    void update(const sf::Vector2f& mousePos);
    void draw(sf::RenderWindow& window) const;

    std::string getTextString() const;
};