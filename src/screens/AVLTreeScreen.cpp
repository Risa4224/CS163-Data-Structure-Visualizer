#include "AVLTreeScreen.hpp"
#include <sstream>

AVLTreeScreen::AVLTreeScreen(const sf::Font &font) // Khởi tạo giao diện, màu, font, vị trí, kích thước UI
    : m_font(font),
      m_topInfoText(font, "AVL Tree Visualizer", 28),
      m_messageText(font, "Ready.", 20),
      m_inputLabel(font, "Value", 20),
      m_inputText(font, "", 24),
      m_stepText(font, "Step: 0 / 0", 22),

      m_insertButton({190.f, 46.f}, {56.f, 250.f}, font, "Insert(v)", 22),
      m_removeButton({190.f, 46.f}, {56.f, 302.f}, font, "Remove(v)", 22),
      m_searchButton({190.f, 46.f}, {56.f, 354.f}, font, "Search(v)", 22),
      m_clearButton({190.f, 46.f}, {56.f, 406.f}, font, "Clear Tree", 22),
      m_backButton({190.f, 46.f}, {56.f, 508.f}, font, "Back", 22),

      m_prevButton({90.f, 40.f}, {56.f, 600.f}, font, "< Prev", 18),
      m_nextButton({90.f, 40.f}, {156.f, 600.f}, font, "Next >", 18),
      m_autoButton({90.f, 40.f}, {256.f, 600.f}, font, "Auto", 18)

{
    m_background.setSize({1280.f, 720.f});
    m_background.setFillColor(sf::Color(212, 212, 212));

    m_leftBar.setSize({52.f, 720.f});
    m_leftBar.setFillColor(sf::Color(41, 128, 185));

    m_menuPanel.setSize({210.f, 270.f});
    m_menuPanel.setPosition({52.f, 240.f});
    m_menuPanel.setFillColor(sf::Color(41, 128, 185));

    m_inputLabel.setFillColor(sf::Color::Black);
    m_inputLabel.setPosition({56.f, 100.f});

    m_inputBox.setSize({190.f, 50.f});
    m_inputBox.setPosition({56.f, 130.f});
    m_inputBox.setFillColor(sf::Color::White);
    m_inputBox.setOutlineThickness(2.f);
    m_inputBox.setOutlineColor(sf::Color(80, 80, 80));

    m_inputText.setFillColor(sf::Color::Black);
    m_inputText.setPosition({70.f, 140.f});

    m_topInfoText.setFillColor(sf::Color::Black);
    centerTextX(m_topInfoText, 700.f, 24.f);

    m_messageText.setFillColor(sf::Color(120, 20, 20));
    centerTextX(m_messageText, 700.f, 690.f);

    m_stepText.setFillColor(sf::Color::Black);
    m_stepText.setPosition({56.f, 650.f});

    m_statusBox.setSize({800.f, 50.f});
    m_statusBox.setPosition({300.f, 650.f});
    m_statusBox.setFillColor(sf::Color(255, 255, 255, 220));
    m_statusBox.setOutlineThickness(2.f);
    m_statusBox.setOutlineColor(sf::Color(41, 128, 185));

    m_messageText.setFillColor(sf::Color(41, 128, 185));
    m_messageText.setCharacterSize(22);
    m_messageText.setStyle(sf::Text::Bold);

    auto styleMenuButton = [](Button &b)
    {
        b.setNormalColor(sf::Color(41, 128, 185));
        b.setHoverColor(sf::Color(52, 152, 219));
        b.setSelectedColor(sf::Color(31, 97, 141));
    };

    styleMenuButton(m_insertButton);
    styleMenuButton(m_removeButton);
    styleMenuButton(m_searchButton);
    styleMenuButton(m_clearButton);
    styleMenuButton(m_backButton);
    styleMenuButton(m_prevButton);
    styleMenuButton(m_nextButton);
    styleMenuButton(m_autoButton);
}

void AVLTreeScreen::centerTextX(sf::Text &text, float x, float y) // Căn giữa
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    text.setPosition({x, y});
}

bool AVLTreeScreen::parseInput(int &value) const // String -> Int
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

void AVLTreeScreen::appendDigit(char digit) // Thêm string -> input
{
    if (m_input.size() < 6)
        m_input += digit;
}

void AVLTreeScreen::removeLastDigit() // Xóa string -> input
{
    if (!m_input.empty())
        m_input.pop_back();
}

