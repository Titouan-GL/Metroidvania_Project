#include "../include/Collider.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>



Collider::Collider(sf::RectangleShape &body, int type, bool active, sf::Vector2f prevLocation): m_body(body), m_active(active), m_prevLocation(prevLocation)
{
    m_radius=sqrt(pow(body.getSize().x ,2) + pow(body.getSize().y ,2))/2;
    if(m_prevLocation == sf::Vector2f(-10000,-10000))
    {
        m_prevLocation = body.getPosition();
    }
}

void Collider::ChangeActive()
{
    if (m_active)
    {
        m_active = false;
    }
    else
    {
        m_active = true;
    }
}

bool Collider::CheckCollision(Collider& other)//sf::RenderWindow& window
{
    sf::Vector2f thisPoints[4];
    thisPoints[0] = sf::Vector2f(GetPosition().x, GetPosition().y);
    thisPoints[1] = sf::Vector2f(GetPosition().x + cos(GetRotation())*GetSize().x, GetPosition().y + sin(GetRotation())*GetSize().x);// usage of trigonometry to find the position of the upper right corner even if the rectangle is inclined
    thisPoints[2] = sf::Vector2f(GetPosition().x - sin(GetRotation())*GetSize().y, GetPosition().y + cos(GetRotation())*GetSize().y);
    thisPoints[3] = sf::Vector2f(thisPoints[2].x + cos(GetRotation())*GetSize().x, thisPoints[2].y + sin(GetRotation())*GetSize().x);
    sf::Vector2f otherPoints[4];
    otherPoints[0] = sf::Vector2f(other.GetPosition().x, other.GetPosition().y);
    otherPoints[1] = sf::Vector2f(other.GetPosition().x + cos(other.GetRotation())*other.GetSize().x, other.GetPosition().y + sin(other.GetRotation())*other.GetSize().x);
    otherPoints[2] = sf::Vector2f(other.GetPosition().x - sin(other.GetRotation())*other.GetSize().y, other.GetPosition().y + cos(other.GetRotation())*other.GetSize().y);
    otherPoints[3] = sf::Vector2f(otherPoints[2].x + cos(other.GetRotation())*other.GetSize().x, otherPoints[2].y + sin(other.GetRotation())*other.GetSize().x);


    float distance = sqrt(pow(thisPoints[0].x+(thisPoints[3].x-thisPoints[0].x)/2 - (otherPoints[0].x+(otherPoints[3].x-otherPoints[0].x)/2),2)+pow(thisPoints[0].y+(thisPoints[3].y-thisPoints[0].y)/2 - (otherPoints[0].y + (otherPoints[3].y-otherPoints[0].y)/2),2));  //distance between the two centers

    if (distance < GetRadius()+other.GetRadius() ||1)
    {


        float thisAxisXPoints[4];
        float thisAxisYPoints[4];
        float otherAxisXPoints[4];
        float otherAxisYPoints[4];

        if(GetRotation() > 0)
        {
            thisAxisXPoints[0] = thisPoints[2].x - thisPoints[2].y * tan(M_PI/2 -GetRotation());//first point of this entity on the X axis of this entity
            thisAxisXPoints[1] = thisPoints[1].x - thisPoints[1].y * tan(M_PI/2 -GetRotation());//second point of this entity on the X axis of this entity
            thisAxisXPoints[2] = std::min(otherPoints[2].x - otherPoints[2].y * tan(M_PI/2 -GetRotation()),otherPoints[3].x - otherPoints[3].y * tan(M_PI/2 -GetRotation()));//first point of the other entity on the X axis of this entity
            thisAxisXPoints[3] = std::max(otherPoints[1].x - otherPoints[1].y * tan(M_PI/2 -GetRotation()),otherPoints[0].x - otherPoints[0].y * tan(M_PI/2 -GetRotation()));//second point of the other entity on the X axis of this entity

            thisAxisYPoints[0] = thisPoints[0].y + thisPoints[0].x * tan(M_PI/2 - GetRotation());
            thisAxisYPoints[1] = thisPoints[3].y + thisPoints[3].x * tan(M_PI/2 - GetRotation());
            thisAxisYPoints[2] = std::min(otherPoints[0].y + otherPoints[0].x * tan(M_PI/2 - GetRotation()),otherPoints[2].y + otherPoints[2].x * tan(M_PI/2 - GetRotation()));
            thisAxisYPoints[3] = std::max(otherPoints[3].y + otherPoints[3].x * tan(M_PI/2 - GetRotation()),otherPoints[1].y + otherPoints[1].x * tan(M_PI/2 - GetRotation()));
        }
        else
        {
            thisAxisXPoints[0] = thisPoints[2].x;
            thisAxisXPoints[1] = thisPoints[1].x;
            thisAxisXPoints[2] = otherPoints[2].x;
            thisAxisXPoints[3] = otherPoints[1].x;

            thisAxisYPoints[0] = thisPoints[0].y;
            thisAxisYPoints[1] = thisPoints[3].y;
            thisAxisYPoints[2] = otherPoints[0].y;
            thisAxisYPoints[3] = otherPoints[3].y;
        }

        if(other.GetRotation() > 0)
        {
            otherAxisXPoints[0] = std::min(thisPoints[2].x - thisPoints[2].y * tan(M_PI/2 -other.GetRotation()),thisPoints[3].x - thisPoints[3].y * tan(M_PI/2 -other.GetRotation()));//first point of this entity on the X axis of the other entity
            otherAxisXPoints[1] = std::max(thisPoints[1].x - thisPoints[1].y * tan(M_PI/2 -other.GetRotation()),thisPoints[0].x - thisPoints[0].y * tan(M_PI/2 -other.GetRotation()));
            otherAxisXPoints[2] = otherPoints[2].x - otherPoints[2].y * tan(M_PI/2 -other.GetRotation());
            otherAxisXPoints[3] = otherPoints[1].x - otherPoints[1].y * tan(M_PI/2 -other.GetRotation());

            otherAxisYPoints[0] = std::min(thisPoints[0].y + thisPoints[0].x * tan(M_PI/2 -other.GetRotation()), thisPoints[2].y + thisPoints[2].x * tan(M_PI/2 -other.GetRotation()));
            otherAxisYPoints[1] = std::max(thisPoints[3].y + thisPoints[3].x * tan(M_PI/2 -other.GetRotation()), thisPoints[1].y + thisPoints[1].x * tan(M_PI/2 -other.GetRotation()));
            otherAxisYPoints[2] = otherPoints[0].y + otherPoints[0].x * tan(M_PI/2 -other.GetRotation());
            otherAxisYPoints[3] = otherPoints[3].y + otherPoints[3].x * tan(M_PI/2 -other.GetRotation());
        }
        else
        {
            otherAxisXPoints[0] = thisPoints[2].x;
            otherAxisXPoints[1] = thisPoints[1].x;
            otherAxisXPoints[2] = otherPoints[2].x;
            otherAxisXPoints[3] = otherPoints[1].x;

            otherAxisYPoints[0] = thisPoints[0].y;
            otherAxisYPoints[1] = thisPoints[3].y;
            otherAxisYPoints[2] = otherPoints[0].y;
            otherAxisYPoints[3] = otherPoints[3].y;
        }
        {/*
        sf::Vertex line[] =
        {
            sf::Vertex(sf::Vector2f(thisAxisXPoints[0], 0)),
            sf::Vertex(sf::Vector2f(thisPoints[2]))
        };
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(thisAxisXPoints[1], 0));
        line[1] = sf::Vertex(sf::Vector2f(thisPoints[1]));
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(thisAxisXPoints[3], 0));
        line[1] = sf::Vertex(sf::Vector2f(otherPoints[3]));
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(thisAxisXPoints[2], 0));
        line[1] = sf::Vertex(sf::Vector2f(otherPoints[0]));
        window.draw(line, 2, sf::Lines);



        line[0] = sf::Vertex(sf::Vector2f(0, thisAxisYPoints[1]));
        line[1] = sf::Vertex(sf::Vector2f(thisPoints[2]));
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(0, thisAxisYPoints[0]));
        line[1] = sf::Vertex(sf::Vector2f(thisPoints[1]));
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(0, thisAxisYPoints[3]));
        line[1] = sf::Vertex(sf::Vector2f(otherPoints[2]));
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(0, thisAxisYPoints[2]));
        line[1] = sf::Vertex(sf::Vector2f(otherPoints[1]));
        window.draw(line, 2, sf::Lines);





        line[0] = sf::Vertex(sf::Vector2f( otherAxisXPoints[0],0));
        line[1] = sf::Vertex(sf::Vector2f(thisPoints[0]));
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(otherAxisXPoints[1], 0));
        line[1] = sf::Vertex(sf::Vector2f(thisPoints[3]));
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(otherAxisXPoints[2], 0));
        line[1] = sf::Vertex(sf::Vector2f(otherPoints[0]));
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(otherAxisXPoints[3], 0));
        line[1] = sf::Vertex(sf::Vector2f(otherPoints[3]));
        window.draw(line, 2, sf::Lines);


        line[0] = sf::Vertex(sf::Vector2f(0, otherAxisYPoints[1]));
        line[1] = sf::Vertex(sf::Vector2f(thisPoints[2]));
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(0, otherAxisYPoints[0]));
        line[1] = sf::Vertex(sf::Vector2f(thisPoints[1]));
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(0, otherAxisYPoints[3]));
        line[1] = sf::Vertex(sf::Vector2f(otherPoints[2]));
        window.draw(line, 2, sf::Lines);

        line[0] = sf::Vertex(sf::Vector2f(0, otherAxisYPoints[2]));
        line[1] = sf::Vertex(sf::Vector2f(otherPoints[1]));
        window.draw(line, 2, sf::Lines);//*/
        }

        float thisTotalProjections[4];
        thisTotalProjections[0] = abs(thisAxisXPoints[0]-thisAxisXPoints[1]);//projection on the X axis of this entity
        thisTotalProjections[1] = abs(thisAxisYPoints[0]-thisAxisYPoints[1]);//projection on the Y axis of this entity
        thisTotalProjections[2] = abs(otherAxisXPoints[0]-otherAxisXPoints[1]);//projection on the X axis of the other entity
        thisTotalProjections[3] = abs(otherAxisYPoints[0]-otherAxisYPoints[1]);//projection on the Y axis of the other entity

        float otherTotalProjections[4];
        otherTotalProjections[0] = abs(thisAxisXPoints[2]-thisAxisXPoints[3]);
        otherTotalProjections[1] = abs(thisAxisYPoints[2]-thisAxisYPoints[3]);
        otherTotalProjections[2] = abs(otherAxisXPoints[2]-otherAxisXPoints[3]);
        otherTotalProjections[3] = abs(otherAxisYPoints[2]-otherAxisYPoints[3]);

        float cumulativeTotalProjections[4];
        cumulativeTotalProjections[0] = std::max(abs(thisAxisXPoints[0]-thisAxisXPoints[3]),abs(thisAxisXPoints[1]-thisAxisXPoints[2]));
        cumulativeTotalProjections[1] = std::max(abs(thisAxisYPoints[0]-thisAxisYPoints[3]),abs(thisAxisYPoints[1]-thisAxisYPoints[2]));
        cumulativeTotalProjections[2] = std::max(abs(otherAxisXPoints[0]-otherAxisXPoints[3]),abs(otherAxisXPoints[1]-otherAxisXPoints[2]));
        cumulativeTotalProjections[3] = std::max(abs(otherAxisYPoints[0]-otherAxisYPoints[3]),abs(otherAxisYPoints[1]-otherAxisYPoints[2]));

        bool verif = cumulativeTotalProjections[0] <= thisTotalProjections[0] + otherTotalProjections[0] && cumulativeTotalProjections[1] <= thisTotalProjections[1] + otherTotalProjections[1]
                    && cumulativeTotalProjections[2] <= thisTotalProjections[2] + otherTotalProjections[2] && cumulativeTotalProjections[3] <= thisTotalProjections[3] + otherTotalProjections[3];


        //std::cout << (bool)(cumulativeTotalProjections[0] < thisTotalProjections[0]+ otherTotalProjections[0])
        //            <<(bool)(cumulativeTotalProjections[1] < thisTotalProjections[1] + otherTotalProjections[1])
        //            <<(bool)(cumulativeTotalProjections[2] < thisTotalProjections[2] + otherTotalProjections[2])
        //            <<(bool)(cumulativeTotalProjections[3] < thisTotalProjections[3] + otherTotalProjections[3])<< std::endl;
        //std::cout << cumulativeTotalProjections[0] << ", "<< cumulativeTotalProjections[1] << ", "<< cumulativeTotalProjections[2] << ", "<< cumulativeTotalProjections[3] << std::endl;
        return verif;
    }
    return false;
}

