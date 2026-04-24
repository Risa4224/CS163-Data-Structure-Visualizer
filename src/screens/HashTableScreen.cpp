#include "HashTableScreen.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cmath>

HashTableScreen::HashTableScreen(const sf::Font &font)
    : m_font(font),
      m_topInfoText(font, "Hash Table (Chaining)", 24),
      m_messageText(font, "Ready.", 20),
      m_inputText(font, "", 24),
      m_speedText(font, "1.0x", 18),
      m_createButton({190.f, 46.f}, {56.f, 300.f}, font, "Create", 22),
      m_searchButton({190.f, 46.f}, {56.f, 352.f}, font, "Search(v)", 22),
      m_insertButton({190.f, 46.f}, {56.f, 404.f}, font, "Insert(v)", 22),
      m_removeButton({190.f, 46.f}, {56.f, 456.f}, font, "Remove(v)", 22),
      m_backButton({190.f, 46.f}, {56.f, 508.f}, font, "Back", 22),
      m_prevButton({60.f, 40.f}, {450.f, 630.f}, font, "<<", 20),
      m_playPauseButton({80.f, 40.f}, {520.f, 630.f}, font, "Play", 20),
      m_nextButton({60.f, 40.f}, {610.f, 630.f}, font, ">>", 20)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    m_background.setSize({1280.f, 720.f});
    m_background.setFillColor(sf::Color(240, 240, 240));
    m_leftBar.setSize({250.f, 720.f});
    m_leftBar.setFillColor(sf::Color(45, 52, 54));

    m_inputBox.setSize({190.f, 40.f});
    m_inputBox.setPosition({56.f, 180.f});
    m_inputBox.setFillColor(sf::Color::White);

    m_inputText.setFillColor(sf::Color::Black);
    m_inputText.setPosition({65.f, 185.f});
    m_topInfoText.setPosition({300.f, 20.f});
    m_topInfoText.setFillColor(sf::Color::Black);

    m_sliderBar.setSize({150.f, 6.f});
    m_sliderBar.setPosition({800.f, 650.f});
    m_sliderBar.setFillColor(sf::Color(160, 160, 160));

    m_sliderKnob.setRadius(10.f);
    m_sliderKnob.setFillColor(sf::Color(52, 152, 219));
    m_sliderKnob.setOrigin({10.f, 10.f});
    m_sliderKnob.setPosition({837.5f, 653.f});
    m_speedText.setPosition({960.f, 640.f});
    m_speedText.setFillColor(sf::Color::Black);

    createTable();
}

int HashTableScreen::hashFunction(int value) const
{
    int res = value % FIXED_N;
    return (res < 0) ? res + FIXED_N : res;
}

void HashTableScreen::createTable()
{
    m_table.assign(FIXED_N, std::vector<int>());
    m_steps.clear();
    m_currentStep = 0;
    m_isPlaying = false;
    m_elementCount = 0;
    m_input.clear();
    for (int i = 0; i < 5; ++i)
    {
        int val = std::rand() % 100;
        m_table[hashFunction(val)].push_back(val);
        m_elementCount++;
    }
}

void HashTableScreen::addStep(const std::string &msg, int line, int bucket, int node)
{
    AnimationStep s;
    s.message = msg;
    s.highlightLine = line;
    s.tableSnapshot = m_table;
    s.activeBucket = bucket;
    s.activeNode = node;
    m_steps.push_back(s);
}

void HashTableScreen::setupPseudocode(const std::string &action)
{
    m_pseudocode.clear();
    if (action == "Search")
    {
        m_pseudocode = {"idx = v % 10;", "bucket = table[idx];", "for each node in bucket:", "  if (node == v) return FOUND;", "return NOT_FOUND;"};
    }
    else if (action == "Insert")
    {
        m_pseudocode = {"idx = v % 10;", "if (exists(v)) return;", "table[idx].push_back(v);", "elementCount++;"};
    }
    else if (action == "Remove")
    {
        m_pseudocode = {"idx = v % 10;", "if (!exists(v)) return;", "table[idx].erase(v);", "elementCount--;"};
    }
}

