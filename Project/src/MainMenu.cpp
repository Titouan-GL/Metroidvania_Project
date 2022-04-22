#include <SFML/Graphics.hpp>
#include "../include/MainMenu.h"

MainMenu::MainMenu(float width, float height)
{
	if (!font.loadFromFile("Fonts/Uroob-Regular.ttf")){
		std::cout << "No font is here" << std::endl;;
	}


	mainMenu[0].setFont(font);
	mainMenu[0].setFillColor(Color::White);
	mainMenu[0].setString("Play");
	mainMenu[0].setCharacterSize(70);
	mainMenu[0].setPosition(500, 200);

	mainMenu[1].setFont(font);
	mainMenu[1].setFillColor(Color::White);
	mainMenu[1].setString("Exit");
	mainMenu[1].setCharacterSize(70);
	mainMenu[1].setPosition(500, 300);


	MainMenuSelected = -1;


}

MainMenu::~MainMenu()
{

}

void MainMenu::draw(RenderWindow& window) {
	for (int i = 0; i < Max_main_menu; ++i){
		window.draw(mainMenu[i]);
	}
}

void MainMenu::MoveUp()
{
	if (MainMenuSelected - 1 >= 0){
		mainMenu[MainMenuSelected].setFillColor(Color::White);

		MainMenuSelected--;
		if (MainMenuSelected == -1){
			MainMenuSelected = 2;
		}
		mainMenu[MainMenuSelected].setFillColor(Color::Blue);

	}
}

void MainMenu::MoveDown(){
	if(MainMenuSelected + 1 <= Max_main_menu){
		mainMenu[MainMenuSelected].setFillColor(Color::White);
		MainMenuSelected++;
		if (MainMenuSelected == 2){
			MainMenuSelected = 0;
		}
		mainMenu[MainMenuSelected].setFillColor(Color::Blue);
	}
}

/*
Event event;
    while (window.pollEvent(event))
    {
        if (event.type == Event::Closed)
        {
            window.close();
        }
        if (event.type == Event::KeyReleased)
        {
            if (event.key.code == Keyboard::Up)
            {
                mainMenu.MoveUp();
                break;
            }
            if (event.key.code == Keyboard::Down)
            {
                mainMenu.MoveDown();
                break;
            }
        }
    }
    */
