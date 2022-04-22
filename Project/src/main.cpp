#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <vector>
#include <math.h>
#include "../include/Map.h"
#include "../include/Ground.h"
#include "../include/Animation.h"
#include "../include/Player.h"
#include "../include/Collider.h"
#include "../include/Fading.h"
#include "../include/Living_Entity.h"
#include "../include/MainMenu.h"
#include "../include/Ennemies/EnnemyFlying_eye.h"
#include "../include/Ennemies/EnnemySkeleton.h"
#include "../include/Ennemies/EnnemyBoss1.h"


static const float VIEW_WIDTH = 1500.0f; // defines the size of the view
static const float VIEW_HEIGHT = 845.0f;
static const float WINDOW_WIDTH = 1920.0f; // defines the size of the window
static const float WINDOW_HEIGHT = 1080.0f;


void ResizeView(const sf::RenderWindow & window, sf::View& view)
//not used anymore because it made the view bug, but it was made to set things to the right size after changing window size
{
       float aspectRatio = float(window.getSize().x / float(window.getSize().y));
       view.setSize(VIEW_WIDTH * aspectRatio, VIEW_WIDTH);
}

void getMapData(std::string txtName, Map &mapping)
/*
@txtName -> name and address of the text file containing the map data
@mapping -> Map object where we will store the map data

the first two words are the width and height of the map, we take them separately from the rest
then we get every info 5 by 5 to add the platforms the infos are :
width, height, position x, position y, platform type
then we store it in the Map object
*/
{
    std::ifstream monFlux(txtName);
    std::vector<float> numbers;
    float number;

    for(int i=0; i < 2; i++)
    {
    monFlux >> number;
    numbers.push_back(number);
    }
    mapping.SetMapSize(numbers[0], numbers[1]);
    numbers.resize(0);

    while(monFlux >> number)
    {
        numbers.push_back(number);
        if (numbers.size()>= 5)
        {
            if (numbers[4] == 1)//type 1 means we're dealing with a regular platform
            {
                Ground platform(sf::Vector2f(numbers[0], numbers[1]), sf::Vector2f(numbers[2], numbers[3]), (int)numbers[4]);
                mapping.AddGround(platform);
                numbers.resize(0);
            }
            else if (numbers[4] ==2 && numbers.size() == 9) //type 2 means we're dealing with a door so we have more parameters
                // the new parameters are : 1-the index of the map we're heading 2-the direction 3-4- the location
            {
                Ground platform(sf::Vector2f(numbers[0], numbers[1]), sf::Vector2f(numbers[2], numbers[3]), (int)numbers[4], (int)numbers[5], numbers[6], sf::Vector2f(numbers[7], numbers[8]));
                mapping.AddGround(platform);
                numbers.resize(0);
            }
            else if (numbers[4] ==3 && numbers.size() == 8) //type 3 means we're dealing with a trap so we have more parameters
                // the new parameters are 1-2- the location and 3- the direction (1 right, 2 down, 3 left, 4 up)
            {
                Ground platform(sf::Vector2f(numbers[0], numbers[1]), sf::Vector2f(numbers[2], numbers[3]), (int)numbers[4], sf::Vector2f(numbers[5], numbers[6]), numbers[7]);
                mapping.AddGround(platform);
                numbers.resize(0);
            }
        }
    }
}