void AVLTreeScreen::clearInput() // Clear input
{
    m_input.clear();
}

void AVLTreeScreen::handleEvent(const sf::Event &event, const sf::RenderWindow &window, bool &goBack) // Xử lý sự kiện chuột, bàn phím
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.is<sf::Event::MouseMoved>())
    {
        m_insertButton.update(mousePos);
        m_removeButton.update(mousePos);
        m_searchButton.update(mousePos);
        m_clearButton.update(mousePos);
        m_backButton.update(mousePos);
        m_prevButton.update(mousePos);
        m_nextButton.update(mousePos);
        m_autoButton.update(mousePos);
    }

    if (const auto *textEvent = event.getIf<sf::Event::TextEntered>())
    {
        char c = static_cast<char>(textEvent->unicode);
        if ((c >= '0' && c <= '9') || (c == '-' && m_input.empty()))
        {
            appendDigit(c);
        }
        else if (textEvent->unicode == 8)
        { // Backspace
            removeLastDigit();
        }
    }

    if (const auto *keyEvent = event.getIf<sf::Event::KeyPressed>()) // điều hướng keyboard
    {
        if (keyEvent->code == sf::Keyboard::Key::Right)
        {
            if (m_currentStep < (int)m_tree.animationStates.size() - 1)
                m_currentStep++;
        }
        if (keyEvent->code == sf::Keyboard::Key::Left)
        {
            if (m_currentStep > 0)
                m_currentStep--;
        }
    }

    if (const auto *mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) // click
    {
        if (mousePressed->button != sf::Mouse::Button::Left)
            return;

        int value = 0;

        if (m_backButton.contains(mousePos))
        {
            goBack = true;
            return;
        }

        if (m_prevButton.contains(mousePos) && m_currentStep > 0)
        {
            m_currentStep--;
            m_animProgress = 0.0f;
            m_isAutoPlaying = false;
            m_autoButton.setText("Auto");
            return;
        }
        if (m_nextButton.contains(mousePos) && m_currentStep < (int)m_tree.animationStates.size() - 1)
        {
            m_currentStep++;
            m_animProgress = 0.0f;
            m_isAutoPlaying = false;
            m_autoButton.setText("Auto");
            return;
        }
        if (m_autoButton.contains(mousePos))
        {
            if (m_currentStep == (int)m_tree.animationStates.size() - 1)
            {
                m_currentStep = 0;
                m_animProgress = 0.0f;
            }
            m_isAutoPlaying = !m_isAutoPlaying;
            m_autoButton.setText(m_isAutoPlaying ? "Stop" : "Auto");
            return;
        }

        if (m_clearButton.contains(mousePos))
        {
            m_tree.clear();
            m_currentStep = 0;
            m_lastStep = 0;
            m_animProgress = 1.0f;
            m_isAutoPlaying = false;
            m_autoButton.setText("Auto");
            m_message = "Tree cleared.";
            return;
        }

        if (m_insertButton.contains(mousePos))
        {
            if (parseInput(value))
            {
                m_tree.insert(value);
                m_currentStep = 0;
                clearInput();
            }
            else
            {
                m_message = "Invalid input.";
            }
            return;
        }

        if (m_removeButton.contains(mousePos))
        {
            if (parseInput(value))
            {
                m_tree.remove(value);
                m_currentStep = 0;
                clearInput();
            }
            else
            {
                m_message = "Invalid input.";
            }
            return;
        }

        if (m_searchButton.contains(mousePos))
        {
            if (parseInput(value))
            {
                m_tree.search(value);
                m_currentStep = 0;
                clearInput();
            }
            else
            {
                m_message = "Invalid input.";
            }
            return;
        }
    }
}

