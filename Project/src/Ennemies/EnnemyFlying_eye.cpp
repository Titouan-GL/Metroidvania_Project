
#include "../../include/Ennemies/EnnemyFlying_eye.h"
#include <stdlib.h>

//primary//
EnnemyFlying_eye::EnnemyFlying_eye(float x, float y)
{

    Reset(x, y);

    getHitboxes("Hitboxes/Ennemies/Flying_eye/Flight.txt");
    getHitboxes("Hitboxes/Ennemies/Flying_eye/Attack.txt");
}

EnnemyFlying_eye::~EnnemyFlying_eye()
{
    //dtor
}

void EnnemyFlying_eye::Reset(float x, float y)
{
    EntityReset();
    m_textureAttack.loadFromFile("Sprites/Ennemies/Flying_eye/Attack.png");
    m_animationAttack = Animation(m_textureAttack, sf::Vector2u(8, 1), 0.10);
    m_textureRun.loadFromFile("Sprites/Ennemies/Flying_eye/Flight.png");
    m_animationRun = Animation(m_textureRun, sf::Vector2u(8, 1), 0.08f);
    m_textureDeath.loadFromFile("Sprites/Ennemies/Flying_eye/Death.png");
    m_animationDeath = Animation(m_textureDeath, sf::Vector2u(4, 1), 0.2);
    m_textureDeathStatic.loadFromFile("Sprites/Ennemies/Flying_eye/DeathStatic.png");
    m_animationDeathStatic = Animation(m_textureDeathStatic, sf::Vector2u(1, 1), 0.15f);
    m_normalSpeed = 93.0f*  (1+(rand() % 4)/12.0);
    m_speed = m_normalSpeed;
    m_scale = 2;
    m_maxLife = 20;
    m_life = 20;
    m_texture.setSize(sf::Vector2f(150.0f * m_scale, 150.0f * m_scale));
    m_texture.setPosition(x, y);
    m_texture.setTexture(&m_textureIdle);
    m_texture.setRotation(m_rotation);

    m_mainHitbox.setSize(sf::Vector2f(150.0f * m_scale, 150.0f * m_scale));
    m_mainHitbox.setPosition(x, y);
    m_mainHitbox.setRotation(m_rotation);

    m_textureOffset = 300;
    m_knockback = 600;
    m_maxAttackCooldown = 1;
    m_attackCooldown = m_maxAttackCooldown;
    m_maxImmortalityTime = 0.3;
    m_idleTime = 0;
    m_idleMovementTime = 3;
}

//actions//
void EnnemyFlying_eye::Behavior(float deltaTime)
{
    m_prevLocation = m_mainHitbox.getPosition();
    m_velocity.x = 0.0f;
    m_velocity.y = 0.0f;
    if(IsAlive())
    {
        if(abs(m_distanceToPlayer.x) < 300  && abs(m_distanceToPlayer.y) < 300){ //player detection
            m_playerDetected = true;
            m_speed = m_normalSpeed*1.5;}
        else if(abs(m_distanceToPlayer.x) > 600 || abs(m_distanceToPlayer.y) > 800){//cease player detection
            m_playerDetected = false;
            m_speed = m_normalSpeed;}

        if(m_playerDetected){
            if(m_distanceToPlayer.x < -10 && m_attackDuration <= 0){
                TurnLeft();}
            else if(m_distanceToPlayer.x > 10 && m_attackDuration <= 0){
                TurnRight();}
            if(abs(m_distanceToPlayer.x) < 110 && abs(m_distanceToPlayer.y) < 100 && m_attackCooldown <= 0){
                m_attackCooldown = m_maxAttackCooldown;
                m_attackDuration = 0.8;
                m_attacking = 1;}
            if(m_attacking == 1){
                m_speed = 0;}
            else{
            m_speed = m_normalSpeed*1.5;}
        }
        if(m_playerDetected)
        {
            if(m_distanceToPlayer.y < 0)
            {
                m_velocity.y -= m_speed;
            }
            else if(m_distanceToPlayer.y > 0)
            {
                m_velocity.y += m_speed;
            }
            if (m_faceRight)
            {
                m_velocity.x += m_speed;
            }
            else
            {
                m_velocity.x -= m_speed;
            }
        }
        else {
            if (m_idleMovementTime > 0){
                if (m_faceRight)
                {
                    m_velocity.x += m_speed;
                }
                else
                {
                    m_velocity.x -= m_speed;
                }
            }
            else if (m_idleMovementTime <= 0){
                m_idleMovementTime = 3;
                TurnBack();}
        }
    }
    else if(!m_canJump){
        m_velocity.x = 0.0f;
        m_velocity.y = 1000.0f;
    }
    else{
        m_velocity.y = 0;
    }
}

void EnnemyFlying_eye::GetHit(sf::Vector2f direction, int damage)
{
    if(m_immortalityTime <= 0 && IsAlive()){
        m_life -= damage;
        m_pushDirection = direction;
        m_immortalityTime = m_maxImmortalityTime;
        m_knockbackTime = 0.1;}
}

void EnnemyFlying_eye::Attacking(float deltaTime)
{
    if(m_attackDuration > 0)
    {
        m_attackDuration -= deltaTime;
    }
    else
    {
        m_attacking = 0;
        m_animationAttack.SetCurrentImage(0);
    }
}

