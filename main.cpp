#include "Game.h"
#include "ResourceManager.h"
#include <iostream>
#include <exception>

int main() {
    try {
        // Crear instancia del juego
        Game game;
        
        // Inicializar el juego
        if (!game.initialize()) {
            std::cerr << "Error: No se pudo inicializar el juego" << std::endl;
            return -1;
        }
        
        std::cout << "Makibot iniciado correctamente" << std::endl;
        
        // Ejecutar el bucle principal del juego
        game.run();
        
        std::cout << "Makibot cerrado correctamente" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error fatal: " << e.what() << std::endl;
        return -1;
    } catch (...) {
        std::cerr << "Error desconocido durante la ejecución" << std::endl;
        return -1;
    }
    
    return 0;
}