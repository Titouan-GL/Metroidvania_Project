#include "../include/TextBox.h"
#include <SFML/Graphics.hpp>
#include <iostream>

TextBox::TextBox(sf::Vector2f position, sf::RectangleShape body)
{
    m_selected = false;
    m_activated = false;
    m_body = body;
    m_body.setPosition(position);
    m_body.setFillColor(sf::Color(230,230,230));
    m_playfair.loadFromFile("Fonts/Playfair/PlayfairDisplay-Black.ttf");
    m_text.setFont(m_playfair);
    m_text.setCharacterSize(16);
    m_text.setFillColor(sf::Color(0,0,0));
    m_text.setPosition(position.x+12, position.y+12);
    m_text.setString("text Placeholder");
}

TextBox::~TextBox()
{
    //dtor
}

void TextBox::Selection(sf::Vector2f mousePos)
{
    if(mousePos.x < m_body.getPosition().x + m_body.getSize().x && mousePos.x > m_body.getPosition().x &&
    mousePos.y < m_body.getPosition().y + m_body.getSize().y && mousePos.y > m_body.getPosition().y && m_activated)
    {
        Select();
    }
    else
    {
        Unselect();
    }
}

void TextBox::Draw(sf::RenderWindow& window)
{
    if (m_activated)
    {
        window.draw(m_body);
        window.draw(m_text);
    }
}


void TextBox::Write(sf::Event evnt)
{
    if (m_selected)
    {
        if (evnt.text.unicode < 128)
        {
            m_input += evnt.text.unicode;
            m_text.setString(m_input);
        }
    }
}

void TextBox::Erase()
{
    if (m_input.getSize()>=1 && m_selected)
    {
        m_input.erase(m_input.getSize()-1,1);
        m_text.setString(m_input);
    }
}
