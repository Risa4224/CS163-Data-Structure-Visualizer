#include "AVLTreeScreen.hpp"
#include <sstream>
#include <set>
#include <cstdint>
#include <ctime>
std::vector<std::string> insertLeftCode = {
    "if (data < node->data)",
    "    node->left = insert(node->left, data);",
    "node->height = 1 + max(h(L), h(R));",
    "return balance(node);"};

std::vector<std::string> rotateLeftCode = {
    "Node* y = x->right;",
    "Node* T2 = y->left;",
    "y->left = x; x->right = T2;",
    "updateHeight(x); updateHeight(y);"};

AVLTreeScreen::AVLTreeScreen(const sf::Font &font)
    : m_font(font),
      m_topInfoText(font, "AVL Tree Visualizer", 28),
      m_messageText(font, "Ready.", 20),
      m_inputLabel(font, "Value", 20),
      m_inputText(font, "", 24),
      m_stepText(font, "Step: 0 / 0", 22),
      m_speedLabel(font, "Auto Speed", 18),
      m_speedSlider(sf::Vector2f(56.f, 580.f), 190.f),
      // --- THÊM NÚT RANDOM VÀO ĐÂY ---
      m_randomButton(sf::Vector2f(190.f, 46.f), sf::Vector2f(56.f, 458.f), font, "Random Ins", 22),
      // ------------------------------
      m_insertButton(sf::Vector2f(190.f, 46.f), sf::Vector2f(56.f, 250.f), font, "Insert(v)", 22),
      m_removeButton(sf::Vector2f(190.f, 46.f), sf::Vector2f(56.f, 302.f), font, "Remove(v)", 22),
      m_searchButton(sf::Vector2f(190.f, 46.f), sf::Vector2f(56.f, 354.f), font, "Search(v)", 22),
      m_clearButton(sf::Vector2f(190.f, 46.f), sf::Vector2f(56.f, 406.f), font, "Clear Tree", 22),
      m_backButton(sf::Vector2f(190.f, 46.f), sf::Vector2f(56.f, 508.f), font, "Back", 22),
      m_prevButton(sf::Vector2f(90.f, 40.f), sf::Vector2f(56.f, 600.f), font, "< Prev", 18),
      m_nextButton(sf::Vector2f(90.f, 40.f), sf::Vector2f(156.f, 600.f), font, "Next >", 18),
      m_autoButton(sf::Vector2f(90.f, 40.f), sf::Vector2f(256.f, 600.f), font, "Auto", 18)

