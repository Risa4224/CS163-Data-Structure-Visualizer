#include <SFML/Graphics.hpp>
#include <optional>

#include "screens/MainMenuScreen.hpp"
#include "screens/HashTableScreen.hpp"

enum class AppScreen
{
    MainMenu,
    HashTableVisualizer
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Data Structure Visualizer");
    window.setFramerateLimit(60);

    const sf::Font font("assets/fonts/roboto.ttf");

    MainMenuScreen mainMenuScreen(font);
    HashTableScreen hashTableScreen(font);

    AppScreen currentScreen = AppScreen::MainMenu;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* resized = event->getIf<sf::Event::Resized>())
            {
                sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
                window.setView(sf::View(visibleArea));
            }
            else
            {
                if (currentScreen == AppScreen::MainMenu)
                {
                    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
                    {
                        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                            window.close();
                    }

                    std::optional<StructureType> requestedStructure;
                    mainMenuScreen.handleEvent(*event, window, requestedStructure);

                    if (requestedStructure.has_value())
                    {
                        if (requestedStructure.value() == StructureType::HashTable)
                        {
                            currentScreen = AppScreen::HashTableVisualizer;
                        }
                    }
                }
                else if (currentScreen == AppScreen::HashTableVisualizer)
                {
                    bool goBack = false;
                    hashTableScreen.handleEvent(*event, goBack);

                    if (goBack)
                        currentScreen = AppScreen::MainMenu;
                }
            }
        }

        if (currentScreen == AppScreen::MainMenu)
            mainMenuScreen.update(window);
        else if (currentScreen == AppScreen::HashTableVisualizer)
            hashTableScreen.update(window);

        window.clear(sf::Color(30, 30, 45));

        if (currentScreen == AppScreen::MainMenu)
            mainMenuScreen.draw(window);
        else if (currentScreen == AppScreen::HashTableVisualizer)
            hashTableScreen.draw(window);

        window.display();
    }

    return 0;
}