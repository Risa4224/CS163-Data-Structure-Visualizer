#include "LinkedListScreen.hpp"
#include <cstdlib>
#include <functional>
#include <iostream>
#include <ctime>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

/*
Important parameters:
Screen: 1280 x 720

the level of hardcoding here is making me cry
*/

LinkedListObject::LinkedListObject(const sf::Font& font,
    float listX, float listY, float stepsX, float stepsY)
    : m_font(font),
      listX(listX),
      listY(listY),
      stepsX(stepsX),
      stepsY(stepsY),

      m_stepText(m_font, "", 24),

      m_leftStepButton({50.f, 50.f}, {stepsX-80.f, stepsY+2.f}, font, "<", 24),
      m_autoStepButton({50.f, 50.f}, {stepsX-25.f, stepsY+2.f}, font, playCharacter, 24),
      m_rightStepButton({50.f, 50.f}, {stepsX+30.f, stepsY+2.f}, font, ">", 24)
{
    generate();

    m_stepText.setFillColor(sf::Color::Black);

    auto styleMenuButton = [this](Button& b) {
        sf::Color normalColor = c_styleButtonColor;
        sf::Color hoverColor = normalColor;
        hoverColor.a = 128;

        b.setNormalColor(normalColor);
        b.setHoverColor(hoverColor);
        b.setSelectedColor(hoverColor); // what is this for????
    };
    styleMenuButton(m_leftStepButton);
    styleMenuButton(m_rightStepButton);
    styleMenuButton(m_autoStepButton);
}

void LinkedListObject::updateMode(int steps, std::string m, int p, int v)
{
    isAuto = false; isStep = true;
    curStep = 1, cntSteps = steps;
    mode = m;
    s_pos =  p, s_value = v;
    stepList = list;
}

int LinkedListObject::generate()
{
    try 
    {
        //disable steps and auto
        isStep = isAuto = false;

        //create new list
        list.clear();
        for (int i=0;i<MAXN/2;i++)
            list.push_back(rand()%MAXA);
        return 0;
    }
    catch (int err) 
    {
        return -1;
    }
}

int LinkedListObject::pushFront(int value)
{
    return insert(0, value);
}

int LinkedListObject::insert(int pos, int value)
{
    if (list.size() >= MAXN) return 1; // list reached limit
    if (pos > list.size() || pos < 0) return 2; // invalid position

    try 
    {
        // update mode
        if (list.size()==0)
        {
            // if array is empty then there is no step
            isStep = isAuto = false;
            list.push_back(value);
            return 0;
        }
            
        updateMode(pos + 2, "Insert", pos, value);

        // add member
        list.push_back(0);
        for (int i=(int)(list.size())-1;i>pos;i--)
            list[i] = list[i-1];
        list[pos] = value;
        return 0;
    }
    catch (int err)
    {
        return -1;
    }
}

int LinkedListObject::popFront()
{
    return remove(0);
}

int LinkedListObject::remove(int pos)
{
    if (list.size() == 0) return 1; // list is empty
    if (pos >= list.size() || pos < 0) return 2; // invalid position 

    try 
    {
        // update mode
        if (list.size()-1 == pos)
        {
            //if removed element is last element
            //then the reconnecting step is skipped
            updateMode(pos + 2, "Remove", pos, -1);
            list.pop_back();
            return 0;
        }
        if (pos == 0)
        {
            // if removed element is start of list
            // then reconnecting step is skipped
            updateMode(pos + 2, "Remove", pos, -1);
        }
        else updateMode(pos + 3, "Remove", pos, -1);

        // remove pos
        for (int i=pos;i+1<list.size();i++)
            list[i] = list[i+1];
        list.pop_back();
        return 0;
    }
    catch (int err)
    {
        return -1;
    }
}

int LinkedListObject::updateValue(int pos, int value)
{
    if (pos >= list.size() || pos < 0) return 2; // invalid position 

    try 
    {
        // update mode
        updateMode(pos + 2, "Update", pos, value);

        // update value
        list[pos] = value;
        return 0;
    }
    catch (int err)
    {
        return -1;
    }
}