sf::Vector2f Collider::Repel(Collider other, sf::Vector2f& direction)
{
    sf::Vector2f Offset = sf::Vector2f(0.0f, 0.0f); //offset is the value by wich "other" should be moved
    if(m_active)
    {
        sf::Vector2f otherHalfSize = other.GetSize()/2.0f;
        sf::Vector2f otherPosition = other.GetPosition()+otherHalfSize;
        sf::Vector2f thisHalfSize = GetSize()/2.0f;
        sf::Vector2f thisPosition = GetPosition()+thisHalfSize;

        float deltaX = otherPosition.x - thisPosition.x;
        float deltaY = otherPosition.y - thisPosition.y;

        float intersectX = abs(deltaX) - (otherHalfSize.x + thisHalfSize.x);
        float intersectY = abs(deltaY) - (otherHalfSize.y + thisHalfSize.y);
        if (intersectX < 0.0f && intersectY < 0.0f)//we will first try to move it relative to it's previous location (more accurate) then depending on it's distance from the center (if an entity is created inside another)
        {
            if (other.GetPreviousPosition().y + other.GetSize().y <= GetPosition().y)//other collide on it's bottom
            {
                Offset.y =  other.GetPosition().y -(GetPosition().y-other.GetSize().y);
                direction.x = 0.0f;
                direction.y = -1.0f;
            }
            else if (other.GetPreviousPosition().y >= GetPosition().y+GetSize().y)//other collide on it's top
            {
                Offset.y = other.GetPosition().y-(GetPosition().y + GetSize().y);
                direction.x = 0.0f;
                direction.y = 1.0f;
            }
            else if (other.GetPreviousPosition().x + other.GetSize().x <= GetPosition().x)//other collide on it's right
            {
                Offset.x = other.GetPosition().x - (GetPosition().x-other.GetSize().x);
                direction.x = 1.0f;
                direction.y = 0.0f;
            }
            else if (other.GetPreviousPosition().x >= GetPosition().x+GetSize().x)//other collide on it's left
            {
                Offset.x = other.GetPosition().x - (GetPosition().x+GetSize().x);
                direction.x = -1.0f;
                direction.y = 0.0f;
            }
            else if (intersectX > intersectY)
            {
                if (deltaX > 0.0f)
                {
                    Offset.x = intersectX;
                }
                else
                {
                    Offset.x = -intersectX;
                }
            }
            else
            {
                if (deltaY > 0.0f)
                {
                    Offset.y = intersectY;
                }
                else
                {
                    Offset.y = -intersectY;
                }
            }
        }
    }
    return Offset;
}


sf::Vector2f Collider::Distance(Collider other)
{
    sf::Vector2f distance = sf::Vector2f(0.0f, 0.0f);
    if(m_active)
    {
        sf::Vector2f otherHalfSize = other.GetSize()/2.0f;
        sf::Vector2f otherPosition = other.GetPosition()+otherHalfSize;
        sf::Vector2f thisHalfSize = GetSize()/2.0f;
        sf::Vector2f thisPosition = GetPosition()+thisHalfSize;

        float deltaX = otherPosition.x - thisPosition.x;
        float deltaY = otherPosition.y - thisPosition.y;

        distance.x = deltaX;
        distance.y = deltaY;

    }
    return distance;
}

Collider::~Collider()
{
    //dtor
}