{
    m_background.setSize(sf::Vector2f(1280.f, 720.f));
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

    m_codeLines = {

        "Node* child = node->right; // or left",
        "Node* T2 = child->left; // or right",
        "child->left = node; node->right = T2;",
        "updateHeight(node); updateHeight(child);",

        "if (data < node->data) insert(left);",
        "else if (data > node->data) insert(right);",
        "node->height = 1 + max(h(L), h(R));",
        "return balance(node); ",

        "if (target < node->data) remove(target);",
        "else if (target == node->data) deleteNode();",
        "if (node->isLeaf) node = nullptr;",
        "else findSuccessor() & replaceData();"};

    for (size_t i = 0; i < m_codeLines.size(); ++i)
    {
        sf::Text t(font, m_codeLines[i], 16);
        t.setPosition({950.f, 200.f + (i * 25.f)});
        m_codeTexts.push_back(t);
    }

    m_codeBackground.setSize({320.f, 400.f});
    m_codeBackground.setPosition({940.f, 150.f});
    m_codeBackground.setFillColor(sf::Color(20, 20, 20, 230));
    m_codeBackground.setOutlineThickness(2.f);
    m_codeBackground.setOutlineColor(sf::Color(41, 128, 185));

    m_speedSlider = Slider({56.f, 580.f}, 190.f);

    m_speedLabel.setCharacterSize(18);
    m_speedLabel.setFillColor(sf::Color::Black);
    m_speedLabel.setPosition({56.f, 550.f});

    for (size_t i = 0; i < m_codeLines.size(); ++i)
    {
        m_codeTexts[i].setFont(m_font);
        m_codeTexts[i].setString(m_codeLines[i]);
        m_codeTexts[i].setCharacterSize(14);

        m_codeTexts[i].setPosition({955.f, 170.f + (i * 28.f)});
    }

    m_codeRepo["ROT_L"] = {
        "y = x->right;",
        "T2 = y->left;",
        "y->left = x; x->right = T2;",
        "updateHeight(x); updateHeight(y);"};

    m_codeRepo["ROT_R"] = {
        "x = y->left;",
        "T2 = x->right;",
        "x->right = y; y->left = T2;",
        "updateHeight(y); updateHeight(x);"};

    m_codeRepo["INS_L"] = {"if (data < node->data) node->left = insert(...);"};
    m_codeRepo["INS_R"] = {"if (data > node->data) node->right = insert(...);"};

    m_codeRepo["BALANCE"] = {
        "if (BF > 1 && L_BF >= 0) rotateRight(node);",
        "if (BF > 1 && L_BF < 0) { rotL(child); rotR(node); }",
        "if (BF < -1 && R_BF <= 0) rotateLeft(node);",
        "if (BF < -1 && R_BF > 0) { rotR(child); rotL(node); }"};

    m_codeRepo["SEARCH"] = {
        "while (current != nullptr)",
        "if (target == current->data) return true;",
        "if (target < current->data) current = current->left;",
        "else current = current->right;",
        "return false;"};

    m_codeRepo["DEL"] = {
        "if (target < node->data) node->left = remove(target);",
        "else if (target > node->data) node->right = remove(target);",
        "if (node->isLeaf) { delete node; node = nullptr; }",
        "else { findSuccessor(); replaceData(); removeSuccessor(); }"};

    auto styleMenuButton = [](Button &b)
    {
        b.setNormalColor(sf::Color(41, 128, 185));
        b.setHoverColor(sf::Color(52, 152, 219));
        b.setSelectedColor(sf::Color(31, 97, 141));
    };
    m_randomButton = Button(sf::Vector2f(190.f, 46.f), sf::Vector2f(56.f, 458.f), font, "Random Ins", 22);

    styleMenuButton(m_insertButton);
    styleMenuButton(m_removeButton);
    styleMenuButton(m_searchButton);
    styleMenuButton(m_clearButton);
    styleMenuButton(m_backButton);
    styleMenuButton(m_prevButton);
    styleMenuButton(m_nextButton);
    styleMenuButton(m_autoButton);
    styleMenuButton(m_randomButton);
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

    if (auto *mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button == sf::Mouse::Button::Left)

            m_isDraggingSlider = true;
    }
    if (event.getIf<sf::Event::MouseButtonReleased>())
    {
        m_isDraggingSlider = false;
    }

    m_speedSlider.update(mousePos, m_isDraggingSlider);

    m_autoDelay = 1.5f - (m_speedSlider.value * 1.4f);

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
        if (m_randomButton.contains(mousePos))
        {
            m_tree.clear();
            m_currentStep = 0;
            m_lastStep = 0;
            m_animProgress = 1.0f;
            m_isAutoPlaying = false;
            m_autoButton.setText("Auto");

            static bool seeded = false;
            if (!seeded)
            {
                srand(static_cast<unsigned int>(time(0)));
                seeded = true;
            }

            int numNodes = rand() % 4 + 4;
            std::set<int> uniqueVals;

            while (uniqueVals.size() < (size_t)numNodes)
            {
                uniqueVals.insert(rand() % 99 + 1);
            }

            for (int val : uniqueVals)
            {
                m_tree.insert(val);
            }

            m_currentStep = 0;
            m_lastStep = 0;
            m_animProgress = 0.0f;
            m_message = "Tree reset and randomized with " + std::to_string(numNodes) + " nodes.";
            return;
        }
    }
}

