#include "../include/Fading.h"
#include <math.h>
#include <iostream>

Fading::Fading(float time, sf::RectangleShape screen) : m_timeMax(time), m_time(0), m_screen(screen), m_alpha(0)
{
    //ctor
}

void Fading::Draw(sf::RenderWindow& window, sf::View view)
{
    m_screen.setFillColor(sf::Color(0,0,0,m_alpha));
    m_screen.setSize(sf::Vector2f(view.getSize()));
    m_screen.setPosition(view.getCenter().x - view.getSize().x/2,view.getCenter().y - view.getSize().y/2);
    window.draw(m_screen);
}

void Fading::Fade(float deltaTime)
{
    m_alpha = (int)(255*(std::min(m_time/m_timeMax, 1.0f)));
    //std::cout << m_time/m_timeMax << std::endl;
    if (m_time < m_timeMax)
        m_time +=deltaTime;
    else
        m_time = m_timeMax;
}

void Fading::UnFade(float deltaTime)
{
    m_alpha = (int)(255*(std::max(m_time/m_timeMax, 0.0f)));
    if (m_time > 0.0f)
        m_time-=deltaTime;
    else
        m_time = 0.0f;
}

Fading::~Fading()
{
    //dtor
}