void checkAllCollisions(Player &player, sf::Vector2f direction, Map &mapping, sf::RenderWindow& window)
/*
@player @direction -> informations regarding the player character in order to have him collide with object in a right way
@mapping -> a Map object from which we get every informations we need regarding the map the player is currently in (like the ennemies or the platforms)

we test the player collision with the platforms, then the ennemies collisions with the platforms and finally the ennemy collision with the player.
It's probably not very optimised but we will fix that in the future.
collision.
*/
{
    Collider playerCollider =player.GetCollider();
    std::vector<Collider> playerHitbox =player.GetDamageRecievedHitboxes();
    std::vector<Collider> playerAttacks =player.GetDamageDealtHitboxes();
    player.UnCollide();

    for(unsigned int i=0; i < mapping.m_levelGrounds.size(); i++) //collision with the ground for the player and every ennemy type
    {
        player.CheckCollisions(mapping.m_levelGrounds[i]);
        for(int enmy = 0; enmy < (int)mapping.m_levelEnnemyFlying_eye.size() ; enmy ++)
        {
            Collider EnnemyCollider = mapping.m_levelEnnemyFlying_eye[enmy].GetCollider();
            mapping.m_levelEnnemyFlying_eye[enmy].CollisionCheck(mapping.m_levelGrounds[i]);
            mapping.m_levelEnnemyFlying_eye[enmy].SetDistance(EnnemyCollider.Distance(playerCollider));
        }
        for(int enmy = 0; enmy < (int)mapping.m_levelEnnemySkeleton.size() ; enmy ++)
        {
            Collider EnnemyCollider = mapping.m_levelEnnemySkeleton[enmy].GetCollider();
            mapping.m_levelEnnemySkeleton[enmy].CollisionCheck(mapping.m_levelGrounds[i]);
            mapping.m_levelEnnemySkeleton[enmy].SetDistance(EnnemyCollider.Distance(playerCollider));
        }
        for(int enmy = 0; enmy < (int)mapping.m_levelEnnemyBoss1.size() ; enmy ++)
        {
            Collider EnnemyCollider = mapping.m_levelEnnemyBoss1[enmy].GetCollider();
            mapping.m_levelEnnemyBoss1[enmy].CollisionCheck(mapping.m_levelGrounds[i]);
            mapping.m_levelEnnemyBoss1[enmy].SetDistance(EnnemyCollider.Distance(playerCollider));
        }
    }
    //collision between player and ennemies
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemyFlying_eye.size() ; enmy ++)
    {
        mapping.m_levelEnnemyFlying_eye[enmy].DamageCheck(player, playerCollider, playerHitbox, playerAttacks);
    }
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemySkeleton.size() ; enmy ++)
    {
        mapping.m_levelEnnemySkeleton[enmy].DamageCheck(player, playerCollider, playerHitbox, playerAttacks);
    }
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemyBoss1.size() ; enmy ++)
    {
        mapping.m_levelEnnemyBoss1[enmy].DamageCheck(player, playerCollider, playerHitbox, playerAttacks);
    }
}

void updateAllMovement(float deltatime, Player &player, Map &mapping)
/*
we update every ennemy on the current map including the player (which is not in &mapping since it's unrelated to maps) and adjust their position depending on
deltaTime. The difference with movementAll is that movementAll decide what movement will the entity do next, and updateAllMovement update their movement. This is why
movementAll do not need deltaTime
*/
{
    player.UpdateMovement(deltatime);
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemyFlying_eye.size() ; enmy ++)
    {
        mapping.m_levelEnnemyFlying_eye[enmy].UpdateMovement(deltatime);
    }
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemySkeleton.size() ; enmy ++)
    {
        mapping.m_levelEnnemySkeleton[enmy].UpdateMovement(deltatime);
    }
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemyBoss1.size() ; enmy ++)
    {
        mapping.m_levelEnnemyBoss1[enmy].UpdateMovement(deltatime);
    }
}

void updateAllAnimation(float deltatime, Player &player, Map &mapping)
/*
we update every animations of every type of ennemy on the current map including the player depending on
deltaTime
*/
{
    player.UpdateAnimation(deltatime);
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemyFlying_eye.size() ; enmy ++)
    {
        mapping.m_levelEnnemyFlying_eye[enmy].UpdateAnimation(deltatime);
    }
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemySkeleton.size() ; enmy ++)
    {
        mapping.m_levelEnnemySkeleton[enmy].UpdateAnimation(deltatime);
    }
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemyBoss1.size() ; enmy ++)
    {
        mapping.m_levelEnnemyBoss1[enmy].UpdateAnimation(deltatime);
    }
}

void movementAll(Player &player, Map &mapping, float deltaTime)
/*
We update the movement and behavior of every ennemy on the map and the player.
*/
{
    player.Inputs(deltaTime);
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemyFlying_eye.size() ; enmy ++)
    {
        mapping.m_levelEnnemyFlying_eye[enmy].Behavior(deltaTime);
    }
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemySkeleton.size() ; enmy ++)
    {
        mapping.m_levelEnnemySkeleton[enmy].Behavior(deltaTime);
    }
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemyBoss1.size() ; enmy ++)
    {
        mapping.m_levelEnnemyBoss1[enmy].Behavior(deltaTime);
    }
}