int LinkedListObject::searchPos(int pos, int &value)
{
    if (pos >= list.size() || pos < 0) return 2; // invalid position 

    try 
    {
        // update mode
        updateMode(pos + 1, "Search", pos, value);

        // search
        value = list[pos];
        return 0;
    }
    catch (int err)
    {
        return -1;
    }
}

void LinkedListObject::drawArrow(sf::RenderWindow& window, 
    float startX, float startY, float endX, float endY) const
{
    float length = std::sqrt((endX - startX) * (endX - startX)
                    +  (endY - startY) * (endY - startY));
    float radius = std::min(length / 5.f, 8.f);

    sf::RectangleShape line{{length, 2.f}};
    sf::CircleShape triangle(radius, 3);
    line.setOrigin({0, 1.f});
    triangle.setOrigin({radius, 0});
    auto rad = sf::radians(std::atan2(endY - startY, endX - startX));
    line.setRotation(rad);
    triangle.setRotation(rad + sf::degrees(90));
    line.setPosition({startX, startY});
    triangle.setPosition({endX, endY});
    window.draw(line); window.draw(triangle);
}

void LinkedListObject::drawNumberBlock(sf::RenderWindow& window, 
    float x, float y, int value, bool isFocused) const
{
    sf::RectangleShape block;
    block.setSize({blockSize, blockSize});
    block.setPosition({x-(blockSize / 2.f), y-(blockSize / 2.f)});
    block.setFillColor(sf::Color::White);
    block.setOutlineThickness(blockBorder);
    if (isFocused) block.setOutlineColor(c_numberBlockBorderColorFocused);
    else block.setOutlineColor(c_numberBlockBorderColor);
    
    sf::Text number(m_font, std::to_string(value), 24);
    number.setFillColor(sf::Color::Black);
    sf::FloatRect bounds = number.getLocalBounds();
    number.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });
    number.setPosition({x, y});
    
    window.draw(block);
    window.draw(number);
}

bool LinkedListObject::leftStep()
{
    if (curStep == 1) return false; // button is hidden
    curStep--; return true;
}

bool LinkedListObject::rightStep()
{
    if (curStep == cntSteps) return false; // button is hidden
    curStep++; return true;
}

bool LinkedListObject::toggleAuto()
{
    if (curStep == cntSteps) return false; // button is hidden
    isAuto = !isAuto; return true;
}

void LinkedListObject::handleEvent(const sf::Event& event, 
    const sf::RenderWindow& window)
{
    // pos of mouse
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    // if mouse is hovering
    // this feels inefficient?
    if (event.is<sf::Event::MouseMoved>())
    {
        m_leftStepButton.update(mousePos);
        m_rightStepButton.update(mousePos);
        m_autoStepButton.update(mousePos);
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (m_leftStepButton.contains(mousePos))
        {
            isAuto = false;
            leftStep();
            return;
        }

        if (m_rightStepButton.contains(mousePos))
        {
            isAuto = false;
            rightStep();
            return;
        }

        if (m_autoStepButton.contains(mousePos))
        {
            toggleAuto();
            return;
        }
    }
}

void LinkedListObject::update(const sf::RenderWindow& window, std::string &message)
{
    if (isAuto) 
    {
        m_autoStepButton.setText(pauseCharacter);
        if (!m_clock.isRunning()) m_clock.start(); // start clock
        // auto tick
        else if (m_clock.getElapsedTime().asSeconds() >= ll_consts.AUTO_TIME)
        {
            m_clock.restart();
            if (!rightStep()) // last element already, stop auto
            {
                isAuto = false;
                m_autoStepButton.setText(playCharacter);
                m_clock.reset();
            }
        }
    }    
    else 
    {
        if (m_clock.isRunning()) m_clock.reset();
        m_autoStepButton.setText(playCharacter);
    }

    if (isStep)
    {
        m_stepText.setString("Step " + std::to_string(curStep)
                            + "/" + std::to_string(cntSteps));
        
        if (curStep - 1 <= s_pos - (mode == "Insert"))
        {
            if (curStep == cntSteps && mode == "Search") 
                message = "position " + std::to_string(s_pos) + " found, has value " + std::to_string(list[s_pos]);
            else
                message = "pointer at position " + std::to_string(curStep - 1);
        }
        else if (mode == "Insert")
        {
            if (curStep + 1 == cntSteps)
                message = "disconnected pointer between positions " + std::to_string(s_pos) 
                        + " and " + std::to_string(s_pos+1) + ", created a new position";
            else if (curStep == cntSteps)
                message = "reconnected the positions to accomodate the new position";
        }
        else if (mode == "Remove")
        {
            if (curStep + 1 == cntSteps || stepList.size() == 1)
                message = "disconnected all pointers to position " + std::to_string(s_pos) + ", and remove the position";
            else if (curStep == cntSteps)
                message = "reconnected the remaining positions";
        }
        else if (mode == "Update")
        {
            message = "updated position " + std::to_string(s_pos) + " to have value " + std::to_string(s_value);
        }
    }
    else m_stepText.setString("");
    sf::FloatRect bounds = m_stepText.getLocalBounds();
    m_stepText.setOrigin({
        bounds.position.x + bounds.size.x / 2.f,
        bounds.position.y + bounds.size.y / 2.f
    });
    m_stepText.setPosition({stepsX, stepsY-32.f});

}

