#include <SFML/Graphics.hpp>
#include <optional>

#include "screens/MainMenuScreen.hpp"
#include "screens/LinkedListScreen.hpp"
#include "screens/HashTableScreen.hpp"
#include "screens/AVLTreeScreen.hpp"
#include "screens/GraphScreen.hpp"

enum class AppScreen
{
    MainMenu,
    LinkedListVisualizer,
    HashTableVisualizer,
    AVLTreeVisualizer,
    GraphVisualizer // Thêm trạng thái màn hình cho Graph
};

int main()
{
    sf::RenderWindow window(sf::VideoMode({1280, 720}), "Data Structure Visualizer");
    window.setFramerateLimit(60);

    const sf::Font font("assets/fonts/roboto.ttf");

    MainMenuScreen mainMenuScreen(font);
    LinkedListScreen linkedListScreen(font);
    HashTableScreen hashTableScreen(font);
    AVLTreeScreen avlTreeScreen(font);
    GraphScreen graphScreen(font);

    AppScreen currentScreen = AppScreen::MainMenu;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto *resized = event->getIf<sf::Event::Resized>())
            {
                sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized->size));
                window.setView(sf::View(visibleArea));
            }
            else
            {
                if (currentScreen == AppScreen::MainMenu)
                {
                    if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>())
                    {
                        if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                            window.close();
                    }

                    std::optional<StructureType> requestedStructure;
                    mainMenuScreen.handleEvent(*event, window, requestedStructure);

                    if (requestedStructure.has_value())
                    {
                        if (requestedStructure.value() == StructureType::LinkedList)
                            currentScreen = AppScreen::LinkedListVisualizer;
                        else if (requestedStructure.value() == StructureType::HashTable)
                        {
                            currentScreen = AppScreen::HashTableVisualizer;
                        }
                        else if (requestedStructure.value() == StructureType::AVLTree)
                            currentScreen = AppScreen::AVLTreeVisualizer;
                        // Bắt sự kiện khi người dùng chọn Graph ở Main Menu
                        else if (requestedStructure.value() == StructureType::Graph) 
                        {
                            currentScreen = AppScreen::GraphVisualizer;
                        }
                    }
                }
                else if (currentScreen == AppScreen::LinkedListVisualizer)
                {
                    bool goBack = false;
                    linkedListScreen.handleEvent(*event, window, goBack);
                    
                    if (goBack) currentScreen = AppScreen::MainMenu;
                }
                else if (currentScreen == AppScreen::HashTableVisualizer)
                {
                    bool goBack = false;
                    hashTableScreen.handleEvent(*event, window, goBack);

                    if (goBack)
                        currentScreen = AppScreen::MainMenu;
                }
                // Xử lý sự kiện click chuột/bàn phím cho màn hình Graph
                else if (currentScreen == AppScreen::GraphVisualizer)
                {
                    bool goBack = false;
                    graphScreen.handleEvent(*event, window, goBack);

                    if (goBack)
                        currentScreen = AppScreen::MainMenu;
                }
                else if (currentScreen == AppScreen::AVLTreeVisualizer) // Xử lý Event cho AVL
                {
                    bool goBack = false;
                    avlTreeScreen.handleEvent(*event, window, goBack);
                    if (goBack)
                        currentScreen = AppScreen::MainMenu;
                }
            }
        }

        // Cập nhật logic (update)
        if (currentScreen == AppScreen::MainMenu)
            mainMenuScreen.update(window);
        else if (currentScreen == AppScreen::LinkedListVisualizer)
            linkedListScreen.update(window);
        else if (currentScreen == AppScreen::HashTableVisualizer)
            hashTableScreen.update(window);
        else if (currentScreen == AppScreen::AVLTreeVisualizer)
            avlTreeScreen.update(window);
        else if (currentScreen == AppScreen::GraphVisualizer) // Thêm update cho Graph
            graphScreen.update(window);

        window.clear(sf::Color(30, 30, 45));

        // Vẽ lên màn hình (draw)
        if (currentScreen == AppScreen::MainMenu)
            mainMenuScreen.draw(window);
        else if (currentScreen == AppScreen::LinkedListVisualizer)
            linkedListScreen.draw(window);
        else if (currentScreen == AppScreen::HashTableVisualizer)
            hashTableScreen.draw(window);
        else if (currentScreen == AppScreen::AVLTreeVisualizer)
            avlTreeScreen.draw(window);
        else if (currentScreen == AppScreen::GraphVisualizer) // Thêm draw cho Graph
            graphScreen.draw(window);

        window.display();
    }

    return 0;
}