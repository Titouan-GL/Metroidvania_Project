#include <SFML/Graphics.hpp>
#include <typeinfo>
#include <math.h>
#include "../include/Player.h"

//primary//
Player::Player()
{
    Reset();

    getHitboxes("Hitboxes/MainCharacter/idleKnight.txt");
    getHitboxes("Hitboxes/MainCharacter/runKnight.txt");
    getHitboxes("Hitboxes/MainCharacter/jumpKnight.txt");
    getHitboxes("Hitboxes/MainCharacter/fallKnight.txt");
    getHitboxes("Hitboxes/MainCharacter/jumpFallKnight.txt");
    getHitboxes("Hitboxes/MainCharacter/TwoAttacksKnight.txt");
    getHitboxes("Hitboxes/MainCharacter/hurtKnight.txt");
}

void Player::Reset()
{
    EntityReset();
    m_textureIdle.loadFromFile("Sprites/FreeKnight/Used/_Idle.png");
    m_animationIdle = Animation(m_textureIdle, sf::Vector2u(10, 1), 0.15f);
    m_textureAttack.loadFromFile("Sprites/FreeKnight/Used/_AttackCombo2hit.png");
    m_animationAttack = Animation(m_textureAttack, sf::Vector2u(10, 1), 0.08f);
    m_textureRun.loadFromFile("Sprites/FreeKnight/Used/_Run.png");
    m_animationRun = Animation(m_textureRun, sf::Vector2u(10, 1), 0.08f);
    m_textureJump.loadFromFile("Sprites/FreeKnight/Used/_Jump.png");
    m_animationJump = Animation(m_textureJump, sf::Vector2u(3, 1), 0.15f);
    m_textureFall.loadFromFile("Sprites/FreeKnight/Used/_Fall.png");
    m_animationFall = Animation(m_textureFall, sf::Vector2u(3, 1), 0.15f);
    m_textureJumpFall.loadFromFile("Sprites/FreeKnight/Used/_JumpFallInbetween.png");
    m_animationJumpFall = Animation(m_textureJumpFall, sf::Vector2u(2, 1), 0.15f);
    m_textureHurt.loadFromFile("Sprites/FreeKnight/Used/_Hit.png");
    m_animationHurt = Animation(m_textureHurt, sf::Vector2u(1, 1), 0.5f);
    m_textureDeath.loadFromFile("Sprites/FreeKnight/Used/_Death.png");
    m_animationDeath = Animation(m_textureDeath, sf::Vector2u(10, 1), 0.3f);
    m_textureDeathStatic.loadFromFile("Sprites/FreeKnight/Used/_DeathStatic.png");
    m_animationDeathStatic = Animation(m_textureDeathStatic, sf::Vector2u(1, 1), 0.1f);

    m_speed = 400.0f;
    m_attacking = 0;
    m_scale = 2;
    m_rotation = 0;
    m_texture.setSize(sf::Vector2f(120.0f * m_scale, 80.0f * m_scale));
    m_texture.setPosition(200.0f, 200.0f);
    m_texture.setTexture(&m_textureIdle);
    m_texture.setRotation(m_rotation);
    m_maxLife = 5;
    m_life = 5;
    m_jumpx = 2; //determine how long the player will reach max height
    m_maxJumpy = 500; //determine how quick ils the jump
    m_maxJumpz = 600; //determine how high is the jump
    m_jumpy = 500; //determine how quick ils the jump
    m_jumpz = 400; //determine how high is the jump
    m_jumpChange = 300;

    m_mainHitbox.setSize(sf::Vector2f(120.0f * m_scale, 80.0f * m_scale));
    m_mainHitbox.setPosition(200.0f, 200.0f);
    m_mainHitbox.setRotation(m_rotation);

    m_inDoor = 0;
    m_trap = 0;
    m_textureOffset = 240;
    m_maxKnockback = 0.3;
    m_maxImmortalityTime = 1;
}
Player::~Player()
{

}