void LinkedListObject::drawStepMenu(sf::RenderWindow& window) const
{
    window.draw(m_stepText);

    if (curStep != 1) m_leftStepButton.draw(window);
    if (curStep != cntSteps)
    {
        m_rightStepButton.draw(window);
        m_autoStepButton.draw(window);
    }
}

// most inefficient function ever
// i also had a whole 2 hours to write it
void LinkedListObject::drawStep(sf::RenderWindow& window) const
{
    // the steps where we only iterate through the list
    if (curStep - 1 <= s_pos - (mode == "Insert"))
    {
        int noLines = (int)(stepList.size()) / MAXLINE + ((int)(stepList.size()) % MAXLINE != 0);
        float startX = listX - blockTotalSize * (MAXLINE - 1);
        float startY = listY - blockTotalSize * (noLines - 1);

        for (int i=0; i*MAXLINE < stepList.size(); i++)
        {
            for (int j=0; j < MAXLINE && i*MAXLINE+j < stepList.size(); j++)
            {
                int pos = i * MAXLINE + j;
                float blockX = startX + blockTotalSize * j * 2;
                float blockY = startY + blockTotalSize * i * 2;
                drawNumberBlock(window, blockX, blockY, stepList[pos], (curStep - 1 == pos));
                if (j+1<MAXLINE && pos+1<stepList.size())
                {
                    float nextBlockX = blockX + blockTotalSize * 2;
                    drawArrow(window, blockX + blockTotalSize / 2.f, blockY, 
                                      nextBlockX - blockTotalSize / 2.f, blockY);
                }
            }
            if ((i+1) * MAXLINE < stepList.size())
                //bottom left corner of top block 
                //to top right corner of bottom block
                drawArrow(
                    window,
                    startX + blockTotalSize * (MAXLINE-1) * 2 - (blockTotalSize / 2.f), // top block x
                    startY + blockTotalSize * i * 2 + (blockTotalSize / 2.f), // top block y
                    startX + (blockTotalSize / 2.f), // bottom block x
                    startY + blockTotalSize * (i+1) * 2 - (blockTotalSize / 2.f) // bottom block y
                );
        }
        return;
    }

    if (mode == "Insert")
    {
        if (curStep + 1 == cntSteps)
        {
            // 2nd to last step: disconnect the points, create a new node
            // we can actually do this by just drawing the original array and remove some of the arrows
            int noLines = (int)(list.size()) / MAXLINE + ((int)(list.size()) % MAXLINE != 0);
            float startX = listX - blockTotalSize * (MAXLINE - 1);
            float startY = listY - blockTotalSize * (noLines - 1);

            for (int i=0; i*MAXLINE < list.size(); i++)
            {
                for (int j=0; j < MAXLINE && i*MAXLINE+j < list.size(); j++)
                {
                    int pos = i * MAXLINE + j;
                    float blockX = startX + blockTotalSize * j * 2;
                    float blockY = startY + blockTotalSize * i * 2;
                    drawNumberBlock(window, blockX, blockY, list[pos], (pos == s_pos));
                    
                    if (pos == s_pos || pos + 1 == s_pos) continue;
                    if (j+1<MAXLINE && pos+1<list.size())
                    {
                        float nextBlockX = blockX + blockTotalSize * 2;
                        drawArrow(window, blockX + blockTotalSize / 2.f, blockY, 
                                          nextBlockX - blockTotalSize / 2.f, blockY);
                    }
                }

                if ((i+1) * MAXLINE - 1 == s_pos || (i+1) * MAXLINE == s_pos) continue;
                if ((i+1) * MAXLINE < list.size())
                    //bottom left corner of top block 
                    //to top right corner of bottom block
                    drawArrow(
                        window,
                        startX + blockTotalSize * (MAXLINE-1) * 2 - (blockTotalSize / 2.f), // top block x
                        startY + blockTotalSize * i * 2 + (blockTotalSize / 2.f), // top block y
                        startX + (blockTotalSize / 2.f), // bottom block x
                        startY + blockTotalSize * (i+1) * 2 - (blockTotalSize / 2.f) // bottom block y
                    );
            }
        }
        else if (curStep == cntSteps)
        {
            // last step: reconnect everything
            // in other words it's literally just the original array
            int noLines = (int)(list.size()) / MAXLINE + ((int)(list.size()) % MAXLINE != 0);
            float startX = listX - blockTotalSize * (MAXLINE - 1);
            float startY = listY - blockTotalSize * (noLines - 1);
            
            for (int i=0; i*MAXLINE < list.size(); i++)
            {
                for (int j=0; j < MAXLINE && i*MAXLINE+j < list.size(); j++)
                {
                    int pos = i * MAXLINE + j;
                    float blockX = startX + blockTotalSize * j * 2;
                    float blockY = startY + blockTotalSize * i * 2;
                    drawNumberBlock(window, blockX, blockY, list[pos], (pos == s_pos));
                    if (j+1<MAXLINE && pos+1<list.size())
                    {
                        float nextBlockX = blockX + blockTotalSize * 2;
                        drawArrow(window, blockX + blockTotalSize / 2.f, blockY, 
                                          nextBlockX - blockTotalSize / 2.f, blockY);
                    }
                }
                if ((i+1) * MAXLINE < list.size())
                    //bottom left corner of top block 
                    //to top right corner of bottom block
                    drawArrow(
                        window,
                        startX + blockTotalSize * (MAXLINE-1) * 2 - (blockTotalSize / 2.f), // top block x
                        startY + blockTotalSize * i * 2 + (blockTotalSize / 2.f), // top block y
                        startX + (blockTotalSize / 2.f), // bottom block x
                        startY + blockTotalSize * (i+1) * 2 - (blockTotalSize / 2.f) // bottom block y
                    );
            }
        }
        return;
    }

    if (mode == "Remove")
    {
        if (curStep + 1 == cntSteps || stepList.size() == 1)
        {
            // second to last step: remove the element
            // unless stepList only has 1 element then it's the last step
            int noLines = (int)(stepList.size()) / MAXLINE + ((int)(stepList.size()) % MAXLINE != 0);
            float startX = listX - blockTotalSize * (MAXLINE - 1);
            float startY = listY - blockTotalSize * (noLines - 1);

            for (int i=0; i*MAXLINE < stepList.size(); i++)
            {
                for (int j=0; j < MAXLINE && i*MAXLINE+j < stepList.size(); j++)
                {
                    int pos = i * MAXLINE + j;
                    if (pos == s_pos) continue;
                    float blockX = startX + blockTotalSize * j * 2;
                    float blockY = startY + blockTotalSize * i * 2;
                    drawNumberBlock(window, blockX, blockY, stepList[pos], (pos == s_pos));
                    
                    if (pos + 1 == s_pos) continue;
                    if (j+1<MAXLINE && pos+1<stepList.size())
                    {
                        float nextBlockX = blockX + blockTotalSize * 2;
                        drawArrow(window, blockX + blockTotalSize / 2.f, blockY, 
                                          nextBlockX - blockTotalSize / 2.f, blockY);
                    }
                }

                if ((i+1) * MAXLINE - 1 == s_pos || (i+1) * MAXLINE == s_pos) continue;
                if ((i+1) * MAXLINE < stepList.size())
                    //bottom left corner of top block 
                    //to top right corner of bottom block
                    drawArrow(
                        window,
                        startX + blockTotalSize * (MAXLINE-1) * 2 - (blockTotalSize / 2.f), // top block x
                        startY + blockTotalSize * i * 2 + (blockTotalSize / 2.f), // top block y
                        startX + (blockTotalSize / 2.f), // bottom block x
                        startY + blockTotalSize * (i+1) * 2 - (blockTotalSize / 2.f) // bottom block y
                    );
            }
        }
        else if (curStep == cntSteps)
        {
            // last step: reconnect everything
            int noLines = (int)(stepList.size()) / MAXLINE + ((int)(stepList.size()) % MAXLINE != 0);
            float startX = listX - blockTotalSize * (MAXLINE - 1);
            float startY = listY - blockTotalSize * (noLines - 1);

            for (int i=0; i*MAXLINE < stepList.size(); i++)
            {
                for (int j=0; j < MAXLINE && i*MAXLINE+j < stepList.size(); j++)
                {
                    int pos = i * MAXLINE + j;
                    if (pos == s_pos) continue;
                    float blockX = startX + blockTotalSize * j * 2;
                    float blockY = startY + blockTotalSize * i * 2;
                    drawNumberBlock(window, blockX, blockY, stepList[pos], (pos == s_pos));
                    
                    if (pos + 1 == s_pos && pos + 1 != (i+1) * MAXLINE) 
                    {
                        if (j+2 < MAXLINE)
                        {
                            // skip 1 block over
                            float nextBlockX = blockX + blockTotalSize * 4;
                            drawArrow(window, blockX + blockTotalSize / 2.f, blockY, 
                                          nextBlockX - blockTotalSize / 2.f, blockY);
                        }
                        else
                        {
                            // 2nd last block top row, first block bottom row
                            drawArrow(
                                window,
                                startX + blockTotalSize * j * 2 - (blockTotalSize / 2.f), // top block x
                                startY + blockTotalSize * i * 2 + (blockTotalSize / 2.f), // top block y
                                startX + (blockTotalSize / 2.f), // bottom block x
                                startY + blockTotalSize * (i+1) * 2 - (blockTotalSize / 2.f) // bottom block y
                            );
                        }
                        continue;
                    }

                    if (j+1<MAXLINE && pos+1<stepList.size())
                    {
                        float nextBlockX = blockX + blockTotalSize * 2;
                        drawArrow(window, blockX + blockTotalSize / 2.f, blockY, 
                                          nextBlockX - blockTotalSize / 2.f, blockY);
                    }
                }

                if ((i+1) * MAXLINE == s_pos)
                {
                    // last block top row, 2nd last block bottom row
                    drawArrow(
                        window,
                        startX + blockTotalSize * (MAXLINE-1) * 2 - (blockTotalSize / 2.f), // top block x
                        startY + blockTotalSize * i * 2 + (blockTotalSize / 2.f), // top block y
                        startX + blockTotalSize * 2 + (blockTotalSize / 2.f), // bottom block x
                        startY + blockTotalSize * (i+1) * 2 - (blockTotalSize / 2.f) // bottom block y
                    );
                    continue;
                }
                if ((i+1) * MAXLINE - 1 == s_pos || (i+1) * MAXLINE == s_pos) continue;
                if ((i+1) * MAXLINE < stepList.size())
                    //bottom left corner of top block 
                    //to top right corner of bottom block
                    drawArrow(
                        window,
                        startX + blockTotalSize * (MAXLINE-1) * 2 - (blockTotalSize / 2.f), // top block x
                        startY + blockTotalSize * i * 2 + (blockTotalSize / 2.f), // top block y
                        startX + (blockTotalSize / 2.f), // bottom block x
                        startY + blockTotalSize * (i+1) * 2 - (blockTotalSize / 2.f) // bottom block y
                    );
            }
        }
        return;
    }

    if (mode == "Update")
    {
        // last step: update the element
        // just print the new list now
        int noLines = (int)(list.size()) / MAXLINE + ((int)(list.size()) % MAXLINE != 0);
        float startX = listX - blockTotalSize * (MAXLINE - 1);
        float startY = listY - blockTotalSize * (noLines - 1);

        for (int i=0; i*MAXLINE < list.size(); i++)
        {
            for (int j=0; j < MAXLINE && i*MAXLINE+j < list.size(); j++)
            {
                int pos = i * MAXLINE + j;
                float blockX = startX + blockTotalSize * j * 2;
                float blockY = startY + blockTotalSize * i * 2;
                drawNumberBlock(window, blockX, blockY, list[pos], (pos == s_pos));
                if (j+1<MAXLINE && pos+1<list.size())
                {
                    float nextBlockX = blockX + blockTotalSize * 2;
                    drawArrow(window, blockX + blockTotalSize / 2.f, blockY, 
                                      nextBlockX - blockTotalSize / 2.f, blockY);
                }
            }
            if ((i+1) * MAXLINE < list.size())
                //bottom left corner of top block 
                //to top right corner of bottom block
                drawArrow(
                    window,
                    startX + blockTotalSize * (MAXLINE-1) * 2 - (blockTotalSize / 2.f), // top block x
                    startY + blockTotalSize * i * 2 + (blockTotalSize / 2.f), // top block y
                    startX + (blockTotalSize / 2.f), // bottom block x
                    startY + blockTotalSize * (i+1) * 2 - (blockTotalSize / 2.f) // bottom block y
                );
        }

        return;
    }
    
    // no search because that's already done above
}

