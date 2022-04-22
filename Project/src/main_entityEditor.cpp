#include <SFML/Graphics.hpp>
#include <iostream>
#include <typeinfo>
#include <fstream>
#include <vector>
#include <math.h>
#include <string>
#include <fstream>
#include "../include/TextBox.h"
#include "../include/Button.h"

static const float VIEW_WIDTH = 1920.0f; // defines the size of the view and the window, we might have to separate the two at some point
static const float VIEW_HEIGHT = 1000.0f;


bool inRange(int maxCol, int maxLig, int curCol, int curLig){
    return curCol > 0 && curCol <= maxCol && curLig > 0 && curLig <= maxLig;
}

void windowDraw(sf::RenderWindow& window, TextBox txtbxs[], const int &txtbxNbr, Button buttons[], const int &buttonsNbr, bool working)
/*
simple drawAll function where we will draw every entities that needs to be drawn on the window
*/
{
    window.clear(sf::Color(150,150,150));
    sf::RectangleShape bg(sf::Vector2f(1580, 864));
    bg.setPosition(0, VIEW_HEIGHT-864);
    bg.setFillColor(sf::Color(180,180,180));
    window.draw(bg);

    for(int i = 0; i < txtbxNbr; i++)
    {
        txtbxs[i].Draw(window);
    }
    for(int i = 0; i < buttonsNbr; i++)
    {
        buttons[i].Draw(window);
    }

}

void drawSquare(sf::RenderWindow& window, sf::Vector2f size, sf::Vector2f position,int type,int rotation, bool selected)
{
    sf::RectangleShape square;
    square.setSize(size);
    square.setPosition(position);
    square.setRotation(rotation);
    if(selected)
    {
        square.setFillColor(sf::Color(200, 200, 200, 200));
    }
    else if(type == 1)
    {
        square.setFillColor(sf::Color(255, 255, 0, 128));
    }
    else if(type == 2)
    {
        square.setFillColor(sf::Color(0, 128, 0, 128));
    }
    else if(type == 3)
    {
        square.setFillColor(sf::Color(128, 0, 0, 128));
    }
    else if(type == 4)
    {
        square.setFillColor(sf::Color(0, 0, 128, 128));
    }
    else if(type >= 5)
    {
        square.setFillColor(sf::Color(100, 100, 100, 128));
    }
    if (type >= 1){
        window.draw(square);}
}
void windowDrawWorking(sf::RenderWindow& window, sf::RectangleShape &imageShape, sf::Texture &image, const int &MaxLign, const int &MaxColumn, const int &currentLign, const int &currentColumn, std::vector<std::vector<std::vector<Button>>> &hitSelect, std::vector<std::vector<int>> &hitboxes)
{
    sf::Vector2f SpriteSize;
    sf::Vector2f SpritePos;
    sf::Vector2f Scale(1.0f, 1.0f);
    if (currentColumn == 0){
        SpriteSize.x = image.getSize().x;
        SpritePos.x = 0;
    }
    else {
        SpriteSize.x = image.getSize().x/MaxColumn;
        SpritePos.x = (currentColumn-1) * image.getSize().x/MaxColumn;
    }
    if (currentLign == 0){
        SpriteSize.y = image.getSize().y;
        SpritePos.y = 0;
    }
    else {
        SpriteSize.y = image.getSize().y/MaxLign;
        SpritePos.y = (currentLign-1) * image.getSize().y/MaxLign;
    }
    while (SpriteSize.x*Scale.x > 1500 || SpriteSize.y*Scale.y > 850)
    {
        Scale *= 0.95f;
    }
    if(inRange(MaxColumn, MaxLign, currentColumn, currentLign))
    {
        for (int i = 0; i < (int)hitSelect[currentLign-1][currentColumn-1].size(); i++)
        {
            hitSelect[currentLign-1][currentColumn-1][i].Draw(window);
        }
    }
    imageShape.setPosition(sf::Vector2f(790 - (SpriteSize.x * Scale.x)/2, 575- (SpriteSize.y* Scale.y)/2));
    imageShape.setScale(Scale);
    imageShape.setSize(sf::Vector2f(SpriteSize.x, SpriteSize.y));
    imageShape.setTexture(&image);
    imageShape.setTextureRect(sf::IntRect(SpritePos.x, SpritePos.y, SpriteSize.x, SpriteSize.y));
    window.draw(imageShape);
}

