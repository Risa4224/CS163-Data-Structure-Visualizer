#include "GraphScreen.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

GraphScreen::GraphScreen(const sf::Font& font)
    : m_font(font),
      m_topInfoText(font, "Minimum Spanning Tree (Kruskal)", 24),
      m_messageText(font, "Ready. Click 'Create Random' to start.", 20),
      m_createButton({190.f, 46.f}, {56.f, 300.f}, font, "Create Random", 20),
      m_runButton({190.f, 46.f}, {56.f, 352.f}, font, "Run Kruskal", 22),
      m_prevButton({190.f, 46.f}, {56.f, 404.f}, font, "Step Prev", 22),
      m_nextButton({190.f, 46.f}, {56.f, 456.f}, font, "Step Next", 22),
      m_backButton({190.f, 46.f}, {56.f, 508.f}, font, "Back", 22),
      m_currentStateIndex(0),
      m_hasResult(false)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    m_background.setSize({1280.f, 720.f});
    m_background.setFillColor(sf::Color(212, 212, 212));

    m_leftBar.setSize({52.f, 720.f});
    m_leftBar.setFillColor(sf::Color(223, 84, 58));

    m_menuPanel.setSize({210.f, 270.f}); // Chứa 5 nút
    m_menuPanel.setPosition({52.f, 290.f});
    m_menuPanel.setFillColor(sf::Color(223, 84, 58));

    m_topInfoText.setFillColor(sf::Color::Black);
    centerTextX(m_topInfoText, 700.f, 24.f);

    m_messageText.setFillColor(sf::Color(120, 20, 20));
    centerTextX(m_messageText, 700.f, 690.f);

    auto styleMenuButton = [](Button& b) {
        b.setNormalColor(sf::Color(223, 84, 58));
        b.setHoverColor(sf::Color(235, 108, 86));
        b.setSelectedColor(sf::Color(190, 65, 45));
    };

    styleMenuButton(m_createButton);
    styleMenuButton(m_runButton);
    styleMenuButton(m_prevButton);
    styleMenuButton(m_nextButton);
    styleMenuButton(m_backButton);
}

void GraphScreen::centerTextX(sf::Text& text, float x, float y) {
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({bounds.position.x + bounds.size.x / 2.f, bounds.position.y + bounds.size.y / 2.f});
    text.setPosition({x, y});
}

void GraphScreen::createRandomGraph() {
    m_graph.clear();
    m_states.clear();
    m_hasResult = false;
    m_currentStateIndex = 0;

    int numNodes = 6;
    float centerX = 750.f;
    float centerY = 360.f;
    float radius = 200.f;

    // Tạo các đỉnh xếp thành vòng tròn để dễ nhìn
    for (int i = 0; i < numNodes; ++i) {
        float angle = i * (2 * 3.14159f / numNodes);
        float x = centerX + std::cos(angle) * radius;
        float y = centerY + std::sin(angle) * radius;
        m_graph.addNode(i, x, y);
    }

    // Tạo các cạnh ngẫu nhiên
    for (int i = 0; i < numNodes; ++i) {
        for (int j = i + 1; j < numNodes; ++j) {
            if (std::rand() % 100 < 60) { // 60% tỉ lệ có cạnh nối
                int weight = (std::rand() % 90) + 10; // Trọng số từ 10 - 99
                m_graph.addEdge(i, j, weight);
            }
        }
    }
    m_messageText.setString("Generated random graph.");
}

void GraphScreen::runKruskal() {
    if (m_graph.nodes.empty()) return;
    
    m_states = m_graph.getKruskalMSTStates(); // Lấy mảng trạng thái từ Bước 1
    m_currentStateIndex = 0;
    m_hasResult = true;
    m_messageText.setString(m_states[m_currentStateIndex].message);
}

void GraphScreen::nextStep() {
    if (m_hasResult && m_currentStateIndex < (int)m_states.size() - 1) {
        m_currentStateIndex++;
        m_messageText.setString(m_states[m_currentStateIndex].message);
    }
}

void GraphScreen::prevStep() {
    if (m_hasResult && m_currentStateIndex > 0) {
        m_currentStateIndex--;
        m_messageText.setString(m_states[m_currentStateIndex].message);
    }
}

void GraphScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window, bool& goBack) {
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (event.is<sf::Event::MouseMoved>()) {
        m_createButton.update(mousePos);
        m_runButton.update(mousePos);
        m_prevButton.update(mousePos);
        m_nextButton.update(mousePos);
        m_backButton.update(mousePos);
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button != sf::Mouse::Button::Left) return;

        if (m_backButton.contains(mousePos)) { goBack = true; return; }
        if (m_createButton.contains(mousePos)) { createRandomGraph(); return; }
        if (m_runButton.contains(mousePos)) { runKruskal(); return; }
        if (m_nextButton.contains(mousePos)) { nextStep(); return; }
        if (m_prevButton.contains(mousePos)) { prevStep(); return; }
    }
}

void GraphScreen::update(const sf::RenderWindow& window) {
    centerTextX(m_messageText, 700.f, 690.f);
}

void GraphScreen::draw(sf::RenderWindow& window) const {
    window.draw(m_background);
    window.draw(m_leftBar);
    window.draw(m_menuPanel);

    sf::Text arrow(m_font, "<", 34);
    arrow.setFillColor(sf::Color::White);
    arrow.setPosition({18.f, 500.f}); // Điều chỉnh lại Y để khớp với nút Back
    window.draw(arrow);

    window.draw(m_topInfoText);
    window.draw(m_messageText);

    m_createButton.draw(window);
    m_runButton.draw(window);
    m_prevButton.draw(window);
    m_nextButton.draw(window);
    m_backButton.draw(window);

    // --- LOGIC VẼ ĐỒ THỊ ---
    if (m_graph.nodes.empty()) return;

    // 1. Vẽ tất cả các cạnh (Edges)
    for (const auto& edge : m_graph.edges) {
        Node u = m_graph.nodes[edge.u];
        Node v = m_graph.nodes[edge.v];

        sf::Color edgeColor = sf::Color(150, 150, 150); // Xám nhạt mặc định
        float thickness = 2.f;

        // Nếu đang chạy thuật toán, tô màu lại cạnh
        if (m_hasResult) {
            const auto& state = m_states[m_currentStateIndex];
            
            // Nếu cạnh này thuộc MST -> Xanh lá
            for (const auto& mstEdge : state.currentMST) {
                if ((mstEdge.u == edge.u && mstEdge.v == edge.v) || (mstEdge.u == edge.v && mstEdge.v == edge.u)) {
                    edgeColor = sf::Color(46, 204, 113); // Xanh lá
                    thickness = 5.f;
                    break;
                }
            }
            
            // Nếu cạnh này đang được xét -> Vàng/Cam
            if (!state.isFinished && ((state.evaluatingEdge.u == edge.u && state.evaluatingEdge.v == edge.v) || 
                                      (state.evaluatingEdge.u == edge.v && state.evaluatingEdge.v == edge.u))) {
                edgeColor = sf::Color(241, 196, 15); // Vàng cam
                thickness = 5.f;
            }
        }

        // SFML Vẽ Line (VertexArray không hỗ trợ độ dày, nên dùng RectangleShape mảnh)
        sf::RectangleShape line;
        float length = std::sqrt(std::pow(v.x - u.x, 2) + std::pow(v.y - u.y, 2));
        line.setSize({length, thickness});
        line.setFillColor(edgeColor);
        line.setOrigin({0, thickness / 2.f});
        line.setPosition({u.x, u.y});
        line.setRotation(sf::radians(std::atan2(v.y - u.y, v.x - u.x)));
        window.draw(line);

        // Hiển thị Text trọng số ở giữa cạnh
        sf::Text weightText(m_font, std::to_string(edge.weight), 18);
        weightText.setFillColor(sf::Color::Blue);
        weightText.setPosition({(u.x + v.x) / 2.f - 10.f, (u.y + v.y) / 2.f - 15.f});
        window.draw(weightText);
    }

    // 2. Vẽ các đỉnh (Nodes)
    for (const auto& node : m_graph.nodes) {
        sf::CircleShape circle(25.f);
        circle.setOrigin({25.f, 25.f});
        circle.setPosition({node.x, node.y});
        circle.setFillColor(sf::Color(245, 245, 245));
        circle.setOutlineThickness(3.f);
        circle.setOutlineColor(sf::Color(60, 60, 60));
        window.draw(circle);

        sf::Text idText(m_font, std::to_string(node.id), 24);
        idText.setFillColor(sf::Color::Black);
        sf::FloatRect tb = idText.getLocalBounds();
        idText.setOrigin({tb.position.x + tb.size.x / 2.f, tb.position.y + tb.size.y / 2.f});
        idText.setPosition({node.x, node.y});
        window.draw(idText);
    }
}