void LinkedListObject::drawLinkedList(sf::RenderWindow& window) const
{
    // preliminary checks
    if (list.size()==0) return;

    // the part where we draw the linked list
    int noLines = (int)(list.size()) / MAXLINE + ((int)(list.size()) % MAXLINE != 0);
    float startX = listX - blockTotalSize * (MAXLINE - 1);
    float startY = listY - blockTotalSize * (noLines - 1);
    for (int i=0; i*MAXLINE < list.size(); i++)
    {
        for (int j=0; j < MAXLINE && i*MAXLINE+j < list.size(); j++)
        {
            int pos = i * MAXLINE + j;
            float blockX = startX + blockTotalSize * j * 2;
            float blockY = startY + blockTotalSize * i * 2;
            drawNumberBlock(window, blockX, blockY, list[pos], false);
            if (j+1<MAXLINE && pos+1<list.size())
            {
                float nextBlockX = blockX + blockTotalSize * 2;
                drawArrow(window, blockX + blockTotalSize / 2.f, blockY, 
                                  nextBlockX - blockTotalSize / 2.f, blockY);
            }
        }
        if ((i+1) * MAXLINE < list.size())
            //bottom left corner of top block 
            //to top right corner of bottom block
            drawArrow(
                window,
                startX + blockTotalSize * (MAXLINE-1) * 2 - (blockTotalSize / 2.f), // top block x
                startY + blockTotalSize * i * 2 + (blockTotalSize / 2.f), // top block y
                startX + (blockTotalSize / 2.f), // bottom block x
                startY + blockTotalSize * (i+1) * 2 - (blockTotalSize / 2.f) // bottom block y
            );
    }
}