void onClick(const sf::RenderWindow& window, const sf::Vector2i& mousePos, TextBox txtbxs[], const int &txtbxNbr, Button buttons[], const int &buttonsNbr, int &HitboxSelected)
{
    for(int i = 0; i < txtbxNbr; i++)
    {
        txtbxs[i].Selection(sf::Vector2f(mousePos.x *(VIEW_WIDTH/window.getSize().x), mousePos.y * (VIEW_HEIGHT/window.getSize().y)));
    }
    for(int i = 0; i < buttonsNbr; i++)
    {
        buttons[i].Clicking(sf::Vector2f(mousePos.x *(VIEW_WIDTH/window.getSize().x), mousePos.y * (VIEW_HEIGHT/window.getSize().y)));
        if(buttons[i].IsChecked())
        {
            HitboxSelected = 0;
            txtbxs[4].SetString("Position X");
            txtbxs[5].SetString("Position Y");
            txtbxs[6].SetString("Width");
            txtbxs[7].SetString("Height");
            txtbxs[8].SetString("Type");
            txtbxs[9].SetString("Angle");
        }
    }
}

void onClickSelection(const sf::RenderWindow& window, const sf::Vector2i& mousePos, std::vector<Button> &HitboxSelect, int &hitboxSelected, std::vector<std::vector<int>> &hitboxes, TextBox txtbxs[])
{
    for(int i = 0; i < (int)HitboxSelect.size(); i++)
    {
        HitboxSelect[i].Unclicked();
        HitboxSelect[i].Clicking(sf::Vector2f(mousePos.x *(VIEW_WIDTH/window.getSize().x), mousePos.y * (VIEW_HEIGHT/window.getSize().y)));
        if(HitboxSelect[i].IsChecked())
        {
            hitboxSelected = i+1;
            std::vector<int> hitbox = hitboxes[i];
            txtbxs[4].SetInput(std::to_string(hitbox[0]));
            txtbxs[4].SetString(std::to_string(hitbox[0]));
            txtbxs[5].SetInput(std::to_string(hitbox[1]));
            txtbxs[5].SetString(std::to_string(hitbox[1]));
            txtbxs[6].SetInput(std::to_string(hitbox[2]));
            txtbxs[6].SetString(std::to_string(hitbox[2]));
            txtbxs[7].SetInput(std::to_string(hitbox[3]));
            txtbxs[7].SetString(std::to_string(hitbox[3]));
            txtbxs[8].SetInput(std::to_string(hitbox[4]));
            txtbxs[8].SetString(std::to_string(hitbox[4]));
            txtbxs[9].SetInput(std::to_string(hitbox[5]));
            txtbxs[9].SetString(std::to_string(hitbox[5]));
        }
    }
    if(hitboxSelected > 0){
        HitboxSelect[hitboxSelected-1].Clicked();}
}

void onInputSetHitboxes(TextBox txtbxs[], std::vector<std::vector<int>> &hitboxes, const int &HitboxSelected)
{
    for(int i = 0; i < 6; i++)
    {
        try{
            hitboxes[HitboxSelected-1][i] = std::stoi(txtbxs[i+4].GetText());
        }
        catch (const std::invalid_argument &e){
            txtbxs[i+4].BecomeRed();
        }
        catch (const std::out_of_range &e){
            txtbxs[i+4].BecomeRed();
        }
    }
}