void AVLTreeScreen::update(const sf::RenderWindow &window) // update trạng thái, animation, message, step text
{
    (void)window;
    m_inputText.setString(m_input.empty() ? "..." : m_input);

    float dt = m_clock.restart().asSeconds();
    if (m_lastStep != m_currentStep) // animation
    {
        m_animProgress += dt * 2.0f;
        if (m_animProgress >= 1.0f)
        {
            m_animProgress = 1.0f;
            m_lastStep = m_currentStep;
            m_pauseTimer = 0.0f;
        }
    }
    else if (m_isAutoPlaying)
    {
        m_pauseTimer += dt;

        if (m_pauseTimer >= 0.8f)
        {
            if (m_currentStep < (int)m_tree.animationStates.size() - 1) // step +
            {
                m_currentStep++;
                m_animProgress = 0.0f;
            }
            else
            {

                m_isAutoPlaying = false;
                m_autoButton.setText("Auto");
            }
        }
    }

    if (!m_tree.animationStates.empty() && m_currentStep < m_tree.animationStates.size()) // message
    {
        m_message = m_tree.animationStates[m_currentStep].action;
        m_stepText.setString("Step: " + std::to_string(m_currentStep + 1) + " / " + std::to_string(m_tree.animationStates.size()));
    }
    else
    {
        m_stepText.setString("Step: 0 / 0");
    }

    m_messageText.setString(m_message);
    centerTextX(m_messageText, 700.f, 670.f);
    ;
}

void AVLTreeScreen::draw(sf::RenderWindow &window) const // Vẽ tất cả UI, cây, animation
{
    window.draw(m_background);
    window.draw(m_leftBar);
    window.draw(m_menuPanel);

    window.draw(m_topInfoText);
    window.draw(m_inputLabel);
    window.draw(m_inputBox);
    window.draw(m_inputText);
    window.draw(m_stepText);

    m_insertButton.draw(window);
    m_removeButton.draw(window);
    m_searchButton.draw(window);
    m_clearButton.draw(window);
    m_backButton.draw(window);
    m_prevButton.draw(window);
    m_nextButton.draw(window);
    m_autoButton.draw(window);

    window.draw(m_statusBox);
    window.draw(m_messageText);

    if (m_tree.animationStates.empty())
        return;

    const TreeState &targetState = m_tree.animationStates[m_currentStep];
    const TreeState &lastState = m_tree.animationStates[m_lastStep];
    float radius = 25.f;

    auto getLerpedPos = [&](int nodeData) -> sf::Vector2f
    {
        sf::Vector2f oldPos(-100.f, -100.f), newPos(-100.f, -100.f);

        for (const auto &n : lastState.captures)
        {
            if (n.data == nodeData)
            {
                oldPos = {(float)n.x, (float)n.y};
                break;
            }
        }
        for (const auto &n : targetState.captures)
        {
            if (n.data == nodeData)
            {
                newPos = {(float)n.x, (float)n.y};
                break;
            }
        }

        if (oldPos.x == -100.f) //
            oldPos = {newPos.x, newPos.y - 50.f};

        return {
            oldPos.x + (newPos.x - oldPos.x) * m_animProgress,
            oldPos.y + (newPos.y - oldPos.y) * m_animProgress};
    };

    for (const auto &parent : targetState.captures) // Vẽ line
    {
        sf::Vector2f pPos = getLerpedPos(parent.data);
        if (parent.left != -1)
        {
            sf::Vector2f cPos = getLerpedPos(targetState.captures[parent.left].data);
            sf::Vertex line[2];
            line[0].position = pPos;
            line[0].color = sf::Color(100, 100, 100);
            line[1].position = cPos;
            line[1].color = sf::Color(100, 100, 100);
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
        if (parent.right != -1)
        {
            sf::Vector2f cPos = getLerpedPos(targetState.captures[parent.right].data);
            sf::Vertex line[2];
            line[0].position = pPos;
            line[0].color = sf::Color(100, 100, 100);
            line[1].position = cPos;
            line[1].color = sf::Color(100, 100, 100);
            window.draw(line, 2, sf::PrimitiveType::Lines);
        }
    }

    for (const auto &node : targetState.captures) // Vẽ node
    {
        sf::Vector2f currentPos = getLerpedPos(node.data);

        sf::CircleShape circle(radius);
        circle.setOutlineThickness(3.f);
        circle.setPosition({currentPos.x - radius, currentPos.y - radius});

        if (node.data == targetState.highlightData) // highlight node
        {
            circle.setFillColor(sf::Color(231, 76, 60));
            circle.setOutlineColor(sf::Color(241, 196, 15));
        }
        else
        {
            circle.setFillColor(sf::Color(46, 204, 113));
            circle.setOutlineColor(sf::Color::White);
        }
        window.draw(circle);

        sf::Text text(m_font, std::to_string(node.data), 20); // text node
        text.setFillColor(sf::Color::White);
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f});
        text.setPosition(currentPos);
        window.draw(text);
    }
}