void drawAll(sf::RenderWindow& window, Player &player, Fading &fadeScreen, Map &mapping, sf::View view)
/*
simple drawAll function where we will draw every entities that needs to be drawn on the window
*/
{
    mapping.DrawBackground(window);
    mapping.DrawPlatforms(window);
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemyFlying_eye.size() ; enmy ++)
    {
        mapping.m_levelEnnemyFlying_eye[enmy].Draw(window);
    }
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemySkeleton.size() ; enmy ++)
    {
        mapping.m_levelEnnemySkeleton[enmy].Draw(window);
    }
    for(int enmy = 0; enmy < (int)mapping.m_levelEnnemyBoss1.size() ; enmy ++)
    {
        mapping.m_levelEnnemyBoss1[enmy].Draw(window);
    }
    player.Draw(window);

    //we then draw the heart in the upper left corner
    sf::Texture heartTexture;
    heartTexture.loadFromFile("Sprites/HUD/heartFull.png");
    sf::RectangleShape heart;
    heart.setSize(sf::Vector2f(50, 50));
    heart.setTexture(&heartTexture);
    for(int i = 0; i < player.GetHealth(); i++){
        heart.setPosition(view.getCenter().x - VIEW_WIDTH/2 + 60*i, view.getCenter().y - VIEW_HEIGHT/2);
        window.draw(heart);
    }
    heartTexture.loadFromFile("Sprites/HUD/heartEmpty.png");
    heart.setTexture(&heartTexture);
    for(int i = player.GetHealth(); i < player.GetMaxHealth(); i++){
        heart.setPosition(view.getCenter().x - VIEW_WIDTH/2 + 60*i, view.getCenter().y - VIEW_HEIGHT/2);
        window.draw(heart);
    }
    fadeScreen.Draw(window, view);
}