void LinkedListObject::draw(sf::RenderWindow& window) const
{    
    if (isStep)
    {
        drawStepMenu(window);
        drawStep(window);
    }
    else drawLinkedList(window);
}

InputBox::InputBox(InputBoxOptions options)
    : m_font(options.font),
    
      isInputFocused(false),
      input(""),
      isInputLegal(options.func),

      pos_x(options.x),
      pos_y(options.y),
      m_fontSize(options.fontSize),
      m_label(options.label),

      c_borderColor(options.borderColor),
      c_borderColorFocused(options.borderColorFocused),
      c_textColor(options.textColor),

      m_inputLabel(m_font, m_label, m_fontSize),
      m_inputText(m_font, "", m_fontSize)
{
    m_inputLabel.setFillColor(c_textColor);
    m_inputLabel.setPosition({pos_x, pos_y});

    m_inputBox.setSize({190.f, m_fontSize + 20.f});
    m_inputBox.setPosition({pos_x, pos_y + m_fontSize + 5.f});
    m_inputBox.setFillColor(sf::Color::White);
    m_inputBox.setOutlineThickness(2.f);
    m_inputBox.setOutlineColor(c_borderColor);

    m_inputText.setFillColor(c_textColor);
    m_inputText.setPosition({1080.f, pos_y + m_fontSize + 15.f});
}

