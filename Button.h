#pragma once
#include <SFML/Graphics.hpp>

class Button{
    public:
    Button() = default;
    sf::Sprite sprite;
    int id;
    bool isHovered = false;
};