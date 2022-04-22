
#include "../../include/Ennemies/EnnemyBoss1.h"
#include <stdlib.h>

//primary//
EnnemyBoss1::EnnemyBoss1(float x, float y)
{
    Reset(x, y);

    getHitboxes("Hitboxes/Ennemies/Boss1/Walk.txt");
    getHitboxes("Hitboxes/Ennemies/Boss1/Idle.txt");
    getHitboxes("Hitboxes/Ennemies/Boss1/Attack1.txt");
    getHitboxes("Hitboxes/Ennemies/Boss1/Attack2.txt");
    getHitboxes("Hitboxes/Ennemies/Boss1/Backward.txt");
}

EnnemyBoss1::~EnnemyBoss1()
{
    //dtor
}

void EnnemyBoss1::Reset(float x, float y)
{
    EntityReset();
    m_textureIdle.loadFromFile("Sprites/Ennemies/Boss1/Idle.png");
    m_animationIdle = Animation(m_textureIdle, sf::Vector2u(6, 1), 0.15f);
    m_textureAttack.loadFromFile("Sprites/Ennemies/Boss1/Attack1.png");
    m_animationAttack = Animation(m_textureAttack, sf::Vector2u(12, 1), 0.10);
    m_textureRun.loadFromFile("Sprites/Ennemies/Boss1/Walk.png");
    m_animationRun = Animation(m_textureRun, sf::Vector2u(16, 1), 0.08f);
    m_textureDeath.loadFromFile("Sprites/Ennemies/Boss1/Death.png");
    m_animationDeath = Animation(m_textureDeath, sf::Vector2u(7, 1), 0.4);
    m_textureDeathStatic.loadFromFile("Sprites/Ennemies/Boss1/Death_Static.png");
    m_animationDeathStatic = Animation(m_textureDeathStatic, sf::Vector2u(1, 1), 0.15f);
    m_textureAttack2.loadFromFile("Sprites/Ennemies/Boss1/Attack2.png");
    m_animationAttack2 = Animation(m_textureAttack2, sf::Vector2u(8, 1), 0.13);
    m_textureBackward.loadFromFile("Sprites/Ennemies/Boss1/Backward.png");
    m_animationBackward = Animation(m_textureBackward, sf::Vector2u(16, 1), 0.10);

    m_speed = 150.0f;
    m_scale = 1;
    m_phase = 1;
    m_maxLife = 100;
    m_life = 100;
    m_texture.setSize(sf::Vector2f(380.0f * m_scale, 310.0f * m_scale));
    m_texture.setPosition(x, y);
    m_texture.setTexture(&m_textureIdle);
    m_texture.setRotation(m_rotation);
    m_jumpx = 2;
    m_maxJumpy = 500; //determine how quick ils the jump
    m_maxJumpz = 600; //determine how high is the jump
    m_jumpy = 500; //determine how quick ils the jump
    m_jumpz = 600; //determine how high is the jump

    m_mainHitbox.setSize(sf::Vector2f(380.0f * m_scale, 310.0f * m_scale));
    m_mainHitbox.setPosition(x,y);
    m_mainHitbox.setRotation(m_rotation);

    m_textureOffset = 380;
    m_attacking = 0;
    m_knockback = 0;
    m_maxAttackCooldown = 1.5;
    m_attackCooldown = 0;
    m_maxAttackDuration = 1.2;
    m_maxImmortalityTime = 0.3;

    m_attack1Counter = 0;
    m_attack1Counter = 0;
    m_attack1Counter = 0;
}
//actions//
void EnnemyBoss1::Behavior(float deltaTime)
{
    m_prevLocation = m_mainHitbox.getPosition();
    if(IsAlive())
    {
        float delta = sqrt(abs(m_jumpz*m_jumpy*-4))/(2*m_jumpy);
        m_velocity.x = 0.0f;
        if(abs(m_distanceToPlayer.x) < 800  && abs(m_distanceToPlayer.y) < 2000 && !m_playerDetected){ //player detection
            m_playerDetected = true;
            m_speed = 300;}

        if(!m_canJump){
            m_fallDuration += deltaTime;
            m_velocity.y = m_jumpy*m_jumpx*pow((m_fallDuration-delta),(m_jumpx-1));
            timer += deltaTime;
        }

        if(m_playerDetected){
            int randomAttack;
            if(m_attackCooldown <= 0 && m_attacking == 0){
                randomAttack = rand() % 5;}
            if(m_phase == 1)
            {
                if(m_attacking == 1){
                    m_speed = 0;}
                else if(m_attacking == 2){
                    if(m_velocity.y == 0){m_speed = 0;}
                }
                else if(m_attacking == 3){
                    if(m_attackDuration >= 1.2){m_speed = -100;}
                    else if(m_attackDuration > 0.2){m_speed = 700;}
                    else if(m_attackDuration > 0){m_speed = 0;}
                }
                else if(m_distanceToPlayer.x < -100 && m_attackDuration <= 0 && m_faceRight){//change direction
                    TurnLeft();}
                else if(m_distanceToPlayer.x > 100 && m_attackDuration <= 0 && !m_faceRight){
                    TurnRight();}
                if(m_attacking == 0)
                {

                    if(abs(m_distanceToPlayer.x) < 180 && abs(m_distanceToPlayer.y) < 200 && m_attackCooldown <= 0 && m_attack1Counter < 1 && abs(m_distanceToPlayer.y) < 500){//Regular attack
                        m_attackCooldown = m_maxAttackCooldown;
                        m_attackDuration = m_maxAttackDuration;
                        m_speed = 0;
                        m_attack2Counter = 0;
                        m_attack3Counter = 0;
                        m_attack1Counter ++;
                        m_attacking = 1;}
                    else if(m_attackCooldown <= 0 && m_attack2Counter < 2 && randomAttack%3 == 0 && abs(m_distanceToPlayer.y) < 500 && m_canJump){//Jump Attack
                        m_attackCooldown = 1.4;
                        m_attackDuration = 1.3;
                        m_canJump = false;
                        m_jumpy = 1400;
                        m_jumpz = 600;
                        m_speed = abs(m_distanceToPlayer.x)/1.3;
                        m_attack1Counter = 0;
                        m_attack2Counter ++;
                        m_attacking = 2;}
                    else if(m_attackCooldown <= 0 && m_attack3Counter < 2 && randomAttack%3 == 1 && abs(m_distanceToPlayer.y) < 500){//Dash Attack
                        m_attackCooldown = 2;
                        m_attackDuration = 1.7;
                        m_speed = -100;
                        m_attack1Counter = 0;
                        m_attack3Counter ++;
                        m_attacking = 3;}
                    else if(m_attackCooldown <= 0 && m_attack2Counter < 2 && m_attack3Counter < 2){
                        m_attack2Counter = 2;
                        m_attack3Counter = 2;
                        m_speed = 400;
                    }
                    else if(m_attack1Counter >= 1){
                        m_attack2Counter --;
                        m_attack3Counter --;
                    }
                    else{
                        m_attack1Counter = 0;
                        m_speed = 400;}
                }
                if(m_life < 40){
                    ChangePhase();}
            }
            if(m_phase == 2)
            {
                if(m_attacking == 1){
                    m_speed = 0;}
                else if(m_attacking == 2){
                    if(m_velocity.y == 0){m_speed = 0;}
                }
                else if(m_attacking == 3){
                    if(m_attackDuration >= 1){m_speed = -100;}
                    else if(m_attackDuration > 0.2){m_speed = 900;}
                    else if(m_attackDuration > 0){m_speed = 0;}
                }
                else if(m_distanceToPlayer.x < -10 && m_attackDuration <= 0 && m_faceRight){//change direction
                    TurnLeft();}
                else if(m_distanceToPlayer.x > 10 && m_attackDuration <= 0 && !m_faceRight){
                    TurnRight();}

                if(m_attacking == 0)
                {

                    if(abs(m_distanceToPlayer.x) < 180 && abs(m_distanceToPlayer.y) < 200 && m_attackCooldown <= 0 && m_attack1Counter < 2 && abs(m_distanceToPlayer.y) < 500){//Regular attack
                        m_attackCooldown = 1.2;
                        m_attackDuration = 1;
                        m_speed = 0;
                        m_attack2Counter = 0;
                        m_attack3Counter = 0;
                        m_attack1Counter ++;
                        m_attacking = 1;}
                    else if(m_attackCooldown <= 0 && m_attack2Counter < 2 && randomAttack%3 == 0 && abs(m_distanceToPlayer.y) < 500 && m_canJump){//Jump Attack
                        m_attackCooldown = 1.2;
                        m_attackDuration = 1.1;
                        m_canJump = false;
                        m_jumpy = 2000;
                        m_jumpz = 600;
                        m_speed = abs(m_distanceToPlayer.x)/1.3;
                        m_attack1Counter = 0;
                        m_attack2Counter ++;
                        m_attacking = 2;}
                    else if(m_attackCooldown <= 0 && m_attack3Counter < 2 && randomAttack%3 == 1 && abs(m_distanceToPlayer.y) < 500){//Dash Attack
                        m_attackCooldown = 1.6;
                        m_attackDuration = 1.4;
                        m_speed = -100;
                        m_attack1Counter = 0;
                        m_attack3Counter ++;
                        m_attacking = 3;}
                    else if(m_attackCooldown <= 0 && m_attack2Counter < 2 && m_attack3Counter < 2){
                        m_attack2Counter = 2;
                        m_attack3Counter = 2;
                        m_speed = 500;
                    }
                    else if(m_attack1Counter >= 1){
                        m_attack2Counter --;
                        m_attack3Counter --;
                    }
                    else{
                        m_attack1Counter = 0;
                        m_speed = 500;}
                }
            }
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
    /*if (m_jumpFall < 100 && m_phase == 1 )
    {
        m_jumpFall += 7*deltaTime;
    }
    if (m_jumpFall < 100 && m_phase == 2)
    {
        m_jumpFall += 10*deltaTime;
    }
    if (m_velocity.y < 1000.0f)
    {
        m_velocity.y += m_jumpFall;
    }*/
}

void EnnemyBoss1::GetHit(sf::Vector2f direction, int damage)
{
    if(m_immortalityTime <= 0 && IsAlive()){
        m_life -= damage;
        m_pushDirection = direction;
        m_immortalityTime = m_maxImmortalityTime;
        m_knockbackTime = 0.1;}
}

void EnnemyBoss1::Attacking(float deltaTime)
{
    if(m_attackDuration > 0)
    {
        m_attackDuration -= deltaTime;
    }
    else{
        m_animationAttack.SetCurrentImage(0);
        m_animationAttack2.SetCurrentImage(0);
    }
    if(m_attackCooldown > 0)
    {
        m_attackCooldown -= deltaTime;
    }
    else
    {
        m_attacking = 0;
    }
}

void EnnemyBoss1::ChangePhase(){
    m_animationAttack = Animation(m_textureAttack, sf::Vector2u(12, 1), 0.08);
    m_animationAttack2 = Animation(m_textureAttack2, sf::Vector2u(8, 1), 0.10);
    m_phase = 2;
}


//Updates//
void EnnemyBoss1::UpdateMovement(float deltaTime)
{
    if (!m_EnnemyColliding)
    {
        Falling();
    }
    m_EnnemyColliding = false;
    if (m_knockbackTime > 0){
        m_knockbackTime -= deltaTime;}
    else {
        m_knockbackTime = 0;}

    if (m_immortalityTime > 0){
        m_immortalityTime -= deltaTime;}
    else {
        m_immortalityTime = 0;}
    if(IsAlive())
    {
        if (m_attacking > 0){
            Attacking(deltaTime);}
        if(m_knockbackTime > 0){
            m_velocity.x -= m_knockback*m_pushDirection.x;}
    }
    else{
        m_velocity.x = 0;}
    m_mainHitbox.move(m_velocity * deltaTime);
}

void EnnemyBoss1::UpdateAnimation(float deltaTime)
{
    if(m_velocity.x != 0)
    {
        if(m_animationAttack.getCurrentImage() == 0)
        {
            if(!m_faceRight && m_texture.getScale().x > 0){
                m_texture.scale(sf::Vector2f(-1.f, 1.f));
                m_faceRight = true;}

            else if(m_faceRight && m_texture.getScale().x < 0){
                m_texture.scale(sf::Vector2f(-1.f, 1.f));
                m_faceRight = false;}
        }
    }
    if(!IsAlive() && m_animationDeath.getCurrentImage() < 6)
    {
        m_animationDeath.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureDeath);
        m_texture.setTextureRect(m_animationDeath.getUvRect());
    }
    else if(!IsAlive() )
    {
        m_animationDeathStatic.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureDeathStatic);
        m_texture.setTextureRect(m_animationDeathStatic.getUvRect());
    }
    else if (m_attacking == 1)
    {
        m_row = 2;
        m_frame = m_animationAttack.getCurrentImage();
        m_animationAttack.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureAttack);
        m_texture.setTextureRect(m_animationAttack.getUvRect());
    }
    else if (m_attacking == 3 && m_speed < 0)
    {
        m_row = 4;
        m_frame = m_animationBackward.getCurrentImage();
        m_animationBackward.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureBackward);
        m_texture.setTextureRect(m_animationBackward.getUvRect());
    }
    else if (m_attacking == 3 && m_speed > 0)
    {
        m_row = 3;
        m_frame = m_animationAttack2.getCurrentImage();
        m_animationAttack2.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureAttack2);
        m_texture.setTextureRect(m_animationAttack2.getUvRect());
    }
    else if(m_velocity.x == 0 || m_attacking == 2)
    {
        m_row = 1;
        m_frame = m_animationIdle.getCurrentImage();
        m_animationIdle.Update(0, deltaTime, m_faceRight);
        m_texture.setTexture(&m_textureIdle);
        m_texture.setTextureRect(m_animationIdle.getUvRect());
    }
    else if(m_velocity.x != 0)
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