int InputBox::appendChar(char c)
{
    // rewrite this later
    if (c < '0' || c > '9') return 2; // not digit char
    
    // wait i never noticed the arbitrary limit here uh
    int value;
    std::string newInput = input + c;
    if (!isInputLegal(value, newInput)) return 1; // not legal input
    
    input = newInput;
    return 0;
}

int InputBox::removeLast()
{
    if (input.empty()) return 1; // nothing to pop
    
    input.pop_back();
    return 0;
}

// not sure if i'll ever use this tbh
int InputBox::clearInput()
{
    input.clear();
    return 0;
}

int InputBox::getInput(int &value)
{
    if (!isInputLegal(value, input)) return 1; // bad input
    return 0;
}

void InputBox::updateLabel(std::string newLabel)
{
    m_inputLabel.setString(newLabel);
}

int InputBox::handleEvent(const sf::Event& event, const sf::RenderWindow& window)
{
    // pos of mouse
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    
    //text input
    if (const auto* textEvent = event.getIf<sf::Event::TextEntered>())
    {
        if (!isInputFocused) return 0;
     
        char c = static_cast<char>(textEvent->unicode);

        if (textEvent->unicode == 8) return removeLast();
        return appendChar(c);
    }

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        if (m_inputBox.getGlobalBounds().contains(mousePos)) 
        {
            isInputFocused = true;
            m_inputBox.setOutlineColor(c_borderColorFocused);
        }
        else
        {
            isInputFocused = false;
            m_inputBox.setOutlineColor(c_borderColor);
        }
    }
    return 0;
}

