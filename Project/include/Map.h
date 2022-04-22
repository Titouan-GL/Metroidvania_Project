#ifndef MAP_H
#define MAP_H

#include "Ground.h"
#include <vector>
#include <variant>
#include "Living_Entity.h"
#include "Ennemies/EnnemyFlying_eye.h"
#include "Ennemies/EnnemySkeleton.h"
#include "Ennemies/EnnemyBoss1.h"

class Map
{
    public:
        Map();
        Map(int const& width, int const& height);
        void AddGround(Ground ground);//add a platofrm to m_levelGrounds
        void AddEnnemySkeleton(EnnemySkeleton ennemy);//add a platofrm to m_levelGrounds
        void AddEnnemyFlying_eye(EnnemyFlying_eye ennemy);//add a platofrm to m_levelGrounds
        void AddEnnemyBoss1(EnnemyBoss1 ennemy);//add a platofrm to m_levelGrounds
        void SetMapSize(float width, float height);
        void DrawBackground(sf::RenderWindow& window);
        void DrawPlatforms(sf::RenderWindow& window);
        void Reset();
        sf::Vector2f GetMapSize() {return m_mapSize.getSize();}
        virtual ~Map();

    public:
        std::vector<Ground> m_levelGrounds;//I had to make it public because modifying it is very important. It's the list of all platforms on the level
        std::vector<EnnemySkeleton> m_levelEnnemySkeleton;
        std::vector<EnnemyFlying_eye> m_levelEnnemyFlying_eye;
        std::vector<EnnemyBoss1> m_levelEnnemyBoss1;

    protected:

        sf::RectangleShape m_mapSize;
        std::vector<sf::Texture> m_backgroundsTexture;
        std::vector<sf::Texture> m_platformsTexture;


    private:
};

#endif // MAP_H
