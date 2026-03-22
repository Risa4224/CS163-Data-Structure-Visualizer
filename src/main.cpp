#include <SFML/Graphics.hpp>
#include <optional>
#include <vector>
#include <string>
#include <iostream>

#include "HashTable.hpp"

struct Button
{
    sf::RectangleShape shape;
};

Button makeButton(const sf::Vector2f& position, const sf::Vector2f& size)
{
    Button button;
    button.shape.setSize(size);
    button.shape.setPosition(position);
    button.shape.setFillColor(sf::Color(70, 70, 70));
    button.shape.setOutlineThickness(2.f);
    button.shape.setOutlineColor(sf::Color::White);
    return button;
}

bool isMouseOver(const Button& button, const sf::Vector2f& mousePos)
{
    return button.shape.getGlobalBounds().contains(mousePos);
}

int main()
{
    const unsigned int windowWidth = 1200;
    const unsigned int windowHeight = 1000;

    sf::RenderWindow window(sf::VideoMode({windowWidth, windowHeight}), "6 Buttons");
    window.setFramerateLimit(60);

    HashTable hashTable;

    std::vector<Button> buttons;
    sf::Vector2f buttonSize(200.f, 100.f);

    float gapX = 50.f;
    float gapY = 30.f;
    float totalWidth = 3 * buttonSize.x + 2 * gapX;
    float startX = (windowWidth - totalWidth) / 2.f;

    float bottomMargin = 40.f;
    float row2Y = windowHeight - bottomMargin - buttonSize.y;
    float row1Y = row2Y - gapY - buttonSize.y;

    buttons.push_back(makeButton({startX, row1Y}, buttonSize));
    buttons.push_back(makeButton({startX + buttonSize.x + gapX, row1Y}, buttonSize));
    buttons.push_back(makeButton({startX + 2 * (buttonSize.x + gapX), row1Y}, buttonSize));
    buttons.push_back(makeButton({startX, row2Y}, buttonSize));
    buttons.push_back(makeButton({startX + buttonSize.x + gapX, row2Y}, buttonSize));
    buttons.push_back(makeButton({startX + 2 * (buttonSize.x + gapX), row2Y}, buttonSize));

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
            {
                sf::Vector2f mousePos(
                    static_cast<float>(mouseMoved->position.x),
                    static_cast<float>(mouseMoved->position.y)
                );

                for (auto& button : buttons)
                {
                    if (isMouseOver(button, mousePos))
                        button.shape.setFillColor(sf::Color(100, 100, 220));
                    else
                        button.shape.setFillColor(sf::Color(70, 70, 70));
                }
            }
            else if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>())
            {
                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    sf::Vector2f mousePos(
                        static_cast<float>(mousePressed->position.x),
                        static_cast<float>(mousePressed->position.y)
                    );

                    for (int i = 0; i < static_cast<int>(buttons.size()); i++)
                    {
                        if (isMouseOver(buttons[i], mousePos))
                        {
                            std::cout << "Button " << i + 1 << " clicked!\n";

                            if (i == 0)
                            {
                                HashTable::InsertResult result = hashTable.insertRandom();

                                window.setTitle(
                                    "Inserted key=" + std::to_string(result.key) +
                                    " value=" + std::to_string(result.value) +
                                    " bucket=" + std::to_string(result.bucket)
                                );
                            }
                            else if (i == 5)
                            {
                                window.close();
                            }
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(30, 30, 30));

        hashTable.draw(window);

        for (const auto& button : buttons)
        {
            window.draw(button.shape);
        }

        window.display();
    }

    return 0;
}