void InputBox::update()
{
    if (input.empty())
        m_inputText.setString("...");
    else
        m_inputText.setString(input);
}

void InputBox::draw(sf::RenderWindow& window) const
{
    window.draw(m_inputLabel);
    window.draw(m_inputBox);
    window.draw(m_inputText);
}

LinkedListScreen::LinkedListScreen(const sf::Font& font)
    : m_font(font),
      list(font, 640.f, 360.f, 1175.f, 520.f),

      // UI
      m_topInfoText(font, "Singly Linked List Visualizer", 32),
      m_messageText(font, "Ready.", 20),

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
    std::function <bool(int&, std::string)> positionChecker =
    [](int &value, std::string input) {
        if (input.empty()) return false;

        try
        {
            value = std::stoi(input);
            return (value >= 0  && value <= MAXN);
        }
        catch (...)
        {
            return false;
        }
    };
    InputBoxOptions positionBoxOptions(font, positionChecker, "Position", 1080.f, 294.f);
    positionBoxOptions.borderColor = c_positionBoxBorderColor;
    positionBoxOptions.borderColorFocused = c_highlightColor;
    m_inputPosition.emplace(positionBoxOptions);

    // value input box
    std::function <bool(int&, std::string)> valueChecker =
    [](int &value, std::string input) {
        if (input.empty()) return false;

        try
        {
            value = std::stoi(input);
            return (value >= 0  && value <= MAXA);
        }
        catch (...)
        {
            return false;
        }
    };
    InputBoxOptions valueBoxOptions(font, valueChecker, "Value", 1080.f, 384.f);
    valueBoxOptions.borderColor = c_valueBoxBorderColor;
    valueBoxOptions.borderColorFocused = c_highlightColor;
    m_inputValue.emplace(valueBoxOptions);

    // info text
    m_topInfoText.setFillColor(sf::Color::Black);
    centerTextX(m_topInfoText, 640.f, 24.f);               

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

void LinkedListScreen::generate()
{
    int err = list.generate();    
    switch (err) {
        case 0:
            m_message = "generated a new list";
            break;
        default:
            m_message = "something deeply wrong happened";
            break;
    }
    return;
}

void LinkedListScreen::pushFront(int value)
{
    int err = list.pushFront(value);
    switch (err) {
        case 1:
            m_message = "linked list reached display limit, can't push a new member";
            break;
        case 0:
            m_message = "added " + std::to_string(value) + " to start of list";
            break;
        default:
            m_message = "something deeply wrong happened";
            break;
    }
    return;
}

