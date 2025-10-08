#pragma once
#include "State.h"
#include "StateManager.h"
#include <SFML/Graphics.hpp>

class MenuState : public State {
private:
    sf::Font font;
    sf::Text title;
    sf::Text startText;
    sf::Text exitText;

public:
    MenuState();

    void handleInput(sf::RenderWindow& window, StateManager& manager) override;
    void update(float dt) override;
    void render(sf::RenderWindow& window) override;
};