bool buttonOneEvent(TextBox txtbxs[], int const &txtbxNumber, sf::Texture &picture, std::string &txtFileName, int &column, int &lign)
{
    bool canWork = true;
    sf::Texture picTexture;
    if (picTexture.loadFromFile(txtbxs[0].GetText()))
    {
        picture.loadFromFile(txtbxs[0].GetText());
    }
    else{
        txtbxs[0].BecomeRed();
        canWork = false;
    }
    if (txtbxs[1].GetText().length() < 1)
    {
        txtbxs[1].BecomeRed();
        canWork = false;
    }
    else
    {
        txtFileName = txtbxs[1].GetText();
    }
    try{
        if (std::stoi(txtbxs[2].GetText()) > 0){
            lign = std::stoi(txtbxs[2].GetText());
        }
        else
        {
            txtbxs[2].BecomeRed();
            canWork = false;
        }
    }
    catch (const std::invalid_argument &e){
        txtbxs[2].BecomeRed();
        canWork = false;
    }
    catch (const std::out_of_range &e){
        txtbxs[2].BecomeRed();
        canWork = false;
    }
    try{
        if (std::stoi(txtbxs[3].GetText()) > 0){
            column = std::stoi(txtbxs[3].GetText());
        }
        else
        {
            txtbxs[3].BecomeRed();
            canWork = false;
        }
    }
    catch (const std::invalid_argument &e){
        txtbxs[3].BecomeRed();
        canWork = false;
    }
    catch (const std::out_of_range &e){
        txtbxs[3].BecomeRed();
        canWork = false;
    }
    return canWork;
}

void GetFrame(TextBox txtbxs[], int &currentLign, int &currentColumn)
{
    try{
        currentLign = std::stoi(txtbxs[10].GetText());
    }
    catch (const std::invalid_argument &e){
        if(!txtbxs[10].IsSelected())
        {
            txtbxs[10].BecomeRed();
        }
        currentLign = 0;
    }
    catch (const std::out_of_range &e){
        if(!txtbxs[10].IsSelected())
        {
            txtbxs[10].BecomeRed();
        }
        currentLign = 0;
    }
    try{
        currentColumn = std::stoi(txtbxs[11].GetText());
    }
    catch (const std::invalid_argument &e){
        if(!txtbxs[11].IsSelected())
        {
            txtbxs[11].BecomeRed();
        }
        currentColumn = 0;
    }
    catch (const std::out_of_range &e){
        if(!txtbxs[11].IsSelected())
        {
            txtbxs[11].BecomeRed();
        }
        currentColumn = 0;
    }

}

void exportToFile(std::vector<std::vector<std::vector<std::vector<int>>>> hitboxes, std::string txtFileName)
{
    std::ofstream txtFile(txtFileName, std::ios::out);
    for(int i = 0; i < (int)hitboxes.size(); i++)
    {
        txtFile << "[";
        for(int j = 0; j < (int)hitboxes[i].size(); j++)
        {
            txtFile << "[";
            for(int k = 0; k < (int)hitboxes[i][j].size(); k++)
            {
                if(hitboxes[i][j][k][4] > 0)
                {
                    txtFile << "[";
                    for(int l = 0; l < (int)hitboxes[i][j][k].size(); l++)
                    {
                        txtFile << hitboxes[i][j][k][l] <<",";
                    }
                    txtFile << "]";
                }
            }
            txtFile << "]";
        }
        txtFile << "]";
    }
    txtFile.close();
}

void importFromFile(std::vector<std::vector<std::vector<std::vector<int>>>> &hitboxes, std::vector<std::vector<std::vector<Button>>> &hitSelect, std::string txtFileName)
{
    std::ifstream txtFile(txtFileName);
    char x = 0;
    int level = 0;
    int lign = 0;
    int column = 0;
    int number = 0;
    std::vector<int> hitbox;
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
                    column++;}
                else if(level == 3){
                    hitbox.push_back(number);
                    Button butt = Button(sf::Vector2f(1600, 200+hitSelect[lign][column].size()*40), sf::RectangleShape(sf::Vector2f(300, 30)));
                    butt.SetColorOn(sf::Color(170, 200, 200));
                    butt.SetColorOff(sf::Color(150, 175, 175));
                    butt.Activated();
                    butt.SetAlwaysOn(true);
                    hitSelect[lign][column].push_back(butt);
                    hitboxes[lign][column].push_back(hitbox);}
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
    }
}

