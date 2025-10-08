#include "PauseState.h"
#include "MenuState.h"

PauseState::PauseState() {
    font.loadFromFile("assets/ARIAL.TTF");
    pauseText.setFont(font);
    pauseText.setString("PAUSA - ENTER para volver al menu");
    pauseText.setCharacterSize(24);
    pauseText.setPosition(100, 200);


    areaPausaTextura.loadFromFile("images/menu.png");
    sf::Sprite areaPausaSprite(areaPausaTextura);
    areaPausaSprite.setPosition(400,200);

    continuaTextura.loadFromFile("images/continua.png");
    sf::Sprite continuaSprite(continuaTextura);
    continuaSprite.setPosition(475, 250);

    selectNivelTextura.loadFromFile("images/nivel1Textura.png");
    sf::Sprite selectNivelSprite(selectNivelTextura);
    selectNivelSprite.setPosition(465, 350);
}

void PauseState::handleInput(sf::RenderWindow& window, StateManager& manager) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if(event.type == sf::Event::MouseButtonPressed){
            
        }
    }
}

void PauseState::update(float dt) {}
void PauseState::render(sf::RenderWindow& window) {
    window.clear(sf::Color(50, 50, 50,200));
    window.draw(pauseText);
    window.display();
}
