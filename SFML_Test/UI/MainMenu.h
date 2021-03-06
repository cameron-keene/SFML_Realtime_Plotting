#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>

using namespace std;

#define WINDOWWIDTH 1180
#define WINDOWHEIGHT 600

class MainMenu {
public:
	sf::RenderWindow mainMenuWindow;
	sf::Font mainMenuFont;
	sf::Event mainMenuEvent;
	sf::String userInput;
	sf::Text userText;
	sf::Text userPrompt;
	sf::Text selectionText;
	sf::Text selection1;
	sf::Text selection2;

	MainMenu();
	void OpenMenu(string &_output);
};