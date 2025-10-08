#include "MenuState.h"
#include "PauseState.h"
#include <iostream>

MenuState::MenuState() {
    font.loadFromFile("assets/fonts/arial.ttf");

    title.setFont(font);
    title.setString("MENU PRINCIPAL");
    title.setCharacterSize(40);
    title.setPosition(200, 100);

    startText.setFont(font);
    startText.setString("Presiona ENTER para comenzar");
    startText.setPosition(180, 250);

    exitText.setFont(font);
    exitText.setString("Presiona ESC para salir");
    exitText.setPosition(180, 300);
}

void MenuState::handleInput(sf::RenderWindow& window, StateManager& manager) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
            std::cout << "Iniciando juego..." << std::endl;
            manager.change(std::make_unique<PauseState>()); // Ejemplo: pasa al estado de pausa
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            window.close();
    }
}

void MenuState::update(float dt) { /* Animaciones si quieres */ }

void MenuState::render(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    window.draw(title);
    window.draw(startText);
    window.draw(exitText);
    window.display();
}
