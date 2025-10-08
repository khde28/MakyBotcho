#pragma once
#include "State.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>

class PauseState : public State {
private:
    sf::Font font;
    sf::Text pauseText;

    sf::Texture continuaTextura;
    sf::Texture areaPausaTextura;
    sf::Texture selectNivelTextura;

public:
    PauseState();

    void handleInput(sf::RenderWindow& window, StateManager& manager) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
};
