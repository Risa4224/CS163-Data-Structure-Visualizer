#include "GraphScreen.hpp"
#include <sstream>
#include <random>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>

GraphScreen::GraphScreen(const sf::Font& font)
    : m_font(font),
      m_topInfoText(font, "Minimum Spanning Tree (Kruskal)", 24),
      m_messageText(font, "Ready. Click 'Create Random' to start.", 20),
      m_inputLabel(font, "V & E (eg: 6 8)", 18),
      m_inputText(font, "", 24),
      m_createRandomButton({190.f, 46.f}, {56.f, 248.f}, font, "Create Random", 20),
      m_createButton({190.f, 46.f}, {56.f, 300.f}, font, "Create (V, E)", 20),
      m_runButton({190.f, 46.f}, {56.f, 352.f}, font, "Run Kruskal", 22),
      m_autoRunButton({190.f, 46.f}, {56.f, 404.f}, font, "Auto / Pause", 22),
      m_prevButton({190.f, 46.f}, {56.f, 456.f}, font, "Step Prev", 22),
      m_nextButton({190.f, 46.f}, {56.f, 508.f}, font, "Step Next", 22),
      m_backButton({190.f, 46.f}, {56.f, 560.f}, font, "Back", 22),
      m_isInputFocused(false),
      m_currentStateIndex(0),
      m_hasResult(false),
      m_isAutoRunning(false),
      m_stepDelay(1.0f),
      m_speedLabel(font, "Speed: 1.0x", 18), 
      m_isDraggingSlider(false)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // --- SETUP MÀU SẮC VS CODE DARK THEME ---
    sf::Color kw(197, 134, 192);   // Màu hồng (Keyword)
    sf::Color fn(220, 220, 170);   // Màu vàng (Function)
    sf::Color vr(156, 220, 254);   // Màu xanh dương nhạt (Variable)
    sf::Color punc(212, 212, 212); // Màu xám sáng (Punctuation)
    sf::Color ty(78, 201, 176);    // Màu xanh ngọc (Type)

    // Dùng explicit CodeToken() để tránh lỗi initializer list của MSVC
    m_kruskalCode = {
        // Dòng 0: std::sort(edges.begin(), edges.end());
        { CodeToken("std", ty), CodeToken("::", punc), CodeToken("sort", fn), CodeToken("(", punc), CodeToken("edges", vr), CodeToken(".", punc), CodeToken("begin", fn), CodeToken("(), ", punc), CodeToken("edges", vr), CodeToken(".", punc), CodeToken("end", fn), CodeToken("());", punc) },
        
        // Dòng 1: for (const auto& edge : edges) {
        { CodeToken("for", kw), CodeToken(" (", punc), CodeToken("const auto", kw), CodeToken("& ", punc), CodeToken("edge", vr), CodeToken(" : ", punc), CodeToken("edges", vr), CodeToken(") {", punc) },
        
        // Dòng 2:     if (findSet(edge.u) != findSet(edge.v)) {
        { CodeToken("    if", kw), CodeToken(" (", punc), CodeToken("findSet", fn), CodeToken("(", punc), CodeToken("edge", vr), CodeToken(".", punc), CodeToken("u", vr), CodeToken(") != ", punc), CodeToken("findSet", fn), CodeToken("(", punc), CodeToken("edge", vr), CodeToken(".", punc), CodeToken("v", vr), CodeToken(")) {", punc) },
        
        // Dòng 3:         mst.push_back(edge);
        { CodeToken("        mst", vr), CodeToken(".", punc), CodeToken("push_back", fn), CodeToken("(", punc), CodeToken("edge", vr), CodeToken(");", punc) },
        
        // Dòng 4:         unionSets(edge.u, edge.v);
        { CodeToken("        unionSets", fn), CodeToken("(", punc), CodeToken("edge", vr), CodeToken(".", punc), CodeToken("u", vr), CodeToken(", ", punc), CodeToken("edge", vr), CodeToken(".", punc), CodeToken("v", vr), CodeToken(");", punc) },
        
        // Dòng 5:     }
        { CodeToken("    }", punc) },
        
        // Dòng 6: }
        { CodeToken("}", punc) }
    };

    m_background.setSize({1280.f, 720.f});
    m_background.setFillColor(sf::Color(212, 212, 212));

    m_leftBar.setSize({52.f, 720.f});
    m_leftBar.setFillColor(sf::Color(223, 84, 58));

    m_menuPanel.setSize({210.f, 374.f}); 
    m_menuPanel.setPosition({52.f, 238.f});
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

    // --- SETUP GIAO DIỆN THANH TRƯỢT TỐC ĐỘ ---
    m_speedLabel.setFillColor(sf::Color::White);
    m_speedLabel.setStyle(sf::Text::Bold);    // 
    m_speedLabel.setPosition({60.f, 610.f});  // 

    m_sliderTrack.setSize({160.f, 8.f});
    m_sliderTrack.setPosition({70.f, 645.f});
    m_sliderTrack.setFillColor(sf::Color(150, 150, 150));
    m_sliderTrack.setOutlineThickness(1.f);
    m_sliderTrack.setOutlineColor(sf::Color::Black);

    m_sliderKnob.setSize({14.f, 24.f});
    m_sliderKnob.setOrigin({7.f, 12.f}); // Đặt tâm vào giữa cục núm
    m_sliderKnob.setPosition({150.f, 649.f}); // Bắt đầu ở giữa thanh trượt (1.0x)
    m_sliderKnob.setFillColor(sf::Color(255, 255, 255)); 
    m_sliderKnob.setOutlineThickness(2.f);
    m_sliderKnob.setOutlineColor(sf::Color(80, 80, 80));

    centerTextX(m_messageText, 700.f, 690.f);

    auto styleMenuButton = [](Button& b) {
        b.setNormalColor(sf::Color(223, 84, 58));
        b.setHoverColor(sf::Color(235, 108, 86));
        b.setSelectedColor(sf::Color(190, 65, 45));
    };

    styleMenuButton(m_createRandomButton);
    styleMenuButton(m_createButton);
    styleMenuButton(m_runButton);
    styleMenuButton(m_autoRunButton);
    styleMenuButton(m_prevButton);
    styleMenuButton(m_nextButton);
    styleMenuButton(m_backButton);
}

