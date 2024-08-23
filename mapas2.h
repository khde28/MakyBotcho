#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

namespace mapLoaded {
    inline int mapa2[1][8][8] = {{{0}}}; // Matriz global

    inline bool loadFile(const std::string& filename) {
        std::ifstream file(filename);

        if (!file.is_open()) {
            std::cerr << "Error al abrir el archivo: " << filename << std::endl;
            return false;
        }

        std::string line;
        int row = 0;

        while (std::getline(file, line) && row < 8) {
            std::istringstream iss(line);
            int value;
            int col = 0;

            while (iss >> value && col < 8) {
                mapa2[0][row][col] = value;
                col++;
            }

            if (col != 8) {
                std::cerr << "Error: La línea " << row + 1 << " no tiene 8 columnas." << std::endl;
                file.close();
                return false;
            }

            row++;
        }

        if (row != 8) {
            std::cerr << "Error: El archivo no tiene 8 filas." << std::endl;
            return false;
        }

        file.close();
        return true;
    }
}

