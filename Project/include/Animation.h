#ifndef ANIMATION_H
#define ANIMATION_H

#include <SFML/Graphics.hpp>

class Animation
{
    public:
    Animation();
    Animation(sf::Texture texture, sf::Vector2u imageCount, float switchTime);
    virtual ~Animation();

    sf::IntRect getUvRect(){return m_uvRect;}
    int getCurrentImage(){return m_currentImage.x;}
    void SetCurrentImage(int x){m_currentImage.x = x;}

    /*
    Updates the animation depending on the time
    */
    void Update(int row, float deltaTime, bool faceRight);

    protected:

    sf::IntRect m_uvRect; //the square of the total texture representing the current sprite
    sf::Vector2u m_imageCount;//the number of sprites in an animation
    sf::Vector2u m_currentImage;//the position of the current sprite being displayed
    float m_totalTime;//the time for which the animation has been going
    float m_switchTime;//the time needed to go from one sprite to another

    private:
};

#endif // ANIMATION_H