void HashTableScreen::prepareSearch(int value)
{
    m_steps.clear();
    m_currentStep = 0;
    setupPseudocode("Search");
    int idx = hashFunction(value);
    addStep("Hashing: " + std::to_string(value) + " % 10 = " + std::to_string(idx), 0, idx);
    for (int i = 0; i < (int)m_table[idx].size(); ++i)
    {
        addStep("Checking node value: " + std::to_string(m_table[idx][i]), 2, idx, i);
        if (m_table[idx][i] == value)
        {
            addStep("Match found!", 3, idx, i);
            return;
        }
    }
    addStep("Value not found.", 4, idx);
}

void HashTableScreen::prepareInsert(int value)
{
    m_steps.clear();
    m_currentStep = 0;
    setupPseudocode("Insert");
    int idx = hashFunction(value);
    addStep("Hashing " + std::to_string(value), 0, idx);
    for (int i = 0; i < (int)m_table[idx].size(); ++i)
    {
        if (m_table[idx][i] == value)
        {
            addStep("Value already exists. Skip.", 1, idx, i);
            return;
        }
    }
    m_table[idx].push_back(value);
    addStep("Inserting " + std::to_string(value), 2, idx, (int)m_table[idx].size() - 1);
    m_elementCount++;
}

void HashTableScreen::prepareRemove(int value)
{
    m_steps.clear();
    m_currentStep = 0;
    setupPseudocode("Remove");
    int idx = hashFunction(value);
    addStep("Hashing " + std::to_string(value), 0, idx);
    auto &chain = m_table[idx];
    for (int i = 0; i < (int)chain.size(); ++i)
    {
        if (chain[i] == value)
        {
            addStep("Found node. Removing...", 2, idx, i);
            chain.erase(chain.begin() + i);
            addStep("Removed successfully.", 3, idx);
            m_elementCount--;
            return;
        }
    }
    addStep("Value not found.", 1, idx);
}

