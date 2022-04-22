#ifndef ENNEMYBOSS1_H
#define ENNEMYBOSS1_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "../Animation.h"
#include "../Collider.h"
#include "../Living_Entity.h"
#include "../Player.h"
#include "../Ground.h"
#include <fstream>
#include <vector>

class EnnemyBoss1 : public Living_Entity
{
    public:
        //primary//
        EnnemyBoss1(float x = 0, float y = 0);
        virtual ~EnnemyBoss1();
        /*
        Reset allow the entity to go back to it's original state if the game is reset
        */
        void Reset(float x, float y);

        //actions//
        /*
        behavior dictates the behavior of the entity and it's IA
        */
        void Behavior(float deltaTime);

        /*
        gethit defines what to do if the entity is damaged
        */
        void GetHit(sf::Vector2f direction = sf::Vector2f(0,0), int damage = 0);

        /*
        Attacking updates the current Attack of the entity
        */
        void Attacking(float deltaTime);

        /*
        change phase modifies somes attributes of the boss
        */
        void ChangePhase();

        /*
        jump updates the current jump of the entity
        */
        void Jump();

        //Updates//
        /*
        Updates movement updates the movement depending on the time
        */
        void UpdateMovement(float deltaTime);

        /*
        Updates movement animation the movement depending on the time
        */
        void UpdateAnimation(float deltaTime);

        /*
        OnCollision dictates what must be done if a collision is occuring and varies depending on the collision
        */
        void OnCollision(sf::Vector2f direction);

        /*
        CollisionCheck check if any collision with the environment is occuring and if so, call the OnCollision function
        */
        void CollisionCheck(Ground platform);

        /*
        DamageCheck check if any collision with the player is occuring and if so, call the gethit function if the player is attacking the entity
        */
        void DamageCheck(Player &player, Collider &playerCollider, std::vector<Collider> &playerHitbox, std::vector<Collider> &playerAttacks);

        //setData//
        /*
        set the distance between the player and the entity
        */
        void SetDistance(sf::Vector2f distance) {m_distanceToPlayer = distance;}

    protected:
        bool m_EnnemyColliding;
        Animation m_animationAttack2;
        Animation m_animationBackward;
        sf::Texture m_textureAttack2;
        sf::Texture m_textureBackward;
        sf::Vector2f m_distanceToPlayer;
        bool m_playerDetected;
        int m_attack1Counter;
        int m_attack2Counter;
        int m_attack3Counter;
        int m_phase;
        float timer;


    private:
};

#endif // ENNEMYBOSS1_H