void EnnemyBoss1::OnCollision(sf::Vector2f direction)
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
        m_canJump = true;
        m_fallDuration = 0;
        m_velocity.y = 0.0f;
        m_jumpFall = 0.0f;
    }
    else if (direction.y > 0.0f)//collision on the top
    {
        m_velocity.y = 0.0f;
        m_jumpDuration = 0.0f;
    }
}

void EnnemyBoss1::CollisionCheck(Ground platform)
{
    sf::Vector2f direction(0,0);
    Collider EnnemyCollider = GetCollider();
    std::vector<Collider> EnnemyColliders = GetColliders();
    std::vector<Collider> EnnemyNextColliders = GetNextColliders();
    if (platform.GetCollider().CheckCollision(EnnemyCollider))
    {
        bool EnnemyOnEdge = false;
        for(int j = 0; j < (int)EnnemyColliders.size() ; j++)
        {
            if (platform.GetCollider().CheckCollision(EnnemyColliders[j]))
            {
                //std::cout << playerColliders[0].GetPreviousPosition().x << " " << playerColliders[0].GetPosition().x << std::endl;
                if (platform.GetType() ==1)
                {
                    sf::Vector2f offset = platform.GetCollider().Repel(EnnemyColliders[j], direction);
                    SetPosition(GetPosition()-offset);
                    m_EnnemyColliding = true;
                    OnCollision(direction);
                    if (!platform.GetCollider().CheckCollision(EnnemyNextColliders[j]))
                    {
                        EnnemyOnEdge = true;
                    }
                }
                if (platform.GetType() ==3)
                {
                    GetHit(sf::Vector2f(0,0), GetHealth());
                }
            }
        }
        if (EnnemyOnEdge && !m_playerDetected)
        {
            TurnBack();
        }
    }
}

void EnnemyBoss1::DamageCheck(Player &player, Collider &playerCollider, std::vector<Collider> &playerHitbox, std::vector<Collider> &playerAttacks)
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




