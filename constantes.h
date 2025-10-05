#pragma once

namespace Constantes{
    inline constexpr int MAX_LEVELS = 10;
    inline constexpr int GRIDSIZE = 8;
    inline constexpr int WINDOW_WIDTH = 1000;

    extern int mapas2d[MAX_LEVELS][GRIDSIZE][GRIDSIZE];
    extern int mapas3d[MAX_LEVELS][GRIDSIZE][GRIDSIZE];
}