bool GraphScreen::parseInput(int& v, int& e) const {
    if (m_input.empty()) return false;
    std::stringstream ss(m_input);
    if (ss >> v >> e) return true;
    return false;
}

void GraphScreen::createCustomGraph(int v, int e) {
    m_graph.clear();
    m_states.clear();
    m_hasResult = false;
    m_currentStateIndex = 0;
    m_isAutoRunning = false;

    // 1. Giới hạn V và E để không bị crash hoặc vẽ tràn màn hình
    if (v < 2) v = 2;
    if (v > 12) v = 12; // Tối đa 12 đỉnh để vẽ vòng tròn cho đẹp
    
    int maxEdges = v * (v - 1) / 2; // Số cạnh tối đa của đơn đồ thị vô hướng
    if (e > maxEdges) e = maxEdges;
    if (e < 0) e = 0;

    // 2. Tạo đỉnh
    float centerX = 550.f;
    float centerY = 360.f;
    float radius = 220.f;
    for (int i = 0; i < v; ++i) {
        float angle = i * (2 * 3.14159f / v);
        float x = centerX + std::cos(angle) * radius;
        float y = centerY + std::sin(angle) * radius;
        m_graph.addNode(i, x, y);
    }

    // 3. Tạo danh sách tất cả các cạnh có thể có
    std::vector<std::pair<int, int>> possibleEdges;
    for (int i = 0; i < v; ++i) {
        for (int j = i + 1; j < v; ++j) {
            possibleEdges.push_back({i, j});
        }
    }

    // 4. Xáo trộn ngẫu nhiên (Shuffle)
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(possibleEdges.begin(), possibleEdges.end(), g);

    // 5. Lấy đúng e cạnh
    for (int i = 0; i < e; ++i) {
        int u = possibleEdges[i].first;
        int dest = possibleEdges[i].second;
        int weight = (std::rand() % 90) + 10;
        m_graph.addEdge(u, dest, weight);
    }

    m_messageText.setString("Created graph: " + std::to_string(v) + " vertices, " + std::to_string(e) + " edges.");
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
    m_isAutoRunning = false;

    int numNodes = (std::rand() % 7) + 4;
    float centerX = 550.f;
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
            if (std::rand() % 100 < 35) { // 60% tỉ lệ có cạnh nối
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

    // 1. CHỈ CẬP NHẬT MÀU SẮC KHI DI CHUỘT (KHÔNG CHẠY LOGIC Ở ĐÂY)
    if (event.is<sf::Event::MouseMoved>()) {
        m_createRandomButton.update(mousePos);
        m_createButton.update(mousePos);
        m_runButton.update(mousePos);
        m_autoRunButton.update(mousePos);
        m_prevButton.update(mousePos);
        m_nextButton.update(mousePos);
        m_backButton.update(mousePos);

        if (m_isDraggingSlider) {
            // Giới hạn x không vượt quá 2 đầu thanh ray (70 đến 230)
            float newX = std::max(77.f, std::min(mousePos.x, 223.f)); 
            m_sliderKnob.setPosition({newX, 649.f});
        }
    }

    // 2. LOGIC GÕ PHÍM
    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>()) {
        if (!m_isInputFocused) return;

        char c = static_cast<char>(textEvent->unicode);
        if ((c >= '0' && c <= '9') || c == ' ') {
            if (m_input.size() < 10) m_input += c;
        } else if (textEvent->unicode == 8 && !m_input.empty()) {
            m_input.pop_back();
        }
    }

    // 3. LOGIC CLICK CHUỘT TRÁI (TẤT CẢ CHỨC NĂNG PHẢI NẰM TRONG NÀY)
    // BẮT SỰ KIỆN THẢ CHUỘT LÀ DỪNG KÉO
    if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>()) {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            m_isDraggingSlider = false;
        }
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>()) {
        if (mousePressed->button != sf::Mouse::Button::Left) return;

        // Focus hộp nhập
        // Xử lý Click vào thanh trượt
        if (m_sliderKnob.getGlobalBounds().contains(mousePos) || m_sliderTrack.getGlobalBounds().contains(mousePos)) {
            m_isDraggingSlider = true;
            float newX = std::max(77.f, std::min(mousePos.x, 223.f));
            m_sliderKnob.setPosition({newX, 649.f});
            return; 
        }
        if (m_inputBox.getGlobalBounds().contains(mousePos)) {
            m_isInputFocused = true;
            m_inputBox.setOutlineColor(sf::Color(46, 204, 113));
        } else {
            m_isInputFocused = false;
            m_inputBox.setOutlineColor(sf::Color(80, 80, 80));
        }

        // Click các nút
        if (m_backButton.contains(mousePos)) { goBack = true; return; }
        
        if (m_createRandomButton.contains(mousePos)) { 
            createRandomGraph(); 
            return; 
        }

        if (m_createButton.contains(mousePos)) { 
            int v = 0, e = 0;
            if (parseInput(v, e)) {
                createCustomGraph(v, e);
                m_input.clear();
            } else {
                m_messageText.setString("Vui long nhap V va E hop le (vd: 6 8)");
            }
            return; 
        }

        if (m_runButton.contains(mousePos)) { runKruskal(); return; }
        
        // LOGIC AUTO RUN NẰM AN TOÀN TRONG SỰ KIỆN CLICK CHUỘT TRÁI
        if (m_autoRunButton.contains(mousePos)) {
            if (m_graph.nodes.empty()) {
                m_messageText.setString("Please create a graph first!");
                return;
            }
            if (!m_hasResult) runKruskal(); 
            if (m_states.empty()) return;

            m_isAutoRunning = !m_isAutoRunning; // Tắt/bật
            
            if (m_isAutoRunning) {
                if (m_currentStateIndex >= (int)m_states.size() - 1) {
                    m_currentStateIndex = 0; // Chạy lại từ đầu nếu đã xong
                }
                m_messageText.setString(m_states[m_currentStateIndex].message);
                m_autoRunClock.restart();
            }
            return;
        }

        if (m_nextButton.contains(mousePos)) { nextStep(); return; }
        if (m_prevButton.contains(mousePos)) { prevStep(); return; }
    }
}