void HashTableScreen::handleEvent(const sf::Event &event, sf::RenderWindow &window, bool &goBack)
{
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // Update hover states cho buttons
    m_createButton.update(mousePos);
    m_searchButton.update(mousePos);
    m_insertButton.update(mousePos);
    m_removeButton.update(mousePos);
    m_backButton.update(mousePos);
    m_prevButton.update(mousePos);
    m_playPauseButton.update(mousePos);
    m_nextButton.update(mousePos);

    // 1. Xử lý nhập liệu văn bản
    if (const auto *textEvent = event.getIf<sf::Event::TextEntered>())
    {
        if (textEvent->unicode >= '0' && textEvent->unicode <= '9')
            m_input += static_cast<char>(textEvent->unicode);
        else if (textEvent->unicode == 8 && !m_input.empty())
            m_input.pop_back();
    }

    // 2. Xử lý click chuột
    if (const auto *mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button != sf::Mouse::Button::Left)
            return;

        if (m_backButton.contains(mousePos))
            goBack = true;
        if (m_createButton.contains(mousePos))
            createTable();

        // Xử lý các lệnh Search/Insert/Remove
        int val;
        bool ok = false;
        try
        {
            if (!m_input.empty())
            {
                val = std::stoi(m_input);
                ok = true;
            }
        }
        catch (...)
        {
            ok = false;
        }

        if (ok)
        {
            if (m_searchButton.contains(mousePos))
                prepareSearch(val);
            if (m_insertButton.contains(mousePos))
                prepareInsert(val);
            if (m_removeButton.contains(mousePos))
                prepareRemove(val);

            if (!m_steps.empty())
            {
                m_input.clear();
                m_currentStep = 0; // Reset về bước đầu
                m_isPlaying = true;
                m_playPauseButton.setText("Pause");
                m_autoTimer.restart();
            }
        }

        // ĐIỀU CHỈNH: Nút Play/Pause
        if (m_playPauseButton.contains(mousePos) && !m_steps.empty())
        {
            m_isPlaying = !m_isPlaying;
            m_playPauseButton.setText(m_isPlaying ? "Pause" : "Play");
            if (m_isPlaying)
                m_autoTimer.restart(); // Restart timer khi tiếp tục
        }

        // Nút Next/Prev
        if (m_nextButton.contains(mousePos) && m_currentStep < (int)m_steps.size() - 1)
        {
            m_currentStep++;
            m_isPlaying = false;
            m_playPauseButton.setText("Play");
        }
        if (m_prevButton.contains(mousePos) && m_currentStep > 0)
        {
            m_currentStep--;
            m_isPlaying = false;
            m_playPauseButton.setText("Play");
        }
    }

    // 3. XỬ LÝ THANH TRƯỢT (SPEED SLIDER)
    // Cho phép kéo thả mượt mà bằng cách kiểm tra chuột trái đang đè
    if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
    {
        if (m_sliderBar.getGlobalBounds().contains(mousePos) || m_sliderKnob.getGlobalBounds().contains(mousePos))
        {
            float startX = m_sliderBar.getPosition().x;
            float width = m_sliderBar.getSize().x;
            float relX = std::clamp(mousePos.x - startX, 0.f, width);
            float ratio = relX / width;

            m_sliderKnob.setPosition({startX + relX, m_sliderKnob.getPosition().y});

            // Speed từ 0.5x đến 4.0x
            m_speedScale = 0.5f + ratio * 3.5f;

            // Format text hiển thị (vd: 1.2x)
            std::string speedStr = std::to_string(m_speedScale);
            m_speedText.setString(speedStr.substr(0, speedStr.find('.') + 2) + "x");
        }
    }
}

void HashTableScreen::update(const sf::RenderWindow &window)
{
    m_inputText.setString(m_input);
    if (m_isPlaying && !m_steps.empty())
    {
        if (m_autoTimer.getElapsedTime().asSeconds() >= (1.0f / m_speedScale))
        {
            if (m_currentStep < (int)m_steps.size() - 1)
            {
                m_currentStep++;
                m_autoTimer.restart();
            }
            else
            {
                m_isPlaying = false;
                m_playPauseButton.setText("Play"); // Cập nhật text nút khi kết thúc
            }
        }
    }
}