//actions//
void Player::Inputs(float deltaTime)
{
    m_prevLocation = m_mainHitbox.getPosition();

    bool playerMovementEnabled = ( m_inDoor <= 0 && m_knockback <= 0);
    if(playerMovementEnabled)
    {
        //MOVEMENTS//
        //horizontal movements//

        m_velocity.x = 0.0f;
        if (m_canMove && playerMovementEnabled)
        {
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                m_velocity.x = -m_speed;
            }

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                m_velocity.x = m_speed;
            }
        }

        //vertical movements//

        //simple jump//
        float delta = sqrt(abs(m_jumpz*m_jumpy*-4))/(2*m_jumpy); // not actually the delta but the result of the full 2nd degree equasion
        if(!m_canJump)
        {
            m_fallDuration += deltaTime;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_canJump) //jump height is approximately 250
        {
            m_canJump = false;
            m_jumpDuration = 1.0f;
            m_jumpFall = 0.0f;
            m_fallDuration = 0.0f;
            m_jumpy = 900;
            m_jumpz = 100;
        }
        else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_jumpy > m_maxJumpy && m_velocity.y < 0)
        {
            m_jumpy -= m_jumpChange *deltaTime;
            m_jumpz += m_jumpChange *deltaTime;
            m_velocity.y = m_jumpy*m_jumpx*pow((m_fallDuration-delta),(m_jumpx-1))*2;//we found out that multiplying by two was better, it would have been fixed with more time

        }
        //falling//
        else if (!m_canJump)
        {
            m_fallDuration += deltaTime;
            m_velocity.y = m_jumpy*m_jumpx*pow((m_fallDuration-delta),(m_jumpx-1));
            /*
            if(m_velocity.y <= -800){
                m_velocity.y = -400.0f;}
            m_jumpDuration = 0.0f;
            if (m_jumpFall < 100.0f)
            {
                m_jumpFall = 50*deltaTime;
            }
        }
        if (m_velocity.y < 1000.0f && !m_canJump)
        {
            m_velocity.y += m_jumpFall;*/
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            m_attackDuration = 0.40;
            m_attacking = 1;
        }
    }
}

void Player::Attacking(float deltaTime)
{
    if(m_attackDuration > 0)
    {
        m_attackDuration -= deltaTime;
        if(m_animationAttack.getCurrentImage() == 4){
            m_animationAttack.SetCurrentImage(5);
            m_attackDuration = 0;
            }
        else if(m_animationAttack.getCurrentImage() == 9){
            m_animationAttack.SetCurrentImage(0);
            m_attackDuration = 0;
            }
    }
    else
    {
        m_attacking = 0;
    }
}

void Player::GetHit()
{
    if(m_immortalityTime <= 0)
    {
        m_animationAttack.SetCurrentImage(0);
        m_attackDuration = 0;
        m_velocity = sf::Vector2f(0,0);
        m_knockback = m_maxKnockback;
        m_immortalityTime = m_maxImmortalityTime;
        m_life -= 1;
    }
}


//interactions//
void Player::Trapped(sf::Vector2f nextLocation)
{
    if (m_trap <= 0)
    {
        m_nextLocation = nextLocation;
        m_trap = 0.7f;
        m_canJump = false;
        m_canMove = false;
    }
}

bool Player::Door(float deltaTime, int &currentMap)
{
    if (m_inDoor > 0)
    {
        m_inDoor -= deltaTime;
        if (m_nextDirection ==1)//going right
        {
            m_velocity.x = m_speed;
        }
        else if (m_nextDirection ==2)//going left
        {
            m_velocity.x = -m_speed;
        }
        if (m_inDoor >= 0.4 && m_inDoor <= 0.5)
        {
            m_mainHitbox.setPosition(m_nextLocation);
            currentMap = m_nextMap;
        }
        if(m_inDoor > 0.3){
            return true;}
        else{
            return false;}
    }
    else
    {
        m_inDoor = 0;
    }
    return false;
}

void Player::GoThroughDoor(sf::Vector2f nextLocation, int nextDirection, int nextMap)
{
    if(m_inDoor <= 0)
    {
        m_attackDuration = 0;
        m_animationAttack.SetCurrentImage(0);
        m_velocity = sf::Vector2f(0.0f, 0.0f);
        m_inDoor = 0.7;
        m_canJump = false;
        m_nextLocation = nextLocation;
        m_nextDirection = nextDirection;
        m_nextMap = nextMap;
    }
}


//Updates//
void Player::UpdateMovement(float deltaTime)
{
    if (!m_playerColliding)
    {
        Falling();
    }
    if (m_velocity.y > 1000) {m_velocity.y = 1000.0;}
    if (m_trap > 0){m_trap -= deltaTime;}
    if (m_attacking > 0){Attacking(deltaTime);}
    if (m_knockback > 0){m_knockback -= deltaTime;}
    else {m_knockback = 0;}
    if (m_immortalityTime > 0){m_immortalityTime -= deltaTime;}
    else {m_immortalityTime = 0;}

    m_mainHitbox.move(m_velocity * deltaTime);
}

