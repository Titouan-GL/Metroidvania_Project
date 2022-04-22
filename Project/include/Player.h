#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>
#include <math.h>
#include "Animation.h"
#include "Collider.h"
#include "Living_Entity.h"
#include "Ground.h"
#include <fstream>
#include <vector>

class Player : public Living_Entity
{
    public:
        //primary//
        Player();
        virtual ~Player();

        //actions//
        /*
        defines what the player will do depending on the inputs
        */
        void Inputs(float deltaTime);
        void GetHit();
        void Attacking(float deltaTime);
        void Reset();

        //interactions//
        /*
        if the player touch a trap, trapped will be used
        */
        void Trapped(sf::Vector2f nextLocation);

        /*
        if the player touched a trap and then is freed the freed will be used
        */
        void Freed() {m_canJump = true; m_canMove = true;}
        bool Door(float deltaTime, int &currentMap);
        void GoThroughDoor(sf::Vector2f nextLocation, int nextDirection, int nextMap);

        //Updates//
        void UpdateMovement(float deltaTime);//Updates the player position depending on what he did
        void UpdateAnimation(float deltaTime);//Updates the player sprite depending on what he did
        void OnCollision(sf::Vector2f direction);
        void CheckCollisions(Ground platform);
        void UpdateAnimationDeath(float deltaTime);
        void UnCollide(){m_playerColliding = false;}

        //GetData
        sf::Vector2f GetNextLocation() { return m_nextLocation;}
        float GetTrap() {return m_trap;}


    protected:
        //actions//
        sf::Vector2f m_nextLocation; //when going through doors
        int m_nextMap;//when going through doors
        float timer;
        int m_nextDirection;//when going through doors
        float m_trap;//active when the player touched a trap
        float m_inDoor;//when going through doors
        bool m_playerColliding;



    private:
};

#endif // PLAYER_H
