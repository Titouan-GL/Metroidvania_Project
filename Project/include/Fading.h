#ifndef FADING_H
#define FADING_H

#include <SFML/Graphics.hpp>

class Fading
{
    public:
        Fading(float time, sf::RectangleShape screen);
        virtual ~Fading();
        void Draw(sf::RenderWindow &window, sf::View view);
        void Fade(float deltaTime);
        void UnFade(float deltaTime);

    protected:

    float m_timeMax;
    float m_time;
    sf::RectangleShape m_screen;
    int m_alpha;

    private:
};

#endif // FADING_H