void Player::UpdateAnimation(float deltaTime)
{
    if(m_velocity.x != 0)
    {
        if(m_attackDuration <= 0)
        {
            if(m_velocity.x > 0.0f){
                TurnRight();
            }
            else if (m_velocity.x < 0.0f){
                TurnLeft();
            }
        }
    }
    if (m_knockback > 0)
    {
        m_row = 6;
        m_frame = m_animationHurt.getCurrentImage();
        m_animationHurt.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureHurt);
        m_texture.setTextureRect(m_animationHurt.getUvRect());
    }
    else if (m_attacking == 1)
    {
        m_row = 5;
        m_frame = m_animationAttack.getCurrentImage();
        m_animationAttack.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureAttack);
        m_texture.setTextureRect(m_animationAttack.getUvRect());
    }
    else if (m_velocity.y < 0)
    {
        m_row = 2;
        m_frame = m_animationJump.getCurrentImage();
        m_animationJump.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureJump);
        m_texture.setTextureRect(m_animationJump.getUvRect());
    }
    else if (m_velocity.y > 0)
    {
        m_row = 3;
        m_frame = m_animationFall.getCurrentImage();
        m_animationFall.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureFall);
        m_texture.setTextureRect(m_animationFall.getUvRect());
    }
    else if(m_velocity.x == 0)
    {
        m_row = 0;
        m_frame = m_animationIdle.getCurrentImage();
        m_animationIdle.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureIdle);
        m_texture.setTextureRect(m_animationIdle.getUvRect());
    }
    else if(m_velocity.x != 0)
    {
        m_row = 1;
        m_frame = m_animationRun.getCurrentImage();
        m_animationRun.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureRun);
        m_texture.setTextureRect(m_animationRun.getUvRect());
    }
    if(m_knockback > 0.2){
        m_texture.setFillColor(sf::Color::Red);}
    else{
        m_texture.setFillColor(sf::Color::White);}
    m_texture.setPosition(sf::Vector2f(m_mainHitbox.getPosition().x +  + m_textureOffset*(int)(!m_faceRight),m_mainHitbox.getPosition().y));
}

void Player::UpdateAnimationDeath(float deltaTime)
{
    m_texture.setFillColor(sf::Color::White);
    m_animationDeath.Update(0, deltaTime, m_faceRight);
    m_texture.setTexture(&m_textureDeath);
    m_texture.setTextureRect(m_animationDeath.getUvRect());
}

void Player::OnCollision(sf::Vector2f direction)
{
    if (direction.x < 0.0f)//collision on the left
    {
        m_velocity.x = 0.0f;
    }
    else if (direction.x > 0.0f)//collision on the right
    {
        m_velocity.x = 0.0f;
    }

    if (direction.y < 0.0f)//collision on the bottom
    {
        m_velocity.y = 0.0f;
        m_canJump = true;
        m_jumpFall = 0;
    }
    else if (direction.y > 0.0f)//collision on the top
    {
        m_jumpDuration = 0.0f;
        m_fallDuration = 1.1f;
        m_jumpy = m_maxJumpy;
        m_jumpz = m_maxJumpz;

    }
}

void Player::CheckCollisions(Ground platform)
{
    Collider playerCollider = GetCollider();
    std::vector<Collider> playerColliders = GetColliders();
    sf::Vector2f direction(0,0);
    if (platform.GetCollider().CheckCollision(playerCollider))
    {
        for(int j = 0; j < (int)playerColliders.size() ; j++)
        {
            if (platform.GetCollider().CheckCollision(playerColliders[j]))
            {
                //std::cout << playerColliders[0].GetPreviousPosition().x << " " << playerColliders[0].GetPosition().x << std::endl;
                if (platform.GetType() ==1)
                {
                    sf::Vector2f offset = platform.GetCollider().Repel(playerColliders[j], direction);
                    SetPosition(GetPosition()-offset);
                    OnCollision(direction);
                    m_playerColliding = true;
                }
                if (platform.GetType() ==2)
                {
                    GoThroughDoor(platform.GetNextLocation(), platform.GetNextDirection(), platform.GetNextMap());
                }
                if (platform.GetType() ==3)
                {
                    GetHit();
                    Trapped(platform.GetNextLocation());
                }
            }
        }
    }
}
