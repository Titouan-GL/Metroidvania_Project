#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <iostream>

class Button
{
    public:
        Button(sf::Vector2f position = sf::Vector2f(0,0), sf::RectangleShape dimentions = sf::RectangleShape(sf::Vector2f(100, 40)));
        virtual ~Button();
        void Draw(sf::RenderWindow& window);
        void Clicking(sf::Vector2f mousePos);
        void SetPosition(sf::Vector2f position) {m_body.setPosition(position);}
        void setSize(sf::Vector2f dimentions) {m_body.setSize(dimentions);}
        void Clicked() {m_checked = true; m_body.setFillColor(m_colorOn);}
        void Unclicked() {m_checked = false; m_body.setFillColor(m_colorOff);}
        void Activated() {m_activated = true;}
        void Deactivate() {m_activated = false;}
        bool IsChecked() {return m_checked;}
        void SetAlwaysOn(bool b) {m_alwaysOn = b;}
        void SetColorOn(sf::Color color) {m_colorOn = color; if(m_checked){m_body.setFillColor(color);}}
        void SetColorOff(sf::Color color) {m_colorOff = color; if(!m_checked){m_body.setFillColor(color);}}

    protected:
    bool m_activated;
    bool m_checked;
    bool m_alwaysOn;
    sf::RectangleShape m_body;
    sf::Color m_colorOn;
    sf::Color m_colorOff;

    private:
};

#endif // BUTTON_H
