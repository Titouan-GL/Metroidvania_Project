#include "../include/Living_Entity.h"

//primary//
Living_Entity::Living_Entity()
{
    m_attacking = 0;
    m_rotation = 0;
    m_row = 0;
    m_frame = 0;
    m_faceRight = true;
    m_canJump = true;

    m_jumpFall = 100;
    m_attacking = 0;
    m_attackDuration = 0;
    m_canMove = true;
    m_knockbackTime = 0;
    m_immortalityTime = 0;

    m_pushDirection = sf::Vector2f(0,0);
}

Living_Entity::~Living_Entity()
{
    //dtor
}

void Living_Entity::EntityReset()
{
    m_attacking = 0;
    m_rotation = 0;
    m_row = 0;
    m_frame = 0;
    m_canJump = true;

    m_jumpFall = 100;
    m_attacking = 0;
    m_attackDuration = 0;
    m_canMove = true;
    m_knockbackTime = 0;
    m_immortalityTime = 0;

    m_pushDirection = sf::Vector2f(0,0);
}

//actions//
void Living_Entity::TurnRight()
{
    if(!m_faceRight){
        m_texture.scale(sf::Vector2f(-1.f, 1.f));}
    m_faceRight = true;
}

void Living_Entity::TurnLeft()
{
    if(m_faceRight){
        m_texture.scale(sf::Vector2f(-1.f, 1.f));}
    m_faceRight = false;
}

void Living_Entity::TurnBack()
{
    if(!m_faceRight){
        TurnRight();
    }
    else if (m_faceRight){
        TurnLeft();
    }
}

void Living_Entity::Falling()
{
    if(m_canJump){
        m_jumpy = m_maxJumpy;
        m_jumpz = m_maxJumpz;
        m_fallDuration = sqrt(abs(m_jumpz*m_jumpy*-4))/(2*m_jumpy);
    }
    m_canJump = false;
}

//Updates//
void Living_Entity::Draw(sf::RenderWindow& window)
{
    /*m_mainHitbox.setFillColor(sf::Color::Cyan);
    window.draw(m_mainHitbox);
    for(int i = 0; i < (int)m_collisionHitboxes[1][m_animationRun.getCurrentImage()].size(); i++)
    {
        sf::RectangleShape drawnHitbox;
        drawnHitbox.setSize(m_collisionHitboxes[m_row][m_frame][i].getSize());
        drawnHitbox.setPosition(m_collisionHitboxes[m_row][m_frame][i].getPosition() + m_mainHitbox.getPosition());
        drawnHitbox.setRotation(m_collisionHitboxes[m_row][m_frame][i].getRotation());
        if(!m_faceRight)
        {
            drawnHitbox.setPosition(m_mainHitbox.getPosition().x - m_collisionHitboxes[m_row][m_frame][i].getPosition().x + m_textureOffset -drawnHitbox.getSize().x, drawnHitbox.getPosition().y);
        }
        window.draw(drawnHitbox);
    }//*/
    window.draw(m_texture);

}

//get data//
sf::Vector2f Living_Entity::GetDirection()
{
    sf::Vector2f returning(0,0);
    if (m_faceRight){
        returning.x = -1;}
    if (!m_faceRight){
        returning.x = 1;}
    return returning;
}




std::vector<Collider> Living_Entity::GetColliders()
{
    std::vector<Collider> returning;
    for(int i = 0; i < (int)m_collisionHitboxes[m_row][m_frame].size(); i++)
    {
        sf::RectangleShape hitbox;
        sf::Vector2f prevLoc = m_prevLocation+m_collisionHitboxes[m_row][m_frame][i].getPosition();
        hitbox.setSize(m_collisionHitboxes[m_row][m_frame][i].getSize());
        hitbox.setPosition(m_collisionHitboxes[m_row][m_frame][i].getPosition() + m_mainHitbox.getPosition());
        hitbox.setRotation(m_collisionHitboxes[m_row][m_frame][i].getRotation());
        if(!m_faceRight)
        {
            hitbox.setPosition(m_mainHitbox.getPosition().x - (m_collisionHitboxes[m_row][m_frame][i].getPosition().x) + m_textureOffset - hitbox.getSize().x, hitbox.getPosition().y);
            prevLoc.x = m_prevLocation.x - m_collisionHitboxes[m_row][m_frame][i].getPosition().x +m_textureOffset -hitbox.getSize().x;
        }
        returning.push_back(Collider(hitbox, 1, true, prevLoc));
    }
    return returning;
}

std::vector<Collider> Living_Entity::GetNextColliders()
{
    std::vector<Collider> returning;
    for(int i = 0; i < (int)m_collisionHitboxes[m_row][m_frame].size(); i++)
    {
        sf::RectangleShape hitbox;
        sf::Vector2f prevLoc = m_prevLocation+m_collisionHitboxes[m_row][m_frame][i].getPosition();
        hitbox.setSize(m_collisionHitboxes[m_row][m_frame][i].getSize());
        hitbox.setRotation(m_collisionHitboxes[m_row][m_frame][i].getRotation());
        hitbox.setPosition(m_collisionHitboxes[m_row][m_frame][i].getPosition() + m_mainHitbox.getPosition());
        if(m_faceRight)
        {
            hitbox.setPosition(hitbox.getPosition().x + hitbox.getSize().x, hitbox.getPosition().y);
        }
        else
        {
            hitbox.setPosition(m_mainHitbox.getPosition().x - (m_collisionHitboxes[m_row][m_frame][i].getPosition().x) + m_textureOffset - hitbox.getSize().x*2, hitbox.getPosition().y);
            prevLoc.x = m_prevLocation.x - m_collisionHitboxes[m_row][m_frame][i].getPosition().x +m_textureOffset -hitbox.getSize().x;
        }
        returning.push_back(Collider(hitbox, 1, true, prevLoc));
    }
    return returning;
}

