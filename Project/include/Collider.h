#ifndef COLLISION_H
#define COLLISION_H

#include <SFML/Graphics.hpp>
#include <math.h>

class Collider
{
    public:
    Collider(sf::RectangleShape &body, int type = 1, bool active = true, sf::Vector2f prevLocation = sf::Vector2f(-10000, -10000));//nothing can be at coordinate -10000,-10000 or it's collision will bug
    virtual ~Collider();

    /*
    moves the colliding object
    */
    void SetPosition(float dx, float dy) {m_body.setPosition(dx, dy);}

    /*
    if active becomes inactive and if inactive become active
    */
    void ChangeActive();

    /*
    check if other and the current object are colliding. return true if they are
    */
    bool CheckCollision(Collider& other);

    /*
    if two entities are colliding, this function will return the distance by wich the other collider should be moved
    */
    sf::Vector2f Repel(Collider other, sf::Vector2f& direction);

    /*
    gives the distance between two entities
    */
    sf::Vector2f Distance(Collider other);

    sf::Vector2f GetPosition() {return m_body.getPosition();}
    sf::Vector2f GetPreviousPosition() {return m_prevLocation;}
    sf::Vector2f GetSize() {return m_body.getSize();}
    float GetRotation() {return m_body.getRotation()*(M_PI/180);}
    float GetRadius() {return m_radius;}
    bool GetActive() {return m_active;}

    protected:

    sf::RectangleShape m_body;
    float m_radius;//the radius of the collider (works even if it's a rectangle, it's the distance between the center and the point the further away from the center
    int m_type;//1 fot rectangle and 2 for circles (but circles have never been implemented)
    bool m_active;
    sf::Vector2f m_prevLocation;

    private:
};

#endif // COLLISION_H
