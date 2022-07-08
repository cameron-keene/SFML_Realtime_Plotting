#include "MainMenu.h";

MainMenu::MainMenu() {
    if (!this->mainMenuFont.loadFromFile("arial.ttf"))
        cout << "ERROR LOADING FONT!" << endl;



    this->userPrompt.setFont(this->mainMenuFont);
    this->userPrompt.setString("Enter Test Number: ");
    this->userPrompt.setPosition(1180 / 2 - 250 , 100);
    this->userPrompt.setFillColor(sf::Color::Blue);
    this->userPrompt.setCharacterSize(24);

    this->userText.setFont(this->mainMenuFont);
    this->userText.setPosition(1180 / 2 - 250 + 250, 100);
    this->userText.setFillColor(sf::Color::Blue);
    this->userText.setCharacterSize(24);

    this->selection1.setFont(this->mainMenuFont);
    this->selection1.setString("Intact: ");
    this->selection1.setPosition(1180 / 2 - 250, 150);
    this->selection1.setFillColor(sf::Color::Blue);
    this->selection1.setCharacterSize(24);

    this->selection2.setFont(this->mainMenuFont);
    this->selection2.setString("Residual: ");
    this->selection2.setPosition(1180 / 2 - 250, 200);
    this->selection2.setFillColor(sf::Color::Blue);
    this->selection2.setCharacterSize(24);

    this->mainMenuWindow.create(sf::VideoMode(1180, 600), "Testing Main Menu", sf::Style::Default);
}
void MainMenu::OpenMenu() {
    int enterCount = 0;

    while (this->mainMenuWindow.isOpen())
    {
        while (this->mainMenuWindow.pollEvent(this->mainMenuEvent))
        {
            if (this->mainMenuEvent.type == sf::Event::TextEntered)
            {
                if (this->mainMenuEvent.text.unicode < 128)
                {
                    this->userInput += this->mainMenuEvent.text.unicode;
                    this->userText.setString(this->userInput);
                }
            }
            else if (this->mainMenuEvent.type == sf::Event::KeyPressed)
            {
                if (this->mainMenuEvent.key.code == sf::Keyboard::Enter) 
                {
                    string temp = string(this->userInput);
                    int testing = stoi(temp);
                    cout << "UserInput: " << temp << endl;
                    enterCount++;
                    //this->mainMenuWindow.close();
                }
                else if (this->mainMenuEvent.key.code == sf::Keyboard::Escape)
                {
                    cout << "Escaped Pressed" << endl;
                    this->mainMenuWindow.close();
                }
            }
        }
        this->mainMenuWindow.clear(sf::Color(255, 255, 255));
        this->mainMenuWindow.draw(this->userText);
        this->mainMenuWindow.draw(this->userPrompt);
        if (enterCount > 0) {
            this->mainMenuWindow.draw(this->selection1);
            this->mainMenuWindow.draw(this->selection2);
        }
        this->mainMenuWindow.display();
    }
}