std::vector<Collider> Living_Entity::GetDamageDealtHitboxes()
{
    std::vector<Collider> returning;
    for(int i = 0; i < (int)m_damageDealtHitboxes[m_row][m_frame].size(); i++)
    {
        sf::RectangleShape hitbox;
        hitbox.setSize(m_damageDealtHitboxes[m_row][m_frame][i].getSize());
        hitbox.setPosition(m_damageDealtHitboxes[m_row][m_frame][i].getPosition() + m_mainHitbox.getPosition());
        hitbox.setRotation(m_damageDealtHitboxes[m_row][m_frame][i].getRotation());
        if(!m_faceRight)
        {
            hitbox.setPosition(m_mainHitbox.getPosition().x - (m_damageDealtHitboxes[m_row][m_frame][i].getPosition().x) + m_textureOffset - hitbox.getSize().x, hitbox.getPosition().y);
        }
        returning.push_back(Collider(hitbox, 3));
    }
    return returning;
}

std::vector<Collider> Living_Entity::GetDamageRecievedHitboxes()
{
    std::vector<Collider> returning;
    for(int i = 0; i < (int)m_damageTakenHitboxes[m_row][m_frame].size(); i++)
    {
        sf::RectangleShape hitbox;
        hitbox.setSize(m_damageTakenHitboxes[m_row][m_frame][i].getSize());
        hitbox.setPosition(m_damageTakenHitboxes[m_row][m_frame][i].getPosition() + m_mainHitbox.getPosition());
        hitbox.setRotation(m_damageTakenHitboxes[m_row][m_frame][i].getRotation());
        if(!m_faceRight)
        {
            hitbox.setPosition(m_mainHitbox.getPosition().x - (m_damageTakenHitboxes[m_row][m_frame][i].getPosition().x) + m_textureOffset - hitbox.getSize().x, hitbox.getPosition().y);
        }
        returning.push_back(Collider(hitbox, 3));
    }
    return returning;
}

void Living_Entity::getHitboxes(std::string txtFileName)
{
    std::ifstream txtFile(txtFileName);
    std::vector<std::vector<sf::RectangleShape>> collisionHitboxes;
    std::vector<std::vector<sf::RectangleShape>> damageTakenHitboxes;
    std::vector<std::vector<sf::RectangleShape>> damageDealtHitboxes;
    std::vector<sf::RectangleShape> currentCollisionHitboxes;
    std::vector<sf::RectangleShape> currentDamageTakenHitboxes;
    std::vector<sf::RectangleShape> currentDamageDealtHitboxes;
    char x = 0;
    int level = 0;
    int lign = 0;
    int column = 0;
    int number = 0;
    std::vector<int> hitbox;
    sf::RectangleShape currentHitbox;
    if(txtFile)
    {
        while(txtFile.get(x))
        {
            if(x == '[')
            {
                level ++;
                number = 0;
            }
            else if(x == ']')
            {
                if(level == 1){
                    lign++;
                    column = 0;}
                else if(level == 2){
                    collisionHitboxes.push_back(currentCollisionHitboxes);
                    damageTakenHitboxes.push_back(currentDamageTakenHitboxes);
                    damageDealtHitboxes.push_back(currentDamageDealtHitboxes);
                    currentCollisionHitboxes.clear();
                    currentDamageTakenHitboxes.clear();
                    currentDamageDealtHitboxes.clear();
                    column++;}
                else if(level == 3){
                    hitbox.push_back(number);
                    currentHitbox.setPosition(sf::Vector2f(hitbox[0] * m_scale, hitbox[1] * m_scale));
                    currentHitbox.setSize(sf::Vector2f(hitbox[2] * m_scale, hitbox[3] * m_scale));
                    currentHitbox.setRotation(hitbox[5]);
                    if(hitbox[4] == 1){
                        currentCollisionHitboxes.push_back(currentHitbox);}
                    else if(hitbox[4] == 2){
                        currentDamageTakenHitboxes.push_back(currentHitbox);}
                    else if(hitbox[4] == 3){
                        currentDamageDealtHitboxes.push_back(currentHitbox);}
                    }
                level --;
                number = 0;
                hitbox = {};
            }
            else if(x == ',' && level == 3)
            {
                hitbox.push_back(number);
                number = 0;;
            }
            else
            {
                number *= 10;
                number += (int)x - 48;
            }

        }
        m_collisionHitboxes.push_back(collisionHitboxes);
        m_damageDealtHitboxes.push_back(damageDealtHitboxes);
        m_damageTakenHitboxes.push_back(damageTakenHitboxes);
    }
    else
    {
        std::cout << "can't open txt file : " << txtFileName << std::endl;
    }
    txtFile.close();
}


