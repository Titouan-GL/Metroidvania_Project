#ifndef LIVING_ENTITY_H
#define LIVING_ENTITY_H


#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include "Animation.h"
#include "Collider.h"
#include <fstream>
#include <vector>

class Living_Entity
{
    public:
        //primary//
        Living_Entity();
        virtual ~Living_Entity();
        void EntityReset();

        //actions//
        void TurnBack();
        void TurnRight();
        void TurnLeft();
        void Falling();

        //Updates//
        void Draw (sf::RenderWindow& window);

        //GetData
        bool IsAlive(){return (m_life > 0);}
        int GetHealth(){return m_life;}
        int GetMaxHealth(){return m_maxLife;}
        sf::Vector2f GetSize() {return m_mainHitbox.getSize();}
        sf::Vector2f GetPosition() { return m_mainHitbox.getPosition();}
        sf::Vector2f GetDirection();

        Collider GetCollider() { return Collider(m_mainHitbox, 1, true, m_prevLocation);}
        std::vector<Collider> GetColliders();
        std::vector<Collider> GetNextColliders();
        std::vector<Collider> GetDamageDealtHitboxes();
        std::vector<Collider> GetDamageRecievedHitboxes();

        //setData//
        void SetPosition(sf::Vector2f position){ m_mainHitbox.setPosition(position);}

    protected:
        void getHitboxes(std::string txtFileName);

    protected:
        //collisions//
        sf::RectangleShape m_texture;
        sf::RectangleShape m_mainHitbox;
        sf::Vector2f m_prevLocation;
        sf::Vector2f m_pushDirection;
        std::vector<std::vector<std::vector<sf::RectangleShape>>> m_collisionHitboxes;//first layer is the type of animation, 2nd the frame, 3rd every hitboxes (usually only one)
        std::vector<std::vector<std::vector<sf::RectangleShape>>> m_damageTakenHitboxes;
        std::vector<std::vector<std::vector<sf::RectangleShape>>> m_damageDealtHitboxes;

        //animations//
        Animation m_animationIdle;
        Animation m_animationAttack;
        Animation m_animationRun;
        Animation m_animationJump;
        Animation m_animationFall;
        Animation m_animationJumpFall;
        Animation m_animationHurt;
        Animation m_animationDeath;
        Animation m_animationDeathStatic;
        sf::Texture m_textureIdle;
        sf::Texture m_textureAttack;
        sf::Texture m_textureRun;
        sf::Texture m_textureJump;
        sf::Texture m_textureFall;
        sf::Texture m_textureJumpFall;
        sf::Texture m_textureHurt;
        sf::Texture m_textureDeath;
        sf::Texture m_textureDeathStatic;
        unsigned int m_row;
        unsigned int m_frame;
        float m_deathAnimationTime;
        float m_idleTime;//for idle movement
        float m_idleMovementTime;
        float m_textureOffset;//used when the character turns

        //actions//
        sf::Vector2f m_velocity;
        int m_attacking;
        float m_maxAttackCooldown;
        float m_attackCooldown;
        bool m_faceRight;
        bool m_canMove;
        bool m_canJump;
        float m_jumpDuration;
        float m_fallDuration;
        float m_jumpFall;
        float m_jumpChange;

        //attributes//
        int m_life;
        int m_maxLife;
        float m_rotation;
        float m_speed;
        float m_normalSpeed;
        float m_scale;
        float m_attackDuration;
        float m_maxAttackDuration;
        float m_knockback;
        float m_maxKnockback;
        float m_knockbackTime;
        float m_immortalityTime;
        float m_maxImmortalityTime;
        float m_jumpx;
        float m_maxJumpy;//(t-1)^x *y + y derivée : y*x(t-1)^(x-1)
        float m_maxJumpz;//(t-1)^x *y + y derivée : y*x(t-1)^(x-1)
        float m_jumpy;//(t-1)^x *y + y derivée : y*x(t-1)^(x-1)
        float m_jumpz;//(t-1)^x *y + y derivée : y*x(t-1)^(x-1)


    protected:

    private:
};

#endif // LIVING_ENTITY_H
