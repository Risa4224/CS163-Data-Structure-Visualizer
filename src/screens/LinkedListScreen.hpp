#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include <string>
#include <vector>
#include <functional>
#include "../ui/Button.hpp"

struct LinkedListConsts {
    // linked list consts
    static constexpr int MAX_N = 35; // max num of elements
    static constexpr int MAX_A = 99; // max num of value
    static constexpr int MAX_LINE = 7; // max num of elements on a line
    static constexpr int AUTO_TIME = 1; // number of seconds per auto tick

    // pjsk color pallete
    // it's my project i get to do whatever i want
    // niigo
    sf::Color c_kanadeColor = sf::Color(187, 101, 136);
    sf::Color c_mafuyuColor = sf::Color(136, 137, 204);
    sf::Color c_enaColor = sf::Color(204, 170, 135);
    sf::Color c_mizukiColor = sf::Color(228, 168, 202);
    // leoni
    sf::Color c_ichikaColor = sf::Color(51, 170, 238);
    sf::Color c_sakiColor = sf::Color(255, 221, 68);
    sf::Color c_honamiColor = sf::Color(238, 102, 102);    
    sf::Color c_shihoColor = sf::Color(187, 221, 34);
};
static constexpr LinkedListConsts ll_consts;

class LinkedListObject
{
private:
    // consts
    static constexpr int MAXN = ll_consts.MAX_N;
    static constexpr int MAXA = ll_consts.MAX_A; 
    static constexpr int MAXLINE = ll_consts.MAX_LINE;
    const sf::Font& m_font;
    const sf::Color c_styleButtonColor = ll_consts.c_mizukiColor;
    const sf::Color c_numberBlockBorderColor = ll_consts.c_kanadeColor;
    const sf::Color c_numberBlockBorderColorFocused = ll_consts.c_mafuyuColor;
    const std::string playCharacter = "P";
    const std::string pauseCharacter = "||";

    // vars
    float blockSize = 40.f;
    float blockBorder = 5.f;
    float blockTotalSize = blockSize + blockBorder * 2;
    float listX, listY;
    float stepsX, stepsY;

    // steps
    // don't like this
    bool isStep = false, isAuto = false;
    int curStep, cntSteps;
    std::string mode = "";
    int s_pos, s_value;

    // DS - singly linked list
    // in vector form because i am NOT writing an actual linked list
    std::vector <int> list;
    // copy of list for steps
    // this is NOT efficient but this code is not efficient anyway
    // so who cares
    std::vector <int> stepList; 

    // constant UI elements
    sf::Text m_stepText;
    sf::Clock m_clock;
    
    Button m_leftStepButton;
    Button m_rightStepButton;
    Button m_autoStepButton;

    // basic UI blocks
    void drawArrow(sf::RenderWindow& window, 
        float startX, float startY, float endX, float endY) const; 
    void drawNumberBlock(sf::RenderWindow& window, 
        float x, float y, int value, bool isFocused) const;

    // steps
    void updateMode(int steps, std::string m, int p, int v);
    bool leftStep();
    bool rightStep();
    bool toggleAuto();

    // drawing
    void drawStepMenu(sf::RenderWindow& window) const;
    void drawStep(sf::RenderWindow& window) const;
    void drawLinkedList(sf::RenderWindow& window) const;

public:
    explicit LinkedListObject(const sf::Font& font,
        float listX, float listY, float stepsX, float stepsY);

    // dealing with linked list
    int generate();
    int pushFront(int value);
    int insert(int pos, int value);
    int popFront();
    int remove(int pos);
    int updateValue(int pos, int value);
    int searchPos(int pos, int &value);

    // UI
    void handleEvent(const sf::Event& event, 
        const sf::RenderWindow& window);
    void update(const sf::RenderWindow& window, std::string &message);
    void draw(sf::RenderWindow& window) const;
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
    static constexpr int MAXN = ll_consts.MAX_N;
    static constexpr int MAXA = ll_consts.MAX_A; 
    
    // UI colors
    const sf::Color c_overallBackgroundColor = ll_consts.c_ichikaColor;
    const sf::Color c_menuPanelColor = ll_consts.c_enaColor;
    const sf::Color c_styleButtonColor = ll_consts.c_mizukiColor;
    const sf::Color c_positionBoxBorderColor = ll_consts.c_sakiColor;
    const sf::Color c_valueBoxBorderColor = ll_consts.c_shihoColor;
    const sf::Color c_messageTextColor = ll_consts.c_honamiColor;

    const sf::Color c_errorColor = ll_consts.c_honamiColor;
    const sf::Color c_highlightColor = ll_consts.c_mafuyuColor;


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
