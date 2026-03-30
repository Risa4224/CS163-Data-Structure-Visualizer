#include "HashTableScreen.hpp"
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <iomanip>

HashTableScreen::HashTableScreen(const sf::Font& font)
    : m_font(font),

      m_topInfoText(font, "", 24),
      m_messageText(font, "Ready.", 20),
      m_inputLabel(font, "Value", 20),
      m_inputText(font, "", 24),

      m_createButton({190.f, 46.f}, {56.f, 300.f}, font, "Create(M, N)", 22),
      m_searchButton({190.f, 46.f}, {56.f, 352.f}, font, "Search(v)", 22),
      m_insertButton({190.f, 46.f}, {56.f, 404.f}, font, "Insert(v)", 22),
      m_removeButton({190.f, 46.f}, {56.f, 456.f}, font, "Remove(v)", 22),
      m_backButton({190.f, 46.f}, {56.f, 508.f}, font, "Back", 22)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    createTable();

    m_background.setSize({1280.f, 720.f});
    m_background.setPosition({0.f, 0.f});
    m_background.setFillColor(sf::Color(212, 212, 212));

    m_leftBar.setSize({52.f, 720.f});
    m_leftBar.setPosition({0.f, 0.f});
    m_leftBar.setFillColor(sf::Color(223, 84, 58));

    m_menuPanel.setSize({210.f, 270.f});
    m_menuPanel.setPosition({52.f, 290.f});
    m_menuPanel.setFillColor(sf::Color(223, 84, 58));

    m_inputLabel.setFillColor(sf::Color::Black);
    m_inputLabel.setPosition({56.f, 150.f});

    m_inputBox.setSize({190.f, 50.f});
    m_inputBox.setPosition({56.f, 180.f});
    m_inputBox.setFillColor(sf::Color::White);
    m_inputBox.setOutlineThickness(2.f);
    m_inputBox.setOutlineColor(sf::Color(80, 80, 80));

    m_inputText.setFillColor(sf::Color::Black);
    m_inputText.setPosition({70.f, 190.f});

    m_topInfoText.setFillColor(sf::Color::Black);
    centerTextX(m_topInfoText, 700.f, 24.f);

    m_messageText.setFillColor(sf::Color(120, 20, 20));
    centerTextX(m_messageText, 700.f, 690.f);

    auto styleMenuButton = [](Button& b)
    {
        b.setNormalColor(sf::Color(223, 84, 58));
        b.setHoverColor(sf::Color(235, 108, 86));
        b.setSelectedColor(sf::Color(190, 65, 45));
    };

    styleMenuButton(m_createButton);
    styleMenuButton(m_searchButton);
    styleMenuButton(m_insertButton);
    styleMenuButton(m_removeButton);
    styleMenuButton(m_backButton);

    updateTopInfo();
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

int HashTableScreen::hashFunction(int value) const
{
    int mod = value % FIXED_N;
    if (mod < 0)
        mod += FIXED_N;
    return mod;
}

bool HashTableScreen::parseInput(int& value) const
{
    if (m_input.empty())
        return false;

    try
    {
        value = std::stoi(m_input);
        return true;
    }
    catch (...)
    {
        return false;
    }
}

void HashTableScreen::appendDigit(char digit)
{
    if (m_input.size() < 9)
        m_input += digit;
}

void HashTableScreen::removeLastDigit()
{
    if (!m_input.empty())
        m_input.pop_back();
}

void HashTableScreen::clearInput()
{
    m_input.clear();
}

void HashTableScreen::createTable()
{
    m_table.clear();
    m_table.resize(FIXED_N);
    m_elementCount = 0;

    int M = 20;
    for (int i = 0; i < M; ++i)
    {
        int value = std::rand() % 100;
        insertValue(value);
    }

    m_message = "Created random hash table";
    updateTopInfo();
}

bool HashTableScreen::searchValue(int value, int& bucketIndex, int& posIndex) const
{
    bucketIndex = hashFunction(value);
    const auto& chain = m_table[bucketIndex];

    for (int i = 0; i < static_cast<int>(chain.size()); ++i)
    {
        if (chain[i] == value)
        {
            posIndex = i;
            return true;
        }
    }

    posIndex = -1;
    return false;
}

bool HashTableScreen::insertValue(int value)
{
    int bucketIndex = -1;
    int posIndex = -1;

    if (searchValue(value, bucketIndex, posIndex))
    {
        m_message = "Value " + std::to_string(value) +
                    " already exists in bucket " + std::to_string(bucketIndex);
        return false;
    }

    bucketIndex = hashFunction(value);
    m_table[bucketIndex].push_back(value);
    ++m_elementCount;

    m_message = "Inserted " + std::to_string(value) +
                " into bucket " + std::to_string(bucketIndex);
    updateTopInfo();
    return true;
}

bool HashTableScreen::removeValue(int value)
{
    int bucketIndex = -1;
    int posIndex = -1;

    if (!searchValue(value, bucketIndex, posIndex))
    {
        m_message = "Value " + std::to_string(value) + " not found";
        return false;
    }

    auto& chain = m_table[bucketIndex];
    chain.erase(chain.begin() + posIndex);
    --m_elementCount;

    m_message = "Removed " + std::to_string(value) +
                " from bucket " + std::to_string(bucketIndex);
    updateTopInfo();
    return true;
}

float HashTableScreen::getAlpha() const
{
    return static_cast<float>(m_elementCount) / static_cast<float>(FIXED_N);
}

void HashTableScreen::updateTopInfo()
{
    std::ostringstream oss;
    oss << "N=" << FIXED_N
        << ", M=" << m_elementCount
        << ", alpha=" << std::fixed << std::setprecision(1) << getAlpha();

    m_topInfoText.setString(oss.str());
    centerTextX(m_topInfoText, 700.f, 24.f);
}

void HashTableScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window, bool& goBack)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.is<sf::Event::MouseMoved>())
    {
        m_createButton.update(mousePos);
        m_searchButton.update(mousePos);
        m_insertButton.update(mousePos);
        m_removeButton.update(mousePos);
        m_backButton.update(mousePos);
    }

    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>())
    {
        char c = static_cast<char>(textEvent->unicode);

        if (c >= '0' && c <= '9')
        {
            appendDigit(c);
        }
        else if (textEvent->unicode == 8)
        {
            removeLastDigit();
        }
        else if (textEvent->unicode == 13)
        {
            int value;
            if (parseInput(value))
            {
                insertValue(value);
                clearInput();
            }
        }
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button != sf::Mouse::Button::Left)
            return;

        int value = 0;

        if (m_backButton.contains(mousePos))
        {
            goBack = true;
            return;
        }

        if (m_createButton.contains(mousePos))
        {
            createTable();
            clearInput();
            return;
        }

        if (m_insertButton.contains(mousePos))
        {
            if (parseInput(value))
            {
                insertValue(value);
                clearInput();
            }
            else
            {
                m_message = "Please enter a valid number";
            }
            return;
        }

        if (m_searchButton.contains(mousePos))
        {
            if (parseInput(value))
            {
                int bucketIndex = -1;
                int posIndex = -1;

                if (searchValue(value, bucketIndex, posIndex))
                {
                    m_message = "Found " + std::to_string(value) +
                                " in bucket " + std::to_string(bucketIndex);
                }
                else
                {
                    m_message = "Value " + std::to_string(value) + " not found";
                }

                clearInput();
            }
            else
            {
                m_message = "Please enter a valid number";
            }
            return;
        }

        if (m_removeButton.contains(mousePos))
        {
            if (parseInput(value))
            {
                removeValue(value);
                clearInput();
            }
            else
            {
                m_message = "Please enter a valid number";
            }
            return;
        }
    }
}

void HashTableScreen::update(const sf::RenderWindow& window)
{
    (void)window;

    if (m_input.empty())
        m_inputText.setString("...");
    else
        m_inputText.setString(m_input);

    m_messageText.setString(m_message);
    centerTextX(m_messageText, 700.f, 690.f);

    updateTopInfo();
}

void HashTableScreen::draw(sf::RenderWindow& window) const
{
    window.draw(m_background);
    window.draw(m_leftBar);
    window.draw(m_menuPanel);

    sf::Text arrow(m_font, "<", 34);
    arrow.setFillColor(sf::Color::White);
    arrow.setPosition({18.f, 500.f});
    window.draw(arrow);

    window.draw(m_topInfoText);

    window.draw(m_inputLabel);
    window.draw(m_inputBox);
    window.draw(m_inputText);

    m_createButton.draw(window);
    m_searchButton.draw(window);
    m_insertButton.draw(window);
    m_removeButton.draw(window);
    m_backButton.draw(window);

    float startX = 300.f;
    float topY = 90.f;
    float columnGap = 90.f;

    for (int i = 0; i < FIXED_N; ++i)
    {
        float x = startX + i * columnGap;

        // index: đưa lên trên ô H
        sf::Text indexText(m_font, std::to_string(i), 18);
        indexText.setFillColor(sf::Color::Red);
        indexText.setPosition({x + 14.f, topY - 24.f});
        window.draw(indexText);

        // H box
        sf::RectangleShape hashBox({42.f, 34.f});
        hashBox.setPosition({x, topY});
        hashBox.setFillColor(sf::Color(235, 235, 235));
        hashBox.setOutlineThickness(2.f);
        hashBox.setOutlineColor(sf::Color(60, 60, 60));
        window.draw(hashBox);

        sf::Text hashText(m_font, "H", 22);
        hashText.setFillColor(sf::Color::Black);
        hashText.setPosition({x + 12.f, topY + 4.f});
        window.draw(hashText);

        sf::Vertex line[2];
        line[0].position = {x + 21.f, topY + 34.f};
        line[0].color = sf::Color(60, 60, 60);
        line[1].position = {x + 21.f, topY + 52.f};
        line[1].color = sf::Color(60, 60, 60);
        window.draw(line, 2, sf::PrimitiveType::Lines);

        const auto& chain = m_table[i];
        for (int j = 0; j < static_cast<int>(chain.size()); ++j)
        {
            float nodeY = topY + 60.f + j * 58.f;

            sf::RectangleShape node({56.f, 38.f});
            node.setPosition({x - 7.f, nodeY});
            node.setFillColor(sf::Color(245, 245, 245));
            node.setOutlineThickness(2.f);
            node.setOutlineColor(sf::Color(60, 60, 60));
            window.draw(node);

            sf::Text valueText(m_font, std::to_string(chain[j]), 22);
            valueText.setFillColor(sf::Color::Black);

            sf::FloatRect tb = valueText.getLocalBounds();
            valueText.setOrigin({
                tb.position.x + tb.size.x / 2.f,
                tb.position.y + tb.size.y / 2.f
            });
            valueText.setPosition({x + 21.f, nodeY + 19.f});
            window.draw(valueText);

            if (j < static_cast<int>(chain.size()) - 1)
            {
                sf::Vertex vline[2];
                vline[0].position = {x + 21.f, nodeY + 38.f};
                vline[0].color = sf::Color(60, 60, 60);
                vline[1].position = {x + 21.f, nodeY + 58.f};
                vline[1].color = sf::Color(60, 60, 60);
                window.draw(vline, 2, sf::PrimitiveType::Lines);
            }
        }
    }

    window.draw(m_messageText);
}