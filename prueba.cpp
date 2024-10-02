#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Crear ventana
    sf::RenderWindow window(sf::VideoMode(400, 600), "Simulador de Semaforo");

    // Cargar las texturas
    sf::Texture redTexture, orangeTexture, greenTexture;

    if (!redTexture.loadFromFile("images/srojo.png") ||
        !orangeTexture.loadFromFile("images/snaranja.png") ||
        !greenTexture.loadFromFile("images/sverde.png")) {
        std::cerr << "Error al cargar las imágenes del semáforo" << std::endl;
        return -1;
    }

    sf::Sprite SpriteSemaforo(redTexture);

    SpriteSemaforo.setPosition(100,100);

    // Temporizador
    sf::Clock clock;
    int state = 0;  // 0: Rojo, 1: Naranja, 2: Verde
    float redTime = 5.0f;    // 5 segundos en rojo
    float orangeTime = 2.0f; // 2 segundos en naranja
    float greenTime = 5.0f;  // 5 segundos en verde

    while (window.isOpen()) {
        // Manejar eventos
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Actualizar temporizador
        float timeSemaforo = clock.getElapsedTime().asSeconds();

        // Cambiar de estado según el tiempo
        if (state == 0 && timeSemaforo >= redTime) {
            state = 1;
            SpriteSemaforo.setTexture(orangeTexture);
            clock.restart();
        } else if (state == 1 && timeSemaforo >= orangeTime) {
            state = 2;
            SpriteSemaforo.setTexture(greenTexture);
            clock.restart();
        } else if (state == 2 && timeSemaforo >= greenTime) {
            SpriteSemaforo.setTexture(redTexture);
            state = 0;
            clock.restart();
        }

        // Limpiar ventana
        window.clear();

        window.draw(SpriteSemaforo);

        window.display();
    }

    return 0;
}
