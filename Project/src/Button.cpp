#include "../include/Button.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Button::Button(sf::Vector2f position, sf::RectangleShape body)
{
    m_activated = false;
    m_checked = false;
    m_body = body;
    m_body.setPosition(position);
    m_colorOn = sf::Color(0,150,0);
    m_colorOff = sf::Color(30,120,30);
    m_body.setFillColor(m_colorOff);
}

Button::~Button()
{
    //dtor
}

void Button::Clicking(sf::Vector2f mousePos)
{
    if(mousePos.x < m_body.getPosition().x + m_body.getSize().x && mousePos.x > m_body.getPosition().x &&
    mousePos.y < m_body.getPosition().y + m_body.getSize().y && mousePos.y > m_body.getPosition().y && m_activated)
    {
        Clicked();
    }
    else if (!m_alwaysOn)
    {
        Unclicked();
    }
}

void Button::Draw(sf::RenderWindow& window)
{
    if (m_activated)
    {
        window.draw(m_body);
    }
}
