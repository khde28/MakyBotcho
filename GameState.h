#pragma once

#include <iostream>

#include "Button.h"
#include "State.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>


class GameState : public State {
private:
    int contadorBucle = 0;

    std::vector<int> botonesPresionados;
    bool clickIzquierdoPresionado = false;


    sf::Transform isoTransform;

    //estrella de colision
    sf::Texture estrellaTextura;
    sf::Sprite estrellaSprite;

    //semaforo
    sf::Texture redSemaforoTextura, yellowSemaforoTextura, greenSemaforoTextura;
    sf::Sprite semaforoSprite;

    sf::Texture numeroContadorTextura[6];
    sf::Sprite numeroContadorSprite;

    //botones
    sf::Texture iniciarTextura;
    sf::Sprite iniciarSprite;

    sf::Texture incrementTexture;
    sf::Sprite incrementeSprite;

    sf::Texture decrementTexture;
    sf::Sprite decrementSprite;;

    sf::Texture ifTextura, elseTextura;
    sf::Sprite ifAndElseSprite;

    sf::Texture pausaTextura;
    sf::Sprite pausaSprite;

    sf::Texture restart1Textura;
    sf::Texture restartaTextura;
    sf::Sprite restart1Sprite;
    sf::Sprite restartaSprite;

    std::vector<sf::Texture> instruccionesTexturas;
    std::vector<Button> instruccionesBotones;

    sf::Texture pisoTextura;
    sf::Sprite pisoSprite;



public:
    GameState(GameStatus& status);

    void handleInput(sf::RenderWindow& window, StateManager& manager) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
};