void AVLTreeScreen::update(const sf::RenderWindow &window)
{
    (void)window;
    m_inputText.setString(m_input.empty() ? "..." : m_input);

    float dt = m_clock.restart().asSeconds();
    if (m_lastStep != m_currentStep)
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
        if (m_pauseTimer >= m_autoDelay)
        {
            if (m_currentStep < (int)m_tree.animationStates.size() - 1)
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

    if (!m_tree.animationStates.empty() && m_currentStep < (int)m_tree.animationStates.size())
    {
        m_message = m_tree.animationStates[m_currentStep].action;
        m_stepText.setString("Step: " + std::to_string(m_currentStep + 1) + " / " + std::to_string(m_tree.animationStates.size()));
    }

    m_messageText.setString(m_message);

    sf::FloatRect statusBounds = m_messageText.getLocalBounds();
    float pX = 60.f;

    m_statusBox.setSize(sf::Vector2f(statusBounds.size.x + pX, 50.f));

    m_statusBox.setOrigin(sf::Vector2f(m_statusBox.getSize().x / 2.f, m_statusBox.getSize().y / 2.f));
    m_statusBox.setPosition(sf::Vector2f(700.f, 670.f));

    m_messageText.setOrigin(sf::Vector2f(statusBounds.position.x + statusBounds.size.x / 2.f, statusBounds.position.y + statusBounds.size.y / 2.f));
    m_messageText.setPosition(sf::Vector2f(700.f, 670.f));
}

void AVLTreeScreen::renderSingleCodeLine(sf::RenderWindow &window, const std::string &code) const
{
    sf::Text text(m_font, code, 20);
    text.setFillColor(sf::Color::Yellow);
    text.setStyle(sf::Text::Bold);

    sf::FloatRect textBounds = text.getLocalBounds();

    sf::RectangleShape bg;
    float paddingX = 40.f;
    float paddingY = 20.f;

    bg.setSize(sf::Vector2f(textBounds.size.x + paddingX, textBounds.size.y + paddingY));

    sf::Vector2f boxPos(800.f, 100.f);
    bg.setPosition(boxPos);

    bg.setFillColor(sf::Color(20, 20, 20, 230));
    bg.setOutlineThickness(2.f);
    bg.setOutlineColor(sf::Color(41, 128, 185));

    text.setOrigin(sf::Vector2f(textBounds.position.x + textBounds.size.x / 2.0f, textBounds.position.y + textBounds.size.y / 2.0f));
    text.setPosition(sf::Vector2f(boxPos.x + bg.getSize().x / 2.0f, boxPos.y + bg.getSize().y / 2.0f));

    window.draw(bg);
    window.draw(text);
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
    m_randomButton.draw(window);

    window.draw(m_statusBox);
    window.draw(m_messageText);

    if (m_tree.animationStates.empty())
        return;

    const TreeState &targetState = m_tree.animationStates[m_currentStep];
    const TreeState &lastState = m_tree.animationStates[m_lastStep];
    float radius = 25.f;

    if (m_tree.animationStates.empty())
        return;

    auto getLerpedPos = [&](int nodeData) -> sf::Vector2f
    {
        sf::Vector2f oldPos(-100.f, -100.f), newPos(-100.f, -100.f);
        bool inOld = false, inNew = false;

        for (const auto &n : lastState.captures)
        {
            if (n.data == nodeData)
            {
                oldPos = {(float)n.x, (float)n.y};
                inOld = true;
                break;
            }
        }
        for (const auto &n : targetState.captures)
        {
            if (n.data == nodeData)
            {
                newPos = {(float)n.x, (float)n.y};
                inNew = true;
                break;
            }
        }

        if (inOld && !inNew)
            return oldPos;
        if (!inOld && inNew)
            return {newPos.x, newPos.y - 50.f + (50.f * m_animProgress)};

        return {oldPos.x + (newPos.x - oldPos.x) * m_animProgress,
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

    std::set<int> allNodeIds; // vẽ node
    for (const auto &n : lastState.captures)
        allNodeIds.insert(n.data);
    for (const auto &n : targetState.captures)
        allNodeIds.insert(n.data);

    for (int nodeId : allNodeIds)
    {
        sf::Vector2f currentPos = getLerpedPos(nodeId);

        bool existsInTarget = false;
        for (const auto &n : targetState.captures)
        {
            if (n.data == nodeId)
            {
                existsInTarget = true;
                break;
            }
        }

        uint8_t alpha = existsInTarget ? 255 : static_cast<uint8_t>(255 * (1.0f - m_animProgress));

        sf::CircleShape circle(radius);
        circle.setOutlineThickness(3.f);
        circle.setPosition({currentPos.x - radius, currentPos.y - radius});

        if (existsInTarget && nodeId == targetState.highlightData)
        {
            circle.setFillColor(sf::Color(231, 76, 60, alpha));
            circle.setOutlineColor(sf::Color(241, 196, 15, alpha));
        }
        else
        {
            circle.setFillColor(sf::Color(46, 204, 113, alpha));
            circle.setOutlineColor(sf::Color(255, 255, 255, alpha));
        }
        window.draw(circle);

        // KHAI BÁO BIẾN LÀ nodeText
        sf::Text nodeText(m_font, std::to_string(nodeId), 20);
        nodeText.setFillColor(sf::Color(255, 255, 255, alpha)); // SỬ DỤNG nodeText

        sf::FloatRect bounds = nodeText.getLocalBounds();
        nodeText.setOrigin({bounds.position.x + bounds.size.x / 2.0f, bounds.position.y + bounds.size.y / 2.0f});
        nodeText.setPosition(currentPos);

        window.draw(nodeText);
    }

    if (m_codeRepo.count(targetState.currentCodeType) > 0 && targetState.lineID >= 0)
    {
        const auto &lines = m_codeRepo.at(targetState.currentCodeType);
        if (targetState.lineID < (int)lines.size())
        {
            renderSingleCodeLine(window, lines[targetState.lineID]);
        }
    }

    m_speedSlider.draw(window);
    window.draw(m_speedLabel);
}