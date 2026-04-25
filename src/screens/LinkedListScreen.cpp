#include "LinkedListScreen.hpp"
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <iomanip>

/*
Important parameters:
Screen: 1280 x 720

color pallete: hooooonestly considering zookster's, 
that'll be dealt with later
it would be REALLY funny if i went with full trans flag pallete

Button background: 228, 168, 202 (mizuki)
Overall background: 51, 170, 238 (ichika)
Notice text: 238, 102, 102 (honami)
border of position input box: 255, 221, 68 (saki)
border of value input box: 187, 221, 34 (shiho)
*/

LinkedListScreen::LinkedListScreen(const sf::Font& font)
    : m_font(font),

      m_topInfoText(font, "Singly Linked List Visualizer", 32),
      m_messageText(font, "Ready.", 20),
      m_inputPositionLabel(font, "Position", 20),
      m_inputPositionText(font, "", 24),
      m_inputValueLabel(font, "Value", 20),
      m_inputValueText(font, "", 24),

      // i don't like hardcoding positions here
      // might return to this
      // what are these variables anyway
      m_generateButton({190.f, 46.f}, {10.f, 294.f}, font, "Generate a list", 22),
      m_pushFrontButton({190.f, 46.f}, {10.f, 346.f}, font, "pushFront(value)", 22),
      m_insertButton({190.f, 46.f}, {10.f, 398.f}, font, "insert(pos, value)", 22),
      m_popFrontButton({190.f, 46.f}, {10.f, 450.f}, font, "popFront()", 22),
      m_removeButton({190.f, 46.f}, {10.f, 502.f}, font, "remove(pos)", 22),
      m_updateButton({190.f, 46.f}, {10.f, 554.f}, font, "update(pos, value)", 22),
      m_searchPosButton({190.f, 46.f}, {10.f, 606.f}, font, "searchPos(pos)", 22),
      m_backButton({190.f, 46.f}, {10.f, 658.f}, font, "Back", 22)
{
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // backgrounds
    m_background.setSize({1280.f, 720.f});
    m_background.setPosition({0.f, 0.f});
    m_background.setFillColor(c_overallBackgroundColor);

    // there's gotta be an easier way to do this????
    m_menuPanel.setSize({210.f, 436.f});
    m_menuPanel.setPosition({0.f, 284.f});
    m_menuPanel.setFillColor(c_menuPanelColor);

    // position input box
    m_inputPositionLabel.setFillColor(sf::Color::Black);
    m_inputPositionLabel.setPosition({1080.f, 294.f});

    m_inputPositionBox.setSize({190.f, 50.f});
    m_inputPositionBox.setPosition({1080.f, 324.f});
    m_inputPositionBox.setFillColor(sf::Color::White);
    m_inputPositionBox.setOutlineThickness(2.f);
    m_inputPositionBox.setOutlineColor(c_positionBoxBorderColor);

    m_inputPositionText.setFillColor(sf::Color::Black);
    m_inputPositionText.setPosition({1080.f, 334.f});

    // value input box
    // there's GOTTA BE another way
    m_inputValueLabel.setFillColor(sf::Color::Black);
    m_inputValueLabel.setPosition({1080.f, 384.f});

    m_inputValueBox.setSize({190.f, 50.f});
    m_inputValueBox.setPosition({1080.f, 414.f});
    m_inputValueBox.setFillColor(sf::Color::White);
    m_inputValueBox.setOutlineThickness(2.f);
    m_inputValueBox.setOutlineColor(c_valueBoxBorderColor);

    m_inputValueText.setFillColor(sf::Color::Black);
    m_inputValueText.setPosition({1080.f, 424.f});

    // info text
    // need to look into what centerTextX actually does
    m_topInfoText.setFillColor(sf::Color::Black);
    centerTextX(m_topInfoText, 640.f, 24.f);
    std::cerr<<m_topInfoText.getPosition().x<<" "
             <<m_topInfoText.getPosition().y<<" ";                

    m_messageText.setFillColor(c_messageTextColor);
    centerTextX(m_messageText, 640.f, 690.f);

    // buttons
    auto styleMenuButton = [this](Button& b) {
        sf::Color normalColor = c_styleButtonColor;
        sf::Color hoverColor = normalColor;
        hoverColor.a = 128;

        b.setNormalColor(normalColor);
        b.setHoverColor(hoverColor);
        b.setSelectedColor(sf::Color(190, 65, 45)); // what is this for????
    };

    styleMenuButton(m_generateButton);
    styleMenuButton(m_pushFrontButton);
    styleMenuButton(m_insertButton);
    styleMenuButton(m_popFrontButton);
    styleMenuButton(m_removeButton);
    styleMenuButton(m_updateButton);
    styleMenuButton(m_searchPosButton);
    styleMenuButton(m_backButton);
}

