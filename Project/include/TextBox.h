#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <SFML/Graphics.hpp>
#include <string>

class TextBox
{
    public:
    TextBox(sf::Vector2f position = sf::Vector2f(0,0), sf::RectangleShape dimentions = sf::RectangleShape(sf::Vector2f(500, 40)));
    virtual ~TextBox();
    void Draw(sf::RenderWindow& window);
    void Selection(sf::Vector2f mousePos);
    void Write(sf::Event evnt);
    void Erase();
    void SetString(std::string txt) {m_text.setString(txt);}
    void SetInput(std::string txt) {m_input = txt;}
    void SetPosition(sf::Vector2f position) {m_body.setPosition(position); m_text.setPosition(position.x+12, position.y+12);}
    void setSize(sf::Vector2f dimentions) {m_body.setSize(dimentions);}
    void Select() {m_selected = true; m_body.setFillColor(sf::Color(255,255,255));}
    bool IsSelected() {return m_selected;}
    void Unselect() {m_selected = false; m_body.setFillColor(sf::Color(230,230,230));}
    void BecomeRed() {m_body.setFillColor(sf::Color(150,30,30));}
    void Activated() {m_activated = true;}
    void Deactivate() {m_activated = false;}
    std::string GetText() {return m_input;}

    protected:
    bool m_activated;
    sf::RectangleShape m_body;
    sf::Font m_playfair;
    sf::Text m_text;
    sf::String m_input;
    bool m_selected;


    private:
};

#endif // TEXTBOX_H