void HashTableScreen::draw(sf::RenderWindow &window) const
{
    window.draw(m_background);
    window.draw(m_leftBar);
    window.draw(m_inputBox);
    window.draw(m_inputText);
    window.draw(m_topInfoText);
    m_createButton.draw(window);
    m_searchButton.draw(window);
    m_insertButton.draw(window);
    m_removeButton.draw(window);
    m_backButton.draw(window);

    const auto &snap = m_steps.empty() ? m_table : m_steps[m_currentStep].tableSnapshot;
    int actB = m_steps.empty() ? -1 : m_steps[m_currentStep].activeBucket;
    int actN = m_steps.empty() ? -1 : m_steps[m_currentStep].activeNode;

    float startX = 300.f, topY = 120.f, bucketSpace = 85.f, nodeH = 38.f, nodeW = 56.f, gap = 22.f;

    for (int i = 0; i < FIXED_N; ++i)
    {
        float x = startX + i * bucketSpace;
        sf::RectangleShape head({nodeW, nodeH});
        head.setPosition({x, topY});
        head.setOutlineThickness(2.f);
        head.setOutlineColor(sf::Color(60, 60, 60));
        head.setFillColor(actB == i ? sf::Color::Yellow : sf::Color::White);
        window.draw(head);

        for (int j = 0; j < (int)snap[i].size(); ++j)
        {
            float nodeY = topY + nodeH + gap + j * (nodeH + gap);
            sf::Vertex line[2];
            line[0].position = {x + nodeW / 2.f, (j == 0 ? topY + nodeH : nodeY - gap)};
            line[0].color = sf::Color(60, 60, 60);
            line[1].position = {x + nodeW / 2.f, nodeY};
            line[1].color = sf::Color(60, 60, 60);
            window.draw(line, 2, sf::PrimitiveType::Lines);

            sf::RectangleShape box({nodeW, nodeH});
            box.setPosition({x, nodeY});
            box.setOutlineThickness(2.f);
            box.setOutlineColor(sf::Color(60, 60, 60));
            box.setFillColor((actB == i && actN == j) ? sf::Color::Cyan : sf::Color(245, 245, 245));
            window.draw(box);

            sf::Text val(m_font, std::to_string(snap[i][j]), 18);
            val.setFillColor(sf::Color::Black);
            sf::FloatRect b = val.getLocalBounds();
            val.setOrigin({b.position.x + b.size.x / 2.f, b.position.y + b.size.y / 2.f});
            val.setPosition({x + nodeW / 2.f, nodeY + nodeH / 2.f});
            window.draw(val);
        }
    }

    if (!m_steps.empty())
    {
        m_prevButton.draw(window);
        m_playPauseButton.draw(window);
        m_nextButton.draw(window);
        window.draw(m_sliderBar);
        window.draw(m_sliderKnob);
        window.draw(m_speedText);
        sf::Text msg(m_font, m_steps[m_currentStep].message, 20);
        msg.setPosition({300.f, 680.f});
        msg.setFillColor(sf::Color(45, 52, 54));
        window.draw(msg);

        // --- PHẦN VẼ KHUNG PSEUDOCODE ĐÃ ĐIỀU CHỈNH ---
        float pcX = 950.f;      // Giữ nguyên hoặc xê dịch nhẹ sang trái nếu cần
        float pcY = 400.f;      // ĐẨY XUỐNG DƯỚI (thay vì 200.f) để không che bảng
        float pcWidth = 300.f;  // Tăng nhẹ chiều rộng để chữ không bị sát lề
        float pcHeight = 220.f; // Giảm chiều cao cho vừa vặn với số dòng code

        // 1. Vẽ nền cho khung Pseudocode
        sf::RectangleShape pcBox({pcWidth, pcHeight});
        pcBox.setPosition({pcX, pcY});
        pcBox.setFillColor(sf::Color(255, 255, 255, 230)); // Tăng độ đậm nền một chút
        pcBox.setOutlineThickness(2.f);
        pcBox.setOutlineColor(sf::Color(45, 52, 54));
        window.draw(pcBox);

        // 2. Vẽ dải màu tiêu đề
        sf::RectangleShape pcHeader({pcWidth, 30.f});
        pcHeader.setPosition({pcX, pcY});
        pcHeader.setFillColor(sf::Color(45, 52, 54));
        window.draw(pcHeader);

        // 3. Vẽ chữ "Pseudocode" tiêu đề
        sf::Text pcTitle(m_font, "Pseudocode", 18);
        pcTitle.setPosition({pcX + 10.f, pcY + 3.f});
        pcTitle.setFillColor(sf::Color::White);
        window.draw(pcTitle);

        // 4. Vẽ các dòng code
        for (size_t i = 0; i < m_pseudocode.size(); ++i)
        {
            sf::Text code(m_font, m_pseudocode[i], 16); // Giảm size chữ xuống 16 cho gọn
            code.setPosition({pcX + 15.f, pcY + 45.f + i * 25.f});

            if (m_steps[m_currentStep].highlightLine == (int)i)
            {
                code.setFillColor(sf::Color::Red);
                code.setStyle(sf::Text::Bold);
            }
            else
            {
                code.setFillColor(sf::Color::Black);
            }
            window.draw(code);
        }
    }
}