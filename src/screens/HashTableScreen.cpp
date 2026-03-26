#include "HashTableScreen.hpp"

HashTableScreen::HashTableScreen(const sf::Font& font)
    : m_font(font),
      m_title(font, "Hash Table Visualizer", 40),
      m_infoText(font, "This is the Hash Table screen", 24),
      m_backText(font, "Press ESC to return to Main Menu", 22)
{
    m_title.setFillColor(sf::Color::White);
    m_infoText.setFillColor(sf::Color(220, 220, 220));
    m_backText.setFillColor(sf::Color(255, 220, 120));

    centerTextX(m_title, 640.f, 70.f);
    centerTextX(m_infoText, 640.f, 130.f);
    centerTextX(m_backText, 640.f, 170.f);
}

void HashTableScreen::centerTextX(sf::Text& text, float x, float y)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });
    text.setPosition({x, y});
}

void HashTableScreen::handleEvent(const sf::Event& event, bool& goBack)
{
    if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>())
    {
        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        {
            goBack = true;
        }
    }
}

void HashTableScreen::update(const sf::RenderWindow& window)
{
    (void)window;
}

void HashTableScreen::draw(sf::RenderWindow& window) const
{
    window.draw(m_title);
    window.draw(m_infoText);
    window.draw(m_backText);

    sf::RectangleShape panel({900.f, 420.f});
    panel.setPosition({190.f, 220.f});
    panel.setFillColor(sf::Color(45, 45, 65));
    panel.setOutlineThickness(2.f);
    panel.setOutlineColor(sf::Color(120, 120, 160));
    window.draw(panel);

    for (int i = 0; i < 6; ++i)
    {
        sf::Text bucketLabel(m_font, std::to_string(i), 22);
        bucketLabel.setFillColor(sf::Color::White);
        bucketLabel.setPosition({220.f, 245.f + i * 60.f});
        window.draw(bucketLabel);

        sf::RectangleShape bucket({700.f, 42.f});
        bucket.setPosition({280.f, 240.f + i * 60.f});
        bucket.setFillColor(sf::Color(70, 70, 100));
        bucket.setOutlineThickness(1.5f);
        bucket.setOutlineColor(sf::Color::White);
        window.draw(bucket);
    }
}