#ifndef GROUND_H
#define GROUND_H

#include "Collider.h"
#include <SFML/Graphics.hpp>

class Ground
{
    public:
        Ground(sf::Vector2f size, sf::Vector2f position, int type);//platforms
        Ground(sf::Vector2f size, sf::Vector2f position, int type,sf::Vector2f destination, int direction);//traps
        Ground(sf::Vector2f size, sf::Vector2f position, int type, int destinationMap, int destinationOrientation , sf::Vector2f destination);//doors
        virtual ~Ground();

        void Draw(sf::RenderWindow& window);

        /*
        gives the collider used for collision
        */
        Collider GetCollider();
        int GetType() {return m_type;}
        int GetNextMap() {return m_destinationMap;}
        int GetNextDirection() {return m_destinationOrientation;}
        int GetDirection() {return m_direction;}
        sf::Vector2f GetNextLocation() {return m_destinationLocation;}
        sf::Vector2f GetPosition();
        sf::Vector2f GetSize();


    protected:

    sf::RectangleShape m_body;
    int m_type; //0 = no platform, 1 = regular platforms, 2 = damaging platform
    int m_direction;

    int m_rotation;
    sf::Vector2f m_destinationLocation;//basic information for if the platform is actually a door
    int m_destinationOrientation;// 0 = none, 1 = right, 2 = left, 3 = up, 4= down
    int m_destinationMap; //the index in which the next map is stored

    private:
};

#endif // GROUND_H
