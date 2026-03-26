#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <string>

#include "ui/Button.hpp"

enum class StructureType
{
    None,
    LinkedList,
    HashTable,
    AVLTree,
    Graph
};

class MainMenuScreen
{
private:
    const sf::Font& m_font;

    sf::Text m_title;
    sf::Text m_subtitle;
    sf::Text m_selectedText;

    std::vector<Button> m_buttons;
    StructureType m_selectedStructure = StructureType::None;

    void centerTextX(sf::Text& text, float x, float y);
    void updateSelection(int index);

public:
    explicit MainMenuScreen(const sf::Font& font);

    void handleEvent(const sf::Event& event,
                     sf::RenderWindow& window,
                     std::optional<StructureType>& requestedStructure);

    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
};