void LinkedListScreen::nextStep()
{
    // dead function
    return;
}

void LinkedListScreen::lastStep()
{
    // dead function
    return;
}

void LinkedListScreen::generate()
{
    list.clear();
    for (int i=0;i<MAXN/2;i++)
        list.push_back(rand()%MAXA);
    
    m_message = "generated a new list";
    return;
}

void LinkedListScreen::pushFront(int value)
{
    if (list.size() >= MAXN)
    {
        m_message = "linked list reached display limit, can't push a new member";
        return;
    }

    list.push_back(0);
    for (int i=(int)(list.size())-1;i>0;i--)
        list[i] = list[i-1];
    list[0] = value;

    m_message = "added " + std::to_string(value) + " to start of list";
    return;
}

void LinkedListScreen::insert(int pos, int value)
{
    if (list.size() >= MAXN)
    {
        m_message = "linked list reached display limit, can't insert a new member";
        return;
    }

    list.push_back(0);
    for (int i=(int)(list.size())-1;i>pos;i--)
        list[i] = list[i-1];
    list[pos] = value;

    m_message = "inserted " + std::to_string(value) + " to position " + std::to_string(pos);
    return;
}

void LinkedListScreen::popFront()
{
    if (list.size() == 0)
    {
        m_message = "linked list is empty";
        return;
    }

    for (int i=0;i+1<list.size();i++)
        list[i] = list[i+1];
    list.pop_back();

    m_message = "deleted the first position";
    return;
}

void LinkedListScreen::remove(int pos)
{
    if (list.size() == 0)
    {
        m_message = "linked list is empty";
        return;
    }

    for (int i=pos;i+1<list.size();i++)
        list[i] = list[i+1];
    list.pop_back();

    m_message = "deleted value at position " + std::to_string(pos);
    return;
}

void LinkedListScreen::update(int pos, int value)
{
    if (list.size() >= pos)
    {
        m_message = "linked list does not have enough elements";
        return;
    }

    list[pos] = value;
    m_message = "update position " + std::to_string(pos) + 
                " with value" + std::to_string(value);
    return;
}

void LinkedListScreen::searchPos(int pos)
{
    if (list.size() >= pos)
    {
        m_message = "linked list does not have enough elements";
        return;
    }
    
    m_message = "position " + std::to_string(pos) + " has value " + std::to_string(list[pos]);
    return;
}

bool LinkedListScreen::parseInput(int& value, std::string m_input) const
{
    if (m_input.empty())
        return false;

    try
    {
        value = std::stoi(m_input);
        return (value >= 0  && value <= MAXA);
    }
    catch (...)
    {
        return false;
    }
}

void LinkedListScreen::appendDigit(char digit, std::string &m_input)
{
    if (m_input.size() < 9)
        m_input += digit;
}

void LinkedListScreen::removeLastDigit(std::string &m_input)
{
    if (!m_input.empty())
        m_input.pop_back();
}

void LinkedListScreen::clearInput(std::string &m_input)
{
    m_input.clear();
}

void LinkedListScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window, bool& goBack)
{
    // pos of mouse
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    // if mouse is hovering
    // this feels inefficient?
    if (event.is<sf::Event::MouseMoved>())
    {
        m_generateButton.update(mousePos);
        m_pushFrontButton.update(mousePos);
        m_insertButton.update(mousePos);
        m_popFrontButton.update(mousePos);
        m_removeButton.update(mousePos);
        m_updateButton.update(mousePos);
        m_searchPosButton.update(mousePos);
        m_backButton.update(mousePos);
    }

    //text input
    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>())
    {
        if (!m_isPositionInputFocused && !m_isValueInputFocused) return;
     
        char c = static_cast<char>(textEvent->unicode);

        if (c >= '0' && c <= '9')
        {
            if (m_isPositionInputFocused) appendDigit(c, m_inputPosition);
            if (m_isValueInputFocused) appendDigit(c, m_inputValue);
        }
        else if (textEvent->unicode == 8)
        {
            if (m_isPositionInputFocused) removeLastDigit(m_inputPosition);
            if (m_isValueInputFocused) removeLastDigit(m_inputValue);
        }
        return;
    }

    // mouse click
    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (mousePressed->button != sf::Mouse::Button::Left)
            return;

        if (m_backButton.contains(mousePos))
        {
            goBack = true;
            return;
        }

        if (m_generateButton.contains(mousePos))
        {
            generate();
            return;
        }

        if (m_popFrontButton.contains(mousePos))
        {
            popFront();
            return;
        }

        if (m_inputPositionBox.getGlobalBounds().contains(mousePos)) {
            m_isPositionInputFocused = true;
            m_isValueInputFocused = false;
            m_inputPositionBox.setOutlineColor(c_highlightColor);
            m_inputValueBox.setOutlineColor(c_valueBoxBorderColor);
            m_message = "Please enter a valid number\n(Number must be between 0 and " + std::to_string(MAXN) + ")";
            return;
        }
        else if (m_inputValueBox.getGlobalBounds().contains(mousePos)) {
            m_isPositionInputFocused = false;
            m_isValueInputFocused = true;
            m_inputPositionBox.setOutlineColor(c_positionBoxBorderColor);
            m_inputValueBox.setOutlineColor(c_highlightColor);
            m_message = "Please enter a valid number\n(Number must be between 0 and " + std::to_string(MAXA) + ")";  
            return;
        } 
        else {
            m_isPositionInputFocused = false;
            m_isValueInputFocused = false;
            m_inputPositionBox.setOutlineColor(c_positionBoxBorderColor);
            m_inputValueBox.setOutlineColor(c_valueBoxBorderColor);
            m_message = "";
        }
        
        // this feels inefficient. again.
        // does c++ has something like lua?
        // like iterate through table for objects

        // lambda to get pos and val
        // i don't like this solution
        // will return
        auto getPosValue = [this]()
        {
            int position = 0;
            if (!parseInput(position, m_inputPosition))
            {
                m_message = "Please enter a valid position value\n(Number must be between 0 and " + std::to_string(MAXN) + ")";
                return -1;
            }
            return position;
        };
        auto getValValue = [this]()
        {
            int value = 0;
            if (!parseInput(value, m_inputValue))
            {
                m_message = "Please enter a valid value\n(Number must be between 0 and " + std::to_string(MAXA) + ")";
                return - 1;
            }
            return value;
        };
        
        
        //functions that require position
        if (m_removeButton.contains(mousePos))
        {
            int position = getPosValue();
            if (position == -1) return;
            remove(position);
            return;
        }

        if (m_searchPosButton.contains(mousePos))
        {
            int position = getPosValue();
            if (position == -1) return;
            searchPos(position);
            return;
        }

        //functions that require value
        if (m_pushFrontButton.contains(mousePos))
        {
            int value = getValValue();
            if (value == -1) return;
            pushFront(value);
            return;
        }

        //functions that require both position and value
        if (m_insertButton.contains(mousePos))
        {
            int position = getPosValue();
            if (position == -1) return;
            int value = getValValue();
            if (value == -1) return;
            insert(position, value);
            return;
        }

        if (m_updateButton.contains(mousePos))
        {
            int position = getPosValue();
            if (position == -1) return;
            int value = getValValue();
            if (value == -1) return;
            update(position, value);
            return;
        }
    }
}

void LinkedListScreen::update(const sf::RenderWindow& window)
{
    (void)window; //what is this black magic

    // constantly??????
    // there's gotta be another way??????
    if (m_inputPosition.empty())
        m_inputPositionText.setString("...");
    else
        m_inputPositionText.setString(m_inputPosition);
    if (m_inputValue.empty())
        m_inputValueText.setString("...");
    else
        m_inputValueText.setString(m_inputValue);
    
    m_messageText.setString(m_message);
    centerTextX(m_messageText, 640.f, 690.f);
}

void LinkedListScreen::draw(sf::RenderWindow& window) const
{
    // drawing everything
    window.draw(m_background);
    window.draw(m_menuPanel);

    window.draw(m_topInfoText);

    window.draw(m_inputPositionLabel);
    window.draw(m_inputPositionBox);
    window.draw(m_inputPositionText);
    window.draw(m_inputValueLabel);
    window.draw(m_inputValueBox);
    window.draw(m_inputValueText);

    m_generateButton.draw(window);
    m_pushFrontButton.draw(window);
    m_insertButton.draw(window);
    m_popFrontButton.draw(window);
    m_removeButton.draw(window);
    m_updateButton.draw(window);
    m_searchPosButton.draw(window);
    m_backButton.draw(window);

    window.draw(m_messageText);

    // the part where we draw the linked list
    if (list.size()==0) return;
    sf::Text printLinkedList(m_font, "", 20);
    std::string linkedListString = std::to_string(list[0]);
    for (int i=1;i<list.size();i++)
        linkedListString = linkedListString + ", " + std::to_string(list[i]);
    printLinkedList.setString(linkedListString);
    printLinkedList.setFillColor(sf::Color::Black);
    // why do we have to rewrite the entire thing here????
    // remember to investigate
    sf::FloatRect bounds = printLinkedList.getLocalBounds();
    printLinkedList.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });
    printLinkedList.setPosition({640, 360});
    window.draw(printLinkedList);
}

// ok it took me way too long to get what this function does
// it centers the text with the center at (x,y)
void LinkedListScreen::centerTextX(sf::Text& text, float x, float y)
{
    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });
    text.setPosition({x, y});
}