void LinkedListScreen::insert(int pos, int value)
{
    int err = list.insert(pos, value);
    switch (err) {
        case 2:
            m_message = "invalid position";
            break;
        case 1:
            m_message = "linked list reached display limit, can't insert a new member";
            break;
        case 0:
            m_message = "inserted " + std::to_string(value) + " to position " + std::to_string(pos);
            break;
        default:
            m_message = "something deeply wrong happened";
            break;
    }    
    return;
}

void LinkedListScreen::popFront()
{
    int err = list.popFront();
    switch (err) {
        case 1:
            m_message = "linked list is empty";
            break;
        case 0:
            m_message = "deleted the first position";
            break;
        default:
            m_message = "something deeply wrong happened";
            break;
    }   
    return;
}

void LinkedListScreen::remove(int pos)
{
    int err = list.remove(pos);
    switch (err) {
        case 2:
            m_message = "invalid position";
            break;
        case 1:
            m_message = "linked list is empty";
            break;
        case 0:
            m_message = "deleted value at position " + std::to_string(pos);
            break;
        default:
            m_message = "something deeply wrong happened";
            break;
    }
    return;
}

void LinkedListScreen::update(int pos, int value)
{
    int err = list.updateValue(pos, value);
    switch (err) {
        case 2:
            m_message = "invalid position";
            break;
        case 0:
            m_message = "update position " + std::to_string(pos) + 
                        " with value" + std::to_string(value);
            break;
        default:
            m_message = "something deeply wrong happened";
            break;
    }
    return;
}

void LinkedListScreen::searchPos(int pos)
{
    int value, err = list.searchPos(pos, value);
    switch (err) {
        case 2:
            m_message = "invalid position";
            break;
        case 0:
            m_message = "position " + std::to_string(pos) + " has value " + std::to_string(value);
            break;
        default:
            m_message = "something deeply wrong happened";
            break;
    }
    return;
}

void LinkedListScreen::handleEvent(const sf::Event& event, const sf::RenderWindow& window, bool& goBack)
{
    //linked list events
    list.handleEvent(event, window);

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

    //input boxes
    int errPos = m_inputPosition->handleEvent(event, window);
    int errVal = m_inputValue->handleEvent(event, window);

    if (errPos == 1)
    {
        m_message = "Please enter a valid number\n(Number must be between 0 and " + std::to_string(MAXN-1) + ")";
        return;
    }
    if (errVal == 1)
    {
        m_message = "Please enter a valid number\n(Number must be between 0 and " + std::to_string(MAXA) + ")";  
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
        
        // this feels inefficient. again.
        // does c++ has something like lua?
        // like iterate through table for objects

        auto getPosValue = [this]()
        {
            int position = 0;
            int err = m_inputPosition->getInput(position);
            if (err == 1)
            {
                m_message = "Please enter a valid position value\n(Number must be between 0 and " + std::to_string(MAXN-1) + ")";
                return -1;
            }
            return position;
        };
        auto getValValue = [this]()
        {
            int value = 0;
            int err = m_inputValue->getInput(value);
            if (err == 1)
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

    // updating values
    m_inputPosition->update();
    m_inputValue->update();
    std::string listMessage = "";
    list.update(window, listMessage);
    if (listMessage != "") m_message = listMessage;
    
    m_messageText.setString(m_message);
    centerTextX(m_messageText, 640.f, 690.f);
}

void LinkedListScreen::draw(sf::RenderWindow& window) const
{
    // drawing everything
    window.draw(m_background);
    window.draw(m_menuPanel);

    window.draw(m_topInfoText);

    m_inputPosition->draw(window);
    m_inputValue->draw(window);

    m_generateButton.draw(window);
    m_pushFrontButton.draw(window);
    m_insertButton.draw(window);
    m_popFrontButton.draw(window);
    m_removeButton.draw(window);
    m_updateButton.draw(window);
    m_searchPosButton.draw(window);
    m_backButton.draw(window);

    window.draw(m_messageText);

    list.draw(window);
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