void GraphScreen::update(const sf::RenderWindow& window) {
    centerTextX(m_messageText, 700.f, 690.f);
    m_inputText.setString(m_input.empty() ? "..." : m_input);

    // THÊM ĐOẠN TÍNH TOÁN SPEED NÀY VÀO TRƯỚC LOGIC AUTO RUN:
    float percent = (m_sliderKnob.getPosition().x - 70.f) / 160.f; // Ra số từ 0.0 đến 1.0
    m_stepDelay = 2.0f - (percent * 1.8f); // Kéo trái = 2.0s (chậm), Kéo phải = 0.2s (nhanh)

    std::stringstream ss;
    ss << "Speed: " << std::fixed << std::setprecision(1) << (1.0f / m_stepDelay) << "x";
    m_speedLabel.setString(ss.str());

    // Logic Tự Động Chạy
    if (m_isAutoRunning) {
        // Nếu đồng hồ đo được thời gian trôi qua lớn hơn delay (1 giây)
        if (m_autoRunClock.getElapsedTime().asSeconds() >= m_stepDelay) {
            m_autoRunClock.restart(); // Reset đồng hồ cho bước kế tiếp
            
            if (m_currentStateIndex < (int)m_states.size() - 1) {
                nextStep(); // Tự động gọi hàm đi tới
            } else {
                m_isAutoRunning = false; // Thuật toán kết thúc -> tự động tắt Auto
            }
        }
    }
}

