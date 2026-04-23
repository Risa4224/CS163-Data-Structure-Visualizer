#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "../ui/Button.hpp"
#include "../Graph.hpp" // File chứa struct Graph và GraphState ở Bước 1

class GraphScreen {
public:
    GraphScreen(const sf::Font& font);
    void handleEvent(const sf::Event& event, const sf::RenderWindow& window, bool& goBack);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

private:
    void centerTextX(sf::Text& text, float x, float y);
    void createRandomGraph();
    void runKruskal();
    void nextStep();
    void prevStep();


    sf::Font m_font;
    sf::RectangleShape m_background;
    sf::RectangleShape m_leftBar;
    sf::RectangleShape m_menuPanel;

    sf::Text m_topInfoText;
    sf::Text m_messageText;

    sf::Text m_inputLabel;
    sf::RectangleShape m_inputBox;
    sf::Text m_inputText;
    std::string m_input;


    void createCustomGraph(int v, int e);
    bool parseInput(int& v, int& e) const;
    // Các nút điều khiển
    Button m_createRandomButton;
    Button m_createButton;
    Button m_runButton;
    Button m_autoRunButton;
    Button m_prevButton;
    Button m_nextButton;
    Button m_backButton;
    // Thêm biến để kiểm tra xem ô nhập liệu có đang được click (focus) hay không
    bool m_isInputFocused; 

    // Thêm nút Create Random riêng

    // Dữ liệu thuật toán
    Graph m_graph;
    std::vector<GraphState> m_states;
    int m_currentStateIndex;
    bool m_hasResult;
    bool m_isAutoRunning;     
    sf::Clock m_autoRunClock; 
    float m_stepDelay;        
};