int main()
{
    int menu=1;//determine if the menu is active
    float deltaTime = 0.0f;//used so that any machine runs the game at the same speed but is the source of some bugs when not handled carefully.
                            // it simply represent the time passed between this frame and the previous frame.
    sf::Clock clock;//needed to update deltatime
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Platformer Game"); //creation of the window and the view
    sf::View view(sf::Vector2f(0.0f, 0.0f), sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT));

    MainMenu mainMenu(window.getSize().x, window.getSize().y); //creation of the main menu
    sf::RectangleShape background;
    background.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    sf::Texture Maintexture;
    sf::Image icon;
    icon.loadFromFile("Sprites/Icon/sword.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    Maintexture.loadFromFile("Sprites/background/menu/backm.png");
    background.setTexture(&Maintexture);

    float closeTimer = 0; //made to know when to close the window
    int currentLevel = 0;//the current level we're in
    int const level1MapNumber = 4;//number of map in the first level
    float view_x;//variables used to display the view
    float view_y;
    Map level1[level1MapNumber];//a list of all map in the first level
    getMapData("Data/000.txt", level1[0]);//we collect data for the map from getMapData
    getMapData("Data/001.txt", level1[1]);//we collect data for the map from getMapData
    getMapData("Data/002.txt", level1[2]);//we collect data for the map from getMapData
    getMapData("Data/003.txt", level1[3]);//we collect data for the map from getMapData

    Player player;
    sf::Vector2f direction;//this is the player direction
    player.SetPosition(sf::Vector2f(650, -1000));
    //player.SetPosition(sf::Vector2f(100, 2000));

    //creating every ennemy. The next step would be to automate this.
    EnnemySkeleton ennemy1(2500, 750);
    EnnemyFlying_eye ennemy2(500, 300);
    EnnemySkeleton ennemy3(1300, 800);
    EnnemySkeleton ennemy4(1500, 1750);
    EnnemySkeleton ennemy5(1400, 1750);
    EnnemyFlying_eye ennemy6(850, 1500);
    EnnemyFlying_eye ennemy7(400, 1000);
    EnnemyFlying_eye ennemy8(600, 800);
    EnnemyFlying_eye ennemy9(200, 300);
    EnnemySkeleton ennemy10(1200, 450);
    EnnemySkeleton ennemy11(1650, 1250);
    EnnemySkeleton ennemy12(1200, 1250);
    EnnemyBoss1 ennemy13(100, 400);
    Fading fadeScreen(0.2, sf::RectangleShape(sf::Vector2f(VIEW_WIDTH, VIEW_HEIGHT)));//creating the fading screen for transitions
    while (window.isOpen())
    {
        deltaTime = clock.restart().asSeconds();//we get the time that took the machine to get through the whole cycle each time so we can calculate movement based on that
        if (deltaTime > 1.0f/20.0f)// we set a framerate limit from which the game will slow down
        {
            deltaTime = 1.0f/20.0f;
        }

        sf::Event evnt;
        while (window.pollEvent(evnt))//check what events are called
        {
            if (evnt.type == sf::Event::Closed){//closing window
                window.close();
            }
            if (evnt.type == sf::Event::KeyReleased && menu == 1)//if we're in the main menu :
            {
                if (evnt.key.code == sf::Keyboard::Up)//moving the selection
                {
                    mainMenu.MoveUp();
                }
                if (evnt.key.code == sf::Keyboard::Down)
                {
                    mainMenu.MoveDown();
                }
                if (evnt.key.code == sf::Keyboard::Return)//if we select an option
                {
                    if (mainMenu.MainMenuPressed() == 0)
                    {
                        for(int i = 0; i < level1MapNumber ; i++)
                        {
                            level1[i].Reset();
                        }
                        closeTimer = 0;
                        currentLevel = 0;
                        menu=0;
                        ennemy1.Reset(2500, 750);//the respawn of ennemies was added pretty late and we are aware it's probably not a good way to do it.
                        ennemy2.Reset(500, 300);
                        ennemy3.Reset(1300, 800);
                        ennemy4.Reset(1500, 1750);
                        ennemy5.Reset(1400, 1750);
                        ennemy6.Reset(850, 1500);
                        ennemy7.Reset(400, 1000);
                        ennemy8.Reset(600, 800);
                        ennemy9.Reset(200, 300);
                        ennemy10.Reset(1200, 450);
                        ennemy11.Reset(1650, 1250);
                        ennemy12.Reset(1200, 1250);
                        ennemy13.Reset(100, 400);
                        level1[0].AddEnnemySkeleton(ennemy1);
                        level1[1].AddEnnemyFlying_eye(ennemy2);
                        level1[1].AddEnnemySkeleton(ennemy3);
                        level1[1].AddEnnemySkeleton(ennemy4);
                        level1[1].AddEnnemySkeleton(ennemy5);
                        level1[2].AddEnnemyFlying_eye(ennemy6);
                        level1[2].AddEnnemyFlying_eye(ennemy7);
                        level1[2].AddEnnemyFlying_eye(ennemy8);
                        level1[2].AddEnnemyFlying_eye(ennemy9);
                        level1[2].AddEnnemySkeleton(ennemy10);
                        level1[2].AddEnnemySkeleton(ennemy11);
                        level1[2].AddEnnemySkeleton(ennemy12);
                        level1[3].AddEnnemyBoss1(ennemy13);
                        player.Reset();
                        player.SetPosition(sf::Vector2f(650, -1000));
                        //player.SetPosition(sf::Vector2f(100, 2000));

                    }
                    else if (mainMenu.MainMenuPressed() == 1)
                    {
                        window.close();
                    }
                }
            }
        }
        if(menu == 1)//when in the menu
        {
            window.clear();
            window.draw(background);
            mainMenu.draw(window);
            window.display();
        }
        if(player.IsAlive() && menu == 0) //when in the game
        {
            movementAll(player, level1[currentLevel], deltaTime);//the character updates depending on the player inputs
            if(player.Door(deltaTime, currentLevel) || player.GetTrap() > 0.5)//handle doors and trap events for screen fading
            {
                fadeScreen.Fade(deltaTime);
            }
            else if (player.GetTrap() >= 0.4 && player.GetTrap() <= 0.5)
            {
                player.SetPosition(player.GetNextLocation());
            }
            else
            {
                fadeScreen.UnFade(deltaTime);
                if (player.GetTrap() > 0 && player.GetTrap() <= 0.1)
                {
                    player.Freed();
                }
            }

            sf::Vector2f playerCenter = player.GetPosition() + player.GetSize()/2.0f;
            view_x = std::min(level1[currentLevel].GetMapSize().x-VIEW_WIDTH/2  , std::max(playerCenter.x, VIEW_WIDTH/2));//the view must only show the inside of a map so we set boundaries here
            view_y = std::min(level1[currentLevel].GetMapSize().y-VIEW_HEIGHT/2, std::max(playerCenter.y, VIEW_HEIGHT/2));
            view.setCenter(view_x, view_y);//and set the view center
            window.setView(view);


            window.clear(sf::Color(150,150,150));//we then refresh the window, update and redraw everything
            updateAllMovement(deltaTime,player, level1[currentLevel]);
            checkAllCollisions(player, direction, level1[currentLevel], window);
            updateAllAnimation(deltaTime,player, level1[currentLevel]);

            drawAll(window, player, fadeScreen, level1[currentLevel], view);
            window.display();
        }
        else if(menu == 0) //if the player is dead and we're in the game :
        {
            if(closeTimer >= 2.9){ //if the animation is finished we get back to the menu
                view.setCenter(background.getPosition()+background.getSize()/2.0f);
                window.setView(view);
                menu = 1;}
            else{//or we update the death window
                closeTimer += deltaTime;
                sf::Vector2f playerCenter(player.GetPosition() + player.GetSize()/2.0f);
                view.setCenter(playerCenter.x, playerCenter.y);//and set the view center
                window.setView(view);
                fadeScreen.Fade(deltaTime);
                fadeScreen.Draw(window, view);
                player.UpdateAnimationDeath(deltaTime);
                player.Draw(window);}
            window.display();

        }
    }
    return 0;
}