void GraphScreen::draw(sf::RenderWindow& window) const {
    window.draw(m_background);
    window.draw(m_leftBar);
    window.draw(m_menuPanel);

    window.draw(m_inputLabel);
    window.draw(m_inputBox);
    window.draw(m_inputText);
    

    sf::Text arrow(m_font, "<", 34);
    arrow.setFillColor(sf::Color::White);
    arrow.setPosition({18.f, 552.f}); // Điều chỉnh lại Y để khớp với nút Back
    window.draw(arrow);

    window.draw(m_topInfoText);
    window.draw(m_messageText);

    

    m_createRandomButton.draw(window);
    m_createButton.draw(window);
    m_runButton.draw(window);
    m_autoRunButton.draw(window);
    m_prevButton.draw(window);
    m_nextButton.draw(window);
    m_backButton.draw(window);

    // --- LOGIC VẼ ĐỒ THỊ ---
    if (m_graph.nodes.empty()) return;

    // 1. Vẽ tất cả các cạnh (Edges)
    for (const auto& edge : m_graph.edges) {
        GraphNode u = m_graph.nodes[edge.u];
        GraphNode v = m_graph.nodes[edge.v];

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

        // 1. Tính toán vị trí: Lệch 35% chiều dài cạnh thay vì 50% (tránh tâm đường tròn)
        float t = 0.35f; 
        float textX = u.x + (v.x - u.x) * t;
        float textY = u.y + (v.y - u.y) * t;

        sf::Text weightText(m_font, std::to_string(edge.weight), 18);
        weightText.setFillColor(sf::Color::Blue);
        
        // Căn tâm cho text
        sf::FloatRect textBounds = weightText.getLocalBounds();
        weightText.setOrigin({textBounds.position.x + textBounds.size.x / 2.f, 
                              textBounds.position.y + textBounds.size.y / 2.f});
        weightText.setPosition({textX, textY});

        // 2. Tạo một hình chữ nhật nhỏ làm "nền" để lót dưới con số, che đi đường kẻ
        sf::RectangleShape bgRect({textBounds.size.x + 6.f, textBounds.size.y + 6.f});
        bgRect.setOrigin({bgRect.getSize().x / 2.f, bgRect.getSize().y / 2.f});
        bgRect.setPosition({textX, textY});
        bgRect.setFillColor(sf::Color(212, 212, 212)); // Cùng màu với background của app

        // 3. Vẽ nền che trước, vẽ số lên sau
        window.draw(bgRect);
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

        // --- VẼ BẢNG CODE C++ (GIẢ LẬP VS CODE) ---
        sf::RectangleShape codePanel({450.f, 320.f}); // Bảng rộng hơn một chút để chứa code C++
        codePanel.setPosition({820.f, 50.f}); 
        codePanel.setFillColor(sf::Color(30, 30, 30)); // Nền đen nhám VS Code
        codePanel.setOutlineThickness(2.f);
        codePanel.setOutlineColor(sf::Color(60, 60, 60));
        window.draw(codePanel);

        // bắt từ khóa trong thông báo để biết đang chạy tới dòng nào
        int activeLine = -1;
        if (m_hasResult && !m_states.empty()) {
            std::string msg = m_states[m_currentStateIndex].message;
            
            // Đã bổ sung cả chữ hoa lẫn chữ thường để bắt chính xác 100%
            if (msg.find("Sort") != std::string::npos || msg.find("Sap xep") != std::string::npos || msg.find("sap xep") != std::string::npos) {
                activeLine = 0; // Highlight dòng: std::sort...
            } 
            else if (msg.find("Xet") != std::string::npos || msg.find("xet") != std::string::npos || msg.find("xét") != std::string::npos) {
                activeLine = 2; // Highlight dòng: if (findSet(edge.u) != findSet(edge.v))
            } 
            else if (msg.find("bo qua") != std::string::npos || msg.find("bỏ qua") != std::string::npos || msg.find("Bo qua") != std::string::npos) {
                activeLine = 2; // Nếu chu trình thì vẫn bôi dòng if (vì đang kẹt ở bước kiểm tra if)
            } 
            else if (msg.find("Them") != std::string::npos || msg.find("them") != std::string::npos || msg.find("thêm") != std::string::npos) {
                activeLine = 3; // Highlight dòng: push_back (dòng unionSets số 4 sẽ tự động chớp theo)
            }
        }
        for (size_t i = 0; i < m_kruskalCode.size(); ++i) {
            float currentX = 835.f; // Căn lề trái
            float yPos = 70.f + i * 40.f;

            // Bôi đen nền của dòng đang chạy (Highlight line)
            // Nếu đang "Them" cạnh (dòng 3) thì bôi luôn dòng unionSets (dòng 4)
            if ((int)i == activeLine || ((int)i == 4 && activeLine == 3)) { 
                sf::RectangleShape highlightRect({420.f, 36.f});
                highlightRect.setPosition({835.f - 5.f, yPos - 6.f});
                highlightRect.setFillColor(sf::Color(58, 61, 65)); // Màu xám highlight trong VS Code
                window.draw(highlightRect);
            }

            // Vẽ từng chữ có màu sắc nối tiếp nhau
            for (const auto& token : m_kruskalCode[i]) {
                sf::Text t(m_font, token.text, 18);
                t.setPosition({currentX, yPos});
                t.setFillColor(token.color);
                window.draw(t);
                
                // SFML có hàm findCharacterPos cực xịn để tính chính xác tọa độ của chữ tiếp theo
                currentX = t.findCharacterPos(token.text.size()).x; 
            }
        }
    }

    window.draw(m_speedLabel);
    window.draw(m_sliderTrack);
    window.draw(m_sliderKnob);

}