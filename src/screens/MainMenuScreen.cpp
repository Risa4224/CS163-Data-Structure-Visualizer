#include "MainMenuScreen.hpp"

#include <iostream>

MainMenuScreen::MainMenuScreen(const sf::Font& font)
    : m_font(font),
      m_title(font, "Data Structure Visualizer", 42),
      m_subtitle(font, "Choose a data structure to visualize", 24),
      m_selectedText(font, "Selected: None", 26)
{
    m_title.setFillColor(sf::Color::White);
    m_subtitle.setFillColor(sf::Color(210, 210, 210));
    m_selectedText.setFillColor(sf::Color(255, 220, 120));

    centerTextX(m_title, 640.f, 70.f);
    centerTextX(m_subtitle, 640.f, 130.f);
    centerTextX(m_selectedText, 640.f, 650.f);

    const sf::Vector2f buttonSize{280.f, 80.f};
    const float horizontalGap = 100.f;
    const float verticalGap = 50.f;

    const float totalWidth = buttonSize.x * 2 + horizontalGap;
    const float startX = (1280.f - totalWidth) / 2.f;
    const float startY = 230.f;

    const float x1 = startX;
    const float x2 = startX + buttonSize.x + horizontalGap;
    const float y1 = startY;
    const float y2 = startY + buttonSize.y + verticalGap;

    m_buttons.emplace_back(buttonSize, sf::Vector2f{x1, y1}, m_font, "Linked List");
    m_buttons.emplace_back(buttonSize, sf::Vector2f{x2, y1}, m_font, "Hash Table");
    m_buttons.emplace_back(buttonSize, sf::Vector2f{x1, y2}, m_font, "AVL Tree");
    m_buttons.emplace_back(buttonSize, sf::Vector2f{x2, y2}, m_font, "Graph");
}

void MainMenuScreen::centerTextX(sf::Text& text, float x, float y)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });
    text.setPosition({x, y});
}

void MainMenuScreen::updateSelection(int index)
{
    for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
        m_buttons[i].setSelected(i == index);

    switch (index)
    {
    case 0:
        m_selectedStructure = StructureType::LinkedList;
        m_selectedText.setString("Selected: Linked List");
        break;
    case 1:
        m_selectedStructure = StructureType::HashTable;
        m_selectedText.setString("Selected: Hash Table");
        break;
    case 2:
        m_selectedStructure = StructureType::AVLTree;
        m_selectedText.setString("Selected: AVL Tree");
        break;
    case 3:
        m_selectedStructure = StructureType::Graph;
        m_selectedText.setString("Selected: Graph");
        break;
    default:
        m_selectedStructure = StructureType::None;
        m_selectedText.setString("Selected: None");
        break;
    }

    centerTextX(m_selectedText, 640.f, 650.f);
}

void MainMenuScreen::handleEvent(const sf::Event& event,
                                 sf::RenderWindow& window,
                                 std::optional<StructureType>& requestedStructure)
{
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)
        {
            sf::Vector2f mousePos = window.mapPixelToCoords(mousePressed->position);

            for (int i = 0; i < static_cast<int>(m_buttons.size()); ++i)
            {
                if (m_buttons[i].contains(mousePos))
                {
                    updateSelection(i);
                    requestedStructure = m_selectedStructure;
                    break;
                }
            }
        }
    }
}

void MainMenuScreen::update(const sf::RenderWindow& window)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    for (auto& button : m_buttons)
        button.update(mousePos);
}

void MainMenuScreen::draw(sf::RenderWindow& window) const
{
    window.draw(m_title);
    window.draw(m_subtitle);

    for (const auto& button : m_buttons)
        button.draw(window);

    window.draw(m_selectedText);
}