int main()
{
    bool working=false;
    sf::RenderWindow window(sf::VideoMode(VIEW_WIDTH, VIEW_HEIGHT), "Entity Editor"); //creation of the window and the view
    std::vector<std::vector<std::vector<std::vector<int>>>> hitboxes;//the first level of table is all the ligns of hitboxes. The second level is all the columns. The third level is all the hitboxes and inside is the fourth level :
                                //the position, the shape, the orientation, the type

    std::vector<std::vector<std::vector<Button>>> hitboxesSelect;

    sf::RectangleShape mainPictureShape;
    sf::Texture mainPicture;
    std::string txtFileName;
    int MaxColumn = 0;
    int MaxLign = 0;
    int currentColumn = 0;
    int currentLign = 0;
    int hitboxSelected = 0;

    int const txtbxNumber = 12;
    TextBox txtbxs[txtbxNumber];
    txtbxs[0].SetPosition(sf::Vector2f(250, 490));
    txtbxs[0].SetString("Sprite Adress");
    txtbxs[0].Activated();
    txtbxs[1].SetPosition(sf::Vector2f(770, 490));
    txtbxs[1].SetString("txt destination");
    txtbxs[1].Activated();
    txtbxs[2].SetPosition(sf::Vector2f(250, 550));
    txtbxs[2].SetString("lign number");
    txtbxs[2].Activated();
    txtbxs[3].SetPosition(sf::Vector2f(770, 550));
    txtbxs[3].SetString("column number");
    txtbxs[3].Activated();

    txtbxs[4].SetPosition(sf::Vector2f(20, 20));
    txtbxs[4].SetString("Position X");
    txtbxs[5].SetPosition(sf::Vector2f(540, 20));
    txtbxs[5].SetString("Position Y");
    txtbxs[6].SetPosition(sf::Vector2f(20, 80));
    txtbxs[6].SetString("Width");
    txtbxs[7].SetPosition(sf::Vector2f(540, 80));
    txtbxs[7].SetString("Height");
    txtbxs[8].SetPosition(sf::Vector2f(1060, 20));
    txtbxs[8].SetString("Type");
    txtbxs[9].SetPosition(sf::Vector2f(1060, 80));
    txtbxs[9].SetString("Angle");

    txtbxs[10].SetPosition(sf::Vector2f(1600, 20));
    txtbxs[10].setSize(sf::Vector2f(300, 40));
    txtbxs[10].SetString("frame lign");
    txtbxs[11].SetPosition(sf::Vector2f(1600, 80));
    txtbxs[11].setSize(sf::Vector2f(300, 40));
    txtbxs[11].SetString("frame column");

    int const buttonNbr = 2;
    Button buttons[buttonNbr];
    buttons[0].SetPosition(sf::Vector2f(1300, 520));
    buttons[0].Activated();

    buttons[1].SetPosition(sf::Vector2f(1800, 150));


    while (window.isOpen())
    {
        sf::Event evnt;
        while (window.pollEvent(evnt))//check what events are called
        {
            if (evnt.type == sf::Event::Closed){
                window.close();
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
            {
                onClick(window, sf::Mouse::getPosition(window), txtbxs, txtbxNumber, buttons, buttonNbr, hitboxSelected);
                if(working && inRange(MaxColumn, MaxLign, currentColumn, currentLign) )
                {
                    onClickSelection(window, sf::Mouse::getPosition(window), hitboxesSelect[currentLign-1][currentColumn-1], hitboxSelected, hitboxes[currentLign-1][currentColumn-1], txtbxs);
                }
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
            {
                for(int i = 0; i < txtbxNumber ; i ++)
                {
                    txtbxs[i].Unselect();
                }
            }
            else if (evnt.type == sf::Event::TextEntered)
            {
                for(int i = 0; i < txtbxNumber ; i ++)
                {
                    if(i == 10 || i == 11)
                    {
                        if(txtbxs[i].IsSelected())
                        {
                            hitboxSelected = 0;
                            txtbxs[4].SetString("Position X");
                            txtbxs[5].SetString("Position Y");
                            txtbxs[6].SetString("Width");
                            txtbxs[7].SetString("Height");
                            txtbxs[8].SetString("Type");
                            txtbxs[9].SetString("Angle");
                        }
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::BackSpace))
                    {
                        txtbxs[i].Erase();
                    }
                    else
                    {
                        txtbxs[i].Write(evnt);
                    }
                }
            }
            if (buttons[0].IsChecked())
            {
                if (buttonOneEvent(txtbxs, txtbxNumber, mainPicture, txtFileName, MaxColumn, MaxLign))
                {
                    for(int i = 0; i < 4; i ++){
                        txtbxs[i].Deactivate();}
                    for(int i = 4; i < 12; i ++){
                        txtbxs[i].Activated();}
                    buttons[0].Deactivate();
                    buttons[1].Activated();
                    for(int i = 0; i < MaxLign; i++)
                    {
                        std::vector<std::vector<std::vector<int>>> newEntry;
                        std::vector<std::vector<Button>> newEntry2;
                        for(int j = 0; j < MaxColumn; j++)
                        {
                            std::vector<std::vector<int>> newNewEntry;
                            std::vector<Button> newNewEntry2;
                            newEntry.push_back(newNewEntry);
                            newEntry2.push_back(newNewEntry2);
                        }
                        hitboxes.push_back(newEntry);
                        hitboxesSelect.push_back(newEntry2);
                    }
                    importFromFile(hitboxes, hitboxesSelect, txtFileName);
                    working = true;
                }
                buttons[0].Unclicked();
            }
        }
        windowDraw(window, txtbxs, txtbxNumber, buttons, buttonNbr, working);
        if (working)
        {
            GetFrame(txtbxs, currentLign, currentColumn);
            if (buttons[1].IsChecked() && inRange(MaxColumn, MaxLign, currentColumn, currentLign))
            {
                Button butt = Button(sf::Vector2f(1600, 200+hitboxesSelect[currentLign-1][currentColumn-1].size()*40), sf::RectangleShape(sf::Vector2f(300, 30)));
                butt.SetColorOn(sf::Color(170, 200, 200));
                butt.SetColorOff(sf::Color(150, 175, 175));
                butt.Activated();
                butt.SetAlwaysOn(true);
                hitboxesSelect[currentLign-1][currentColumn-1].push_back(butt);
                std::vector<int> hitbox = {0, 0, 100, 100, 1, 0};
                hitboxes[currentLign-1][currentColumn-1].push_back(hitbox);
                buttons[1].Unclicked();
            }
            if(hitboxSelected > 0 && inRange(MaxColumn, MaxLign, currentColumn, currentLign))
            {
                onInputSetHitboxes(txtbxs, hitboxes[currentLign-1][currentColumn-1], hitboxSelected);
            }
            windowDrawWorking(window,mainPictureShape, mainPicture, MaxLign, MaxColumn, currentLign, currentColumn, hitboxesSelect, hitboxes[currentLign-1][currentColumn-1]);
            if(inRange(MaxColumn, MaxLign, currentColumn, currentLign))
            {
                for(int i = 0; i < (int)hitboxes[currentLign-1][currentColumn-1].size() ; i++)
                {
                    drawSquare(window, sf::Vector2f(hitboxes[currentLign-1][currentColumn-1][i][2], hitboxes[currentLign-1][currentColumn-1][i][3]),
                    sf::Vector2f(mainPictureShape.getPosition().x + hitboxes[currentLign-1][currentColumn-1][i][0], mainPictureShape.getPosition().y + hitboxes[currentLign-1][currentColumn-1][i][1]),
                    hitboxes[currentLign-1][currentColumn-1][i][4],hitboxes[currentLign-1][currentColumn-1][i][5], hitboxSelected == i+1);
                }
            }
            exportToFile(hitboxes, txtFileName);
        }

    window.display();
    }


    return 0;
}
