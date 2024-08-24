#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>

// Función para cargar la imagen
bool loadImage(sf::Texture& texture, sf::Sprite& sprite, const std::string& filePath) {
    if (!texture.loadFromFile(filePath)) {
        std::cerr << "Error al cargar la imagen" << std::endl;
        return false;
    }
    sprite.setTexture(texture);

    // Centrar la imagen
    sf::Vector2u windowSize(800, 600); // Tamaño de la ventana
    sf::Vector2u imageSize = texture.getSize(); // Tamaño de la imagen
    sprite.setPosition((windowSize.x - imageSize.x) / 2.0f, (windowSize.y - imageSize.y) / 2.0f);

    return true;
}

// Función para leer una línea y almacenarla en un array de enteros
void parseLineToArray(const std::string& line, int* arr, int& size, int maxSize) {
    std::istringstream iss(line);
    int num;
    size = 0;
    while (iss >> num && size < maxSize) {
        arr[size++] = num;
    }
}

// Función para leer el archivo y almacenar los datos
bool readDataFromFile(const std::string& filePath, int* mainbot, int& mainbotSize, int* f1bot, int& f1botSize, int* buclebot, int& buclebotSize, int& counter) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo " << filePath << std::endl;
        return false;
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        switch (lineNumber) {
            case 0:
                parseLineToArray(line, mainbot, mainbotSize, 100); // Se asume un tamaño máximo de 100
                break;
            case 1:
                parseLineToArray(line, f1bot, f1botSize, 100);
                break;
            case 2:
                parseLineToArray(line, buclebot, buclebotSize, 100);
                break;
            case 3:
                counter = std::stoi(line);
                break;
            default:
                break;
        }
        lineNumber++;
    }
    return true;
}

// Función para verificar si se ha hecho clic en la imagen
bool isSpriteClicked(const sf::Sprite& sprite, const sf::Vector2i& mousePos) {
    return sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
}

int main() {
    // Crear la ventana
    sf::RenderWindow window(sf::VideoMode(800, 600), "Leer archivo al presionar imagen");

    // Cargar la imagen
    sf::Texture texture;
    sf::Sprite sprite;
    if (!loadImage(texture, sprite, "images/cargar.png")) {
        return -1;
    }

    // Variables para guardar los datos del archivo
    int mainbot[100], f1bot[100], buclebot[100];  // Se asume un tamaño máximo de 100
    int mainbotSize = 0, f1botSize = 0, buclebotSize = 0;
    int counter = 0;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Detectar clic en la imagen
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (isSpriteClicked(sprite, mousePos)) {
                    // Leer el archivo
                    if (readDataFromFile("datos.txt", mainbot, mainbotSize, f1bot, f1botSize, buclebot, buclebotSize, counter)) {
                        std::cout << "Datos leídos correctamente" << std::endl;
                    }
                }
            }
        }

        // Limpiar la ventana
        window.clear();
        // Dibujar la imagen
        window.draw(sprite);
        // Mostrar la ventana
        window.display();
    }

    // Imprimir los datos leídos (para verificación)
    std::cout << "mainbot: ";
    for (int i = 0; i < mainbotSize; i++) std::cout << mainbot[i] << " ";
    std::cout << "\nf1bot: ";
    for (int i = 0; i < f1botSize; i++) std::cout << f1bot[i] << " ";
    std::cout << "\nbuclebot: ";
    for (int i = 0; i < buclebotSize; i++) std::cout << buclebot[i] << " ";
    std::cout << "\ncounter: " << counter << std::endl;

    return 0;
}
