#pragma once

#include <SFML/Graphics.hpp>
#include "GameStatus.h"

class StateManager; // Declaración adelantada

// Clase base abstracta para todos los estados (menú, pausa, juego, etc.)
class State {
    protected:
        GameStatus& gameStatus;
public:
    State(GameStatus& status) : gameStatus(status) {}
    virtual ~State() = default;

    virtual void handleInput(sf::RenderWindow& window, StateManager& manager) = 0;
    virtual void update(float dt) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
};