//Updates//
void EnnemyFlying_eye::UpdateMovement(float deltaTime)
{
    if (m_knockbackTime > 0){
        m_knockbackTime -= deltaTime;}
    else {
        m_knockbackTime = 0;}

    if (m_immortalityTime > 0){
        m_immortalityTime -= deltaTime;}
    else {
        m_immortalityTime = 0;}

    if (m_idleMovementTime > 0){
        m_idleMovementTime -= deltaTime;}
    if (m_idleTime > 0){
        m_idleTime -= deltaTime;}

    if(IsAlive())
    {
        if(m_attackCooldown >= 0){
            m_attackCooldown -= deltaTime;}
        if (m_attacking > 0){
            Attacking(deltaTime);}
        if(m_knockbackTime > 0){
            m_velocity.x -= m_knockback*m_pushDirection.x;}
    }
    m_mainHitbox.move(m_velocity * deltaTime);
}

void EnnemyFlying_eye::UpdateAnimation(float deltaTime)
{
    if(m_velocity.x != 0)
    {
        if(m_animationAttack.getCurrentImage() == 0 || m_animationAttack.getCurrentImage() == 5 )
        {
            if(!m_faceRight && m_texture.getScale().x > 0){
                m_texture.scale(sf::Vector2f(-1.f, 1.f));
                m_faceRight = true;}

            else if(m_faceRight && m_texture.getScale().x < 0){
                m_texture.scale(sf::Vector2f(-1.f, 1.f));
                m_faceRight = false;}
        }
    }
    if(!IsAlive() && m_animationDeath.getCurrentImage() < 3)
    {
        m_animationDeath.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureDeath);
        m_texture.setTextureRect(m_animationDeath.getUvRect());
    }
    else if(!IsAlive())
    {
        m_animationDeathStatic.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureDeathStatic);
        m_texture.setTextureRect(m_animationDeathStatic.getUvRect());
    }
    else if (m_attacking == 1)
    {
        m_row = 1;
        m_frame = m_animationAttack.getCurrentImage();
        m_animationAttack.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureAttack);
        m_texture.setTextureRect(m_animationAttack.getUvRect());
    }
    else
    {
        m_row = 0;
        m_frame = m_animationRun.getCurrentImage();
        m_animationRun.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureRun);
        m_texture.setTextureRect(m_animationRun.getUvRect());
    }
    if(m_knockbackTime > 0){
        m_texture.setFillColor(sf::Color::Red);}
    else{
        m_texture.setFillColor(sf::Color::White);}
    m_texture.setPosition(sf::Vector2f(m_mainHitbox.getPosition().x + m_textureOffset*(int)(!m_faceRight),m_mainHitbox.getPosition().y));
}

void EnnemyFlying_eye::OnCollision(sf::Vector2f direction)
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
    }
    else if (direction.y > 0.0f)//collision on the top
    {
        m_velocity.y = 0.0f;
    }
}


void EnnemyFlying_eye::CollisionCheck(Ground platform)
{
    sf::Vector2f direction(0,0);
    Collider EnnemyCollider = GetCollider();
    std::vector<Collider> EnnemyColliders = GetColliders();

    if (platform.GetCollider().CheckCollision(EnnemyCollider))
    {
        bool EnnemyColliding = false;
        for(int j = 0; j < (int)EnnemyColliders.size() ; j++)
        {
            if (platform.GetCollider().CheckCollision(EnnemyColliders[j]))
            {
                //std::cout << playerColliders[0].GetPreviousPosition().x << " " << playerColliders[0].GetPosition().x << std::endl;
                if (platform.GetType() ==1)
                {
                    sf::Vector2f offset = platform.GetCollider().Repel(EnnemyColliders[j], direction);
                    SetPosition(GetPosition()-offset);
                    EnnemyColliding = true;
                    OnCollision(direction);
                }
                if (platform.GetType() ==3)
                {
                    GetHit(sf::Vector2f(0,0), GetHealth());
                }
            }
        }
        if (!EnnemyColliding)
            {
                Falling();
            }
    }
}

void EnnemyFlying_eye::DamageCheck(Player &player, Collider &playerCollider, std::vector<Collider> &playerHitbox, std::vector<Collider> &playerAttacks)
{
    if(IsAlive())
    {
        Collider EnnemyCollider = GetCollider();
        std::vector<Collider> EnnemyAttacks = GetDamageDealtHitboxes();
        std::vector<Collider> EnnemyHitbox = GetDamageRecievedHitboxes();

        if(EnnemyCollider.CheckCollision(playerCollider))
        {
            for(int i = 0; i < (int)EnnemyAttacks.size() ; i++)
            {
                for(int j = 0 ; j < (int)playerHitbox.size(); j++)
                {
                    if(EnnemyAttacks[i].CheckCollision(playerHitbox[j]))
                    {
                        player.GetHit();
                    }
                }
            }
            for(int i = 0; i < (int)EnnemyHitbox.size() ; i++)
            {
                for(int j = 0 ; j < (int)playerAttacks.size(); j++)
                {
                    if(EnnemyHitbox[i].CheckCollision(playerAttacks[j]))
                    {
                        GetHit(player.GetDirection(), 4);
                    }
                }
            }
        }
    }
}


