#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include "../ui/Button.hpp"

struct AnimationStep {
    std::string message;
    int highlightLine = -1;
    std::vector<std::vector<int>> tableSnapshot;
    int activeBucket = -1;
    int activeNode = -1;
};

class HashTableScreen {
private:
    static constexpr int FIXED_N = 10;
    const sf::Font &m_font;

    // Data
    std::vector<std::vector<int>> m_table;
    std::vector<AnimationStep> m_steps;
    size_t m_currentStep = 0;
    std::vector<std::string> m_pseudocode;

    // UI Components
    Button m_createButton, m_searchButton, m_insertButton, m_removeButton, m_backButton;
    Button m_prevButton, m_nextButton, m_playPauseButton;
    
    sf::Text m_messageText, m_inputText, m_topInfoText, m_speedText;
    sf::RectangleShape m_inputBox, m_background, m_leftBar;
    
    // Speed Slider Components
    sf::RectangleShape m_sliderBar;
    sf::CircleShape m_sliderKnob;
    
    // Automation Logic
    bool m_isPlaying = false;
    sf::Clock m_autoTimer;
    float m_speedScale = 1.0f; // 0.5x to 4.0x
    std::string m_input;
    int m_elementCount = 0;

private:
    void addStep(const std::string &msg, int line, int bucket = -1, int node = -1);
    void setupPseudocode(const std::string &action);
    void createTable();
    int hashFunction(int value) const;

public:
    explicit HashTableScreen(const sf::Font& font);
    void handleEvent(const sf::Event& event, sf::RenderWindow& window, bool& goBack);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;

    void prepareSearch(int value);
    void prepareInsert(int value);
    void prepareRemove(int value);
};