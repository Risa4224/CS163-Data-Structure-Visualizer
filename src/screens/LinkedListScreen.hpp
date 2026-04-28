#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <vector>
#include <functional>
#include "../ui/Button.hpp"

#define MAX_N 40
#define MAX_A 99 // limit to 2 digits for easier UI
#define MAX_LINE 8 // number of blocks on a line

class LinkedListObject
{
private:
    // limits
    static constexpr int MAXN = MAX_N;
    static constexpr int MAXA = MAX_A; 
    static constexpr int MAXLINE = MAX_LINE;
    const sf::Font& m_font;

    // DS - singly linked list
    // in vector form because i am NOT writing an actual linked list
    std::vector <int> list;
public:
    explicit LinkedListObject(const sf::Font& font);

    // dealing with linked list
    int generate();
    int pushFront(int value);
    int insert(int pos, int value);
    int popFront();
    int remove(int pos);
    int update(int pos, int value);
    int searchPos(int pos, int &value);

    // drawing
    void drawArrow (sf::RenderWindow& window, 
        float startX, float startY, float endX, float endY) const; 
    void drawNumberBlock(sf::RenderWindow& window, 
        float x, float y, int value) const;
    void draw(sf::RenderWindow& window, float x, float y) const;
};

struct InputBoxOptions 
{
    const sf::Font& font;
    std::function<bool(int&, std::string)> func;
    std::string label;
    float x, y;
    sf::Color backgroundColor = sf::Color::White;
    sf::Color borderColor = sf::Color(128, 128, 128);
    sf::Color borderColorFocused = sf::Color::Black;
    sf::Color textColor = sf::Color::Black;
    float fontSize = 24.f;

    InputBoxOptions(const sf::Font& font,
        std::function<bool(int&, std::string)> func,
        std::string label, float x, float y)
    : font(font), func(func), label(label), x(x), y(y) {};
};

class InputBox 
{
private:
    //dealing with input
    bool isInputFocused;
    std::string input;
    std::function<bool(int&, std::string)> isInputLegal;

    //UI
    float pos_x, pos_y;
    int m_fontSize;
    std::string m_label;
    const sf::Font& m_font;

    sf::Color c_borderColor;
    sf::Color c_borderColorFocused;
    sf::Color c_textColor;

    sf::Text m_inputLabel;
    sf::Text m_inputText;

    sf::RectangleShape m_inputBox;

    // input functions
    // only digits for now
    int appendChar(char c);
    int removeLast();
    int clearInput();
public:
    explicit InputBox(InputBoxOptions options);

    // get value
    int getInput(int &value);

    // UI
    void updateLabel(std::string newLabel);

    int handleEvent(const sf::Event& event, 
        const sf::RenderWindow& window);
    void update();
    void draw(sf::RenderWindow& window) const;
};

class LinkedListScreen
{
private:
    static constexpr int MAXN = MAX_N;
    static constexpr int MAXA = MAX_A; 

    // pjsk color pallete
    // it's my project i get to do whatever i want
    sf::Color c_mizukiColor = sf::Color(228, 168, 202);
    sf::Color c_mafuyuColor = sf::Color(136, 137, 204);
    sf::Color c_enaColor = sf::Color(204, 170, 135);
    sf::Color c_ichikaColor = sf::Color(51, 170, 238);
    sf::Color c_honamiColor = sf::Color(238, 102, 102);
    sf::Color c_sakiColor = sf::Color(255, 221, 68);
    sf::Color c_shihoColor = sf::Color(187, 221, 34);
    
    // UI colors
    sf::Color c_overallBackgroundColor = c_ichikaColor;
    sf::Color c_menuPanelColor = c_enaColor;
    sf::Color c_styleButtonColor = c_mizukiColor;
    sf::Color c_positionBoxBorderColor = c_sakiColor;
    sf::Color c_valueBoxBorderColor = c_shihoColor;
    sf::Color c_messageTextColor = c_honamiColor;

    sf::Color c_errorColor = c_honamiColor;
    sf::Color c_highlightColor = c_mafuyuColor;


    // UI things
    const sf::Font& m_font;

    // UI
    sf::Text m_topInfoText;
    sf::Text m_messageText;
    
    sf::RectangleShape m_background;
    sf::RectangleShape m_menuPanel;
    
    std::optional<InputBox> m_inputPosition;
    std::optional<InputBox> m_inputValue;

    // input
    std::string m_message = "Ready.";

    // action buttons
    Button m_generateButton;
    Button m_pushFrontButton;
    Button m_insertButton;
    Button m_popFrontButton;
    Button m_removeButton;
    Button m_updateButton;
    Button m_searchPosButton;
    Button m_backButton;

    // linked list
    LinkedListObject list;

private:
    void centerTextX(sf::Text& text, float x, float y);

    void nextStep();
    void lastStep();

    void generate();
    void pushFront(int value);
    void insert(int pos, int value);
    void popFront();
    void remove(int pos);
    void update(int pos, int value);
    void searchPos(int pos);

public:
    explicit LinkedListScreen(const sf::Font& font);

    void handleEvent(const sf::Event& event, 
        const sf::RenderWindow& window, bool& goBack);
    void update(const sf::RenderWindow& window);
    void draw(sf::RenderWindow& window) const;
};

#undef MAX_N
#undef MAX_A
#undef MAX_LINE