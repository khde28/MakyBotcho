#pragma once
#include <vector>

#include <SFML/System/Vector2.hpp>
#include "ParametrosNivel.h"

class GameStatus {
public:
    ParametrosNivel pNivel;

    int puntaje;

    // Podrías agregar incluso referencias o punteros a otras cosas globales:
    bool juegoPausado;
    bool victoria;
    bool derrota;

    GameStatus()
        : 
          puntaje(0),
          juegoPausado(false),
          victoria(false),
          derrota(false)
    {}
};
