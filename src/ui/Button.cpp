#include "Button.hpp"

Button::Button(const sf::Vector2f& size,
               const sf::Vector2f& position,
               const sf::Font& font,
               const std::string& label,
               unsigned int characterSize)
    : text(font, label, characterSize),
      normalColor(sf::Color(70, 70, 90)),
      hoverColor(sf::Color(100, 100, 140)),
      selectedColor(sf::Color(70, 130, 220))
{
    shape.setSize(size);
    shape.setPosition(position);
    shape.setFillColor(normalColor);
    shape.setOutlineThickness(2.f);
    shape.setOutlineColor(sf::Color::White);

    text.setFillColor(sf::Color::White);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.f,
        textBounds.position.y + textBounds.size.y / 2.f
    });

    text.setPosition({
        position.x + size.x / 2.f,
        position.y + size.y / 2.f
    });
}

void Button::setPosition(const sf::Vector2f& position)
{
    shape.setPosition(position);

    sf::Vector2f size = shape.getSize();
    text.setPosition({
        position.x + size.x / 2.f,
        position.y + size.y / 2.f
    });
}

void Button::setSize(const sf::Vector2f& size)
{
    shape.setSize(size);

    sf::Vector2f position = shape.getPosition();
    text.setPosition({
        position.x + size.x / 2.f,
        position.y + size.y / 2.f
    });
}

void Button::setText(const std::string& label)
{
    text.setString(label);

    sf::FloatRect textBounds = text.getLocalBounds();
    text.setOrigin({
        textBounds.position.x + textBounds.size.x / 2.f,
        textBounds.position.y + textBounds.size.y / 2.f
    });

    sf::Vector2f position = shape.getPosition();
    sf::Vector2f size = shape.getSize();

    text.setPosition({
        position.x + size.x / 2.f,
        position.y + size.y / 2.f
    });
}

void Button::setNormalColor(const sf::Color& color)
{
    normalColor = color;
    if (!selected)
        shape.setFillColor(normalColor);
}

void Button::setHoverColor(const sf::Color& color)
{
    hoverColor = color;
}

void Button::setSelectedColor(const sf::Color& color)
{
    selectedColor = color;
    if (selected)
        shape.setFillColor(selectedColor);
}

void Button::setSelected(bool value)
{
    selected = value;
    shape.setFillColor(selected ? selectedColor : normalColor);
}

bool Button::isSelected() const
{
    return selected;
}

bool Button::contains(const sf::Vector2f& point) const
{
    return shape.getGlobalBounds().contains(point);
}

void Button::update(const sf::Vector2f& mousePos)
{
    if (selected)
    {
        shape.setFillColor(selectedColor);
        return;
    }

    if (contains(mousePos))
        shape.setFillColor(hoverColor);
    else
        shape.setFillColor(normalColor);
}

void Button::draw(sf::RenderWindow& window) const
{
    window.draw(shape);
    window.draw(text);
}

std::string Button::getTextString() const
{
    return text.getString().toAnsiString();
}