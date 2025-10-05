#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class GameManager {
    public:
        
        GameManager();
        ~GameManager();

        bool loadMapas3D(const std::string&);
        bool loadMapas2D(const std::string&);

        template <size_t N, size_t R, size_t C>
        bool loadMapa(const std::string&, int (&mapas)[N][R][C]);
        bool initMapas();

        void InitGame();
};