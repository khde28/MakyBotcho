#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "mapas.h"
#include "mapas2.h"
#include "analizador.h"
#include "converttxt.h"
#include "get_map_sol.h"
//------------------------------------------------------------------------------------------------------------
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sstream>

using namespace std;
const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;
const float BUTTON_SCALE = 1.06f;

void escribirInstruccion(int numero, std::ofstream &archivo)
{
    switch (numero)
    {
    case 1:
        archivo << "avanzar;" << std::endl;
        break;
    case 2:
        archivo << "girarDerecho;" << std::endl;
        break;
    case 3:
        archivo << "girarIzquierda;" << std::endl;
        break;
    case 4:
        archivo << "encender;" << std::endl;
        break;
    default:
        std::cerr << "Número inválido en el array: " << numero << std::endl;
        break;
    }
}

void guardarInstrucciones(int *array, int tamaño, int *array2, int tamaño2, int *array3, int tamaño3, const std::string &nombreArchivo, int iter)
{
    std::ofstream archivo(nombreArchivo, std::ios::out);
    if (!archivo)
    {
        std::cerr << "No se pudo abrir el archivo para escribir." << std::endl;
        return;
    }
    archivo << "iniciar()" << std::endl;
    for (int i = 0; i < tamaño; ++i)
    {
        escribirInstruccion(array[i], archivo);
        if (array[i] == 5)
        {
            for (int j = 0; j < tamaño2; ++j)
            {
                escribirInstruccion(array2[j], archivo);
            }
        }
        if (array[i] == 6)
        {
            for (int g = 0; g < iter; ++g)
            {
                for (int k = 0; k < tamaño3; ++k)
                {
                    escribirInstruccion(array3[k], archivo);
                    if (array3[k] == 5)
                    {
                        for (int m = 0; m < tamaño2; ++m)
                        {
                            escribirInstruccion(array2[m], archivo);
                        }
                    }
                }
            }
        }
    }
    archivo << "." << std::endl;

    archivo.close();
}

void imprimirArray(int array[], int size)
{
    std::cout << "Contenido del array: ";
    for (int i = 0; i < size; ++i)
    {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

void Inicializar(int arr[], int size)
{

    for (int i = 0; i < size; ++i)
    {
        arr[i] = 0;
    }
}

void FIFOdelete(int array[], int &tamano)
{
    for (int i = tamano - 1; i >= 0; --i)
    {
        if (array[i] != 0)
        {
            array[i] = 0;
            break;
        }
    }
}

// Función para dibujar las imágenes en base al array
void dibujarImagenes(sf::RenderWindow &window, const std::vector<sf::Texture> &texturas, int array[], int tamano, int xx, int yy, sf::Event event, bool &estado)
{
    // Tamaño de cada imagen (asumimos que todas tienen el mismo tamaño)
    sf::Vector2u imageSize = texturas[0].getSize();
    int imageWidth = imageSize.x;
    int imageHeight = imageSize.y;

    int offsetX = xx; // Desplazamiento en x
    int offsetY = yy; // Desplazamiento en y

    for (int i = 0; i < tamano; ++i)
    {
        if (array[i] >= 0 && array[i] < texturas.size())
        {
            sf::Sprite sprite(texturas[array[i]]);
            sprite.setPosition(offsetX + (i % 4) * imageWidth, offsetY + (i / 4) * imageHeight);
            window.draw(sprite);
        }
        else
        {
            std::cerr << "Valor de índice fuera de rango: " << array[i] << std::endl;
        }
    }
    static int esc = 30;
    static int esc2 = 30;

    sf::RectangleShape button(sf::Vector2f(esc, esc));
    sf::RectangleShape button2(sf::Vector2f(esc2, esc2));
    button.setPosition((offsetX + (0 % 4) * imageWidth) - 40, offsetY + (0 / 4) * imageHeight);
    button2.setPosition((offsetX + (0 % 4) * imageWidth) - 40, (offsetY + (0 / 4) * imageHeight) + 40);
    button.setFillColor(!estado ? sf::Color::Red : sf::Color::Red);
    button2.setFillColor(!estado ? sf::Color::Green : sf::Color::Yellow);

    static bool buttonPressed = false;
    static bool button2Pressed = false;

    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (button.getGlobalBounds().contains(mousePosF) && !buttonPressed)
        {
            buttonPressed = true;
            FIFOdelete(array, tamano);
            button.setFillColor(sf::Color::White);
            esc = 28;
        }

        if (button2.getGlobalBounds().contains(mousePosF) && !button2Pressed)
        {
            button2Pressed = true;
            estado = !estado; // Cambia el estado
            esc2 = 28;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        if (buttonPressed)
        {
            buttonPressed = false; // Reinicia el estado al soltar el botón
            esc = 30;
        }
        if (button2Pressed)
        {
            button2Pressed = false; // Reinicia el estado al soltar el botón
            esc2 = 30;
        }
    }

    window.draw(button);
    window.draw(button2);
}

struct Button
{
    sf::Sprite sprite;
    int id;
    bool isHovered = false;
};

void InsertarInstru(int array[], int size, int valor)
{
    for (int i = 0; i < size; ++i)
    {
        if (array[i] == 0)
        {
            array[i] = valor;
            break;
        }
    }
}

void scaleOnHover(sf::Sprite &sprite, sf::RenderWindow &window)
{
    if (sprite.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
    {
        sprite.setScale(0.9f, 0.9f);
    }
    else
    {
        sprite.setScale(0.8f, 0.8f);
    }
}

//----------------------------------------------------------------------------------------------------------------

#define PI 3.14159265f

using namespace std;
using namespace sf;

const float lado = 50.f;
const int gridSize = 8;
const float xIso = sqrt(2 * lado * lado);
const float yIso = xIso / 2.f;
const float ladoIso = sqrt(xIso * xIso / 4.f + yIso * yIso / 4.f);
const float angXY = atan2f(xIso, yIso);
const float angYX = atan2f(yIso, xIso);
const Vector2f pos_origin(300.f, 150.f);

Vector2i calculateGridIndices(const Vector2f &position, const Vector2f &gridOrigin)
{
    Vector2f relativePosition = position - gridOrigin;
    // cout<<"posicionxy: "<<position.x<<"  "<<position.y<<endl;
    float ang_c1 = angXY - atan2f(abs(relativePosition.x), relativePosition.y);
    float ang_c2 = 2.f * angXY - ang_c1;
    float distX, distY;
    // cout<<ang_c1*180.f/PI<<endl;
    float hipo = sqrt(relativePosition.x * relativePosition.x + relativePosition.y * relativePosition.y);

    if (relativePosition.x <= 0)
    {
        distX = (hipo / sin(PI - 2.f * angXY)) * sin(ang_c1);
        distY = (hipo / sin(PI - 2.f * angXY)) * sin(ang_c2);
    }
    else
    {
        distY = (hipo / sin(PI - 2.f * angXY)) * sin(ang_c1);
        distX = (hipo / sin(PI - 2.f * angXY)) * sin(ang_c2);
    }
    // cout<<"distancia xy isometricas"<<distX<<" "<<distY<<endl;
    Vector2f transformedPosition(
        distX / ladoIso,
        distY / ladoIso);

    int posXIso = floor(transformedPosition.x);
    int posYISo = floor(transformedPosition.y);

    return Vector2i(posXIso, posYISo);
}

void crearSpritesPiso(Sprite tiles[][gridSize], const int matriz3D[][gridSize], Texture &texturaLozaAzul, Texture &texturaPiso)
{
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (matriz3D[i][j] == -1)
            {
                tiles[i][j].setTexture(texturaLozaAzul);
            }
            else
            {
                tiles[i][j].setTexture(texturaPiso);
            }
            tiles[i][j].setPosition(i * lado, j * lado);
        }
    }
}

// Función para crear los sprites de los bloques
void crearSpritesBloques(vector<Sprite> &bloques, const int matriz3D[][gridSize], Texture &texturaBloque)
{
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (matriz3D[i][j] != 0)
            {
                for (int k = 1; k <= matriz3D[i][j]; ++k)
                {
                    Sprite bloque(texturaBloque);
                    bloque.setPosition(i * lado - (8.0f * k), j * lado - (8.0f * k));
                    bloques.push_back(bloque);
                }
            }
        }
    }
}

// Función para crear los sprites en una matriz 2D
void configurarSprites(sf::Sprite tiles2d[][gridSize], const int matriz2D[][gridSize], sf::Texture &texturaBloque2d, sf::Texture &texturaLozaAzul2D, sf::Texture &texturaPiso2d)
{
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (matriz2D[i][j] >= 1)
            {
                tiles2d[i][j].setTexture(texturaBloque2d);
            }
            else if (matriz2D[i][j] == -1)
            {
                tiles2d[i][j].setTexture(texturaLozaAzul2D);
            }
            else
            {
                tiles2d[i][j].setTexture(texturaPiso2d);
            }
            tiles2d[i][j].setPosition(50 + i * 15, 40 + j * 15);
        }
    }
}
bool isThere(const std::vector<int>& vec, int value) {
    return std::find(vec.begin(), vec.end(), value) != vec.end();
}
void configurarSpritesPaused(vector<int>& optPath, sf::Sprite tiles2d[][gridSize], const int matriz2D[][gridSize], sf::Texture &texturaBloque2d, sf::Texture &texturaLozaAzul2D, sf::Texture &texturaPiso2d, sf::Texture &textureLozaRojo2D)
{
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (matriz2D[i][j] >= 1)
            {
                tiles2d[i][j].setTexture(texturaBloque2d);
            }
            else if (matriz2D[i][j] == -1)
            {
                tiles2d[i][j].setTexture(texturaLozaAzul2D);
            }
            else
            {

                if (isThere(optPath, i*8+j))
                {
                    tiles2d[i][j].setTexture(textureLozaRojo2D);
                }else{

                    tiles2d[i][j].setTexture(texturaPiso2d);
                }
                
            }
            tiles2d[i][j].setPosition(50 + i * 15, 40 + j * 15);
        }
    }
}
void moveRobot(Sprite &makibot, const Vector2f &targetPosition, Vector2f &currentPosition, float xIso, float yIso)
{
    Vector2f movement(0.f, 0.f);

    if (targetPosition.x > currentPosition.x)
        movement.x = xIso / 2.f / 50.f;
    else if (targetPosition.x < currentPosition.x)
        movement.x = -xIso / 2.f / 50.f;

    if (targetPosition.y > currentPosition.y)
        movement.y = yIso / 2.f / 50.f;
    else if (targetPosition.y < currentPosition.y)
        movement.y = -yIso / 2.f / 50.f;

    makibot.move(movement);
    // currentPosition = makibot.getPosition();  // Actualiza la posición actual después de mover
}

void stopMovement(Sprite &makibot, const Vector2f &targetPosition, const Vector2f &currentPosition, int &currentFrame, bool miraNE, bool miraNO, bool miraSO, bool miraSE, bool &moving, const vector<IntRect> &framesB, const vector<IntRect> &framesF)
{
    if (abs(targetPosition.x - currentPosition.x) < 1.f && abs(targetPosition.y - currentPosition.y) < 1.f)
    {
        moving = false;
        currentFrame = 0;
        makibot.setPosition(targetPosition);
        if (miraNE || miraNO)
        {
            makibot.setTextureRect(framesB[currentFrame]);
            std::cout << "arriba" << endl;
        }
        else if (miraSE || miraSO)
        {
            makibot.setTextureRect(framesF[currentFrame]);
            std::cout << "abajo" << endl;
        }
    }
}

void updateBlocks(vector<Sprite> &bloques2, const int mapas[][gridSize], int gridSize, const Texture &texturaBloque, float lado, int posXIso, int posYISo)
{
    bloques2.clear();
    for (int i = 0; i < gridSize; ++i)
    {
        for (int j = 0; j < gridSize; ++j)
        {
            if (posXIso <= i && posYISo <= j)
            {
                for (int k = 1; k <= mapas[i][j]; ++k)
                {
                    Sprite bloque(texturaBloque);
                    bloque.setPosition(i * lado - (8.0f * k), j * lado - (8.0f * k));
                    bloques2.push_back(bloque);
                }
            }
        }
    }
}

struct Estado
{
    bool miraNE;
    bool miraNO;
    bool miraSO;
    bool miraSE;
};

Estado estados[] = {
    {false, false, false, true}, // 0: Mira hacia el abajo
    {false, false, true, false}, // 1: Mira hacia el izquierda
    {false, true, false, false}, // 2: Mira hacia arriba
    {true, false, false, false}  // 3: Mira hacia el derecha
};

// Función para cambiar la dirección cíclicamente
void updateDirection(int &contador, int movimiento)
{ // moviento es el valor del arreglo
    if (movimiento == 2)
    {
        // Incrementar el contador cíclicamente
        contador = (contador + 1) % 4;
    }
    else if (movimiento == 3)
    {
        // Decrementar el contador cíclicamente
        contador = (contador - 1 + 4) % 4; // +4 para manejar el caso negativo
    }
}

// Función para mover el robot
void move2(Vector2f &targetPosition, bool &moving, const Estado &estado, float xIso, float yIso)
{
    if (estado.miraNE)
    {
        targetPosition.x += xIso / 2.f;
        targetPosition.y -= yIso / 2.f;
    }
    else if (estado.miraNO)
    {
        targetPosition.x -= xIso / 2.f;
        targetPosition.y -= yIso / 2.f;
    }
    else if (estado.miraSO)
    {
        targetPosition.x -= xIso / 2.f;
        targetPosition.y += yIso / 2.f;
    }
    else if (estado.miraSE)
    {
        targetPosition.x += xIso / 2.f;
        targetPosition.y += yIso / 2.f;
    }
    moving = true;
}
//------------------------------------------Screen Management---------------------------------
class MainPage
{
public:
    MainPage()
    {

        if (!fuente.loadFromFile("digital.ttf"))
        {
            cout << "error" << endl;
        }

        welcomeText.setFont(fuente);
        welcomeText.setString("MAKYBOTCHO");
        welcomeText.setCharacterSize(50);
        welcomeText.setFillColor(sf::Color::White);

        buttonGame.setSize(sf::Vector2f(400, 100));
        buttonGame.setFillColor(sf::Color(125, 125, 125, 200));

        buttonCustom.setSize(sf::Vector2f(400, 100));
        buttonCustom.setFillColor(sf::Color(125, 125, 125, 200));

        buttonGameText.setFont(fuente);
        buttonGameText.setString("NORMAL MODE");
        buttonGameText.setCharacterSize(35);
        buttonGameText.setFillColor(sf::Color(255, 255, 255));

        buttonCustomText.setFont(fuente);
        buttonCustomText.setString("CUSTOM MODE");
        buttonCustomText.setCharacterSize(35);
        buttonCustomText.setFillColor(sf::Color(255, 255, 255));
    }

    void handleEvent(const sf::Event &event, int &mode, RenderWindow &window)
    {
        if (event.type == sf::Event::MouseButtonPressed)
        {
            // sf::Vector2i mousePos = sf::Mouse::getPosition();
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            // std::cout << "Mouse position: " << mousePos.x << ", " << mousePos.y << std::endl;
            // std::cout << "Button position: " << buttonGame.getPosition().x << ", " << buttonGame.getPosition().y << std::endl;

            // sf::FloatRect bounds = buttonGame.getGlobalBounds();
            // std::cout << "Button bounds: Left: " << bounds.left << ", Top: " << bounds.top << ", Width: " << bounds.width << ", Height: " << bounds.height << std::endl;

            if (buttonGame.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
            {
                mode = 1;
                cout << "presiona game" << endl;
            }
            else if (buttonCustom.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)))
            {
                mode = 2;
                cout << "presiona custom" << endl;
            }
        }
    }

    void draw(sf::RenderWindow &window)
    {
        window.clear(sf::Color::Blue);

        window.draw(welcomeText);

        window.draw(buttonGame);
        window.draw(buttonGameText);
        window.draw(buttonCustom);
        window.draw(buttonCustomText);
    }

    void allSetPosition(sf::RenderWindow &window)
    {
        textPosition(window);
        buttonCustomModePosition(window);
        buttonGameModePosition(window);
    }
    void textPosition(sf::RenderWindow &window)
    {
        sf::FloatRect textBounds = welcomeText.getLocalBounds();
        float windowWidth = window.getSize().x;
        float windowHeight = window.getSize().y;

        welcomeText.setPosition(
            (windowWidth - textBounds.width) / 2.0f - textBounds.left,
            (windowHeight * 1 / 4 - textBounds.height) / 2.0f - textBounds.top);
    }
    void buttonGameModePosition(sf::RenderWindow &window)
    {
        float windowWidth = window.getSize().x;
        float windowHeight = window.getSize().y;

        buttonGame.setPosition(
            (windowWidth - buttonGame.getSize().x) / 2.0f,
            (windowHeight * 2 / 4 - buttonGame.getSize().y));

        sf::FloatRect textBounds = buttonGameText.getLocalBounds();
        buttonGameText.setPosition(
            buttonGame.getPosition().x + (buttonGame.getSize().x - textBounds.width) / 2.0f - textBounds.left,
            buttonGame.getPosition().y + (buttonGame.getSize().y - textBounds.height) / 2.0f - textBounds.top);
    }

    void buttonCustomModePosition(sf::RenderWindow &window)
    {
        float windowWidth = window.getSize().x;
        float windowHeight = window.getSize().y;

        buttonCustom.setPosition(
            (windowWidth - buttonCustom.getSize().x) / 2.0f,
            (windowHeight * 3 / 4 - buttonCustom.getSize().y));

        sf::FloatRect textBounds = buttonCustomText.getLocalBounds();
        buttonCustomText.setPosition(
            buttonCustom.getPosition().x + (buttonCustom.getSize().x - textBounds.width) / 2.0f - textBounds.left,
            buttonCustom.getPosition().y + (buttonCustom.getSize().y - textBounds.height) / 2.0f - textBounds.top);
    }

private:
    Font fuente;
    sf::Text welcomeText;
    sf::RectangleShape buttonCustom;
    sf::RectangleShape buttonGame;
    sf::Text buttonCustomText;
    sf::Text buttonGameText;
};

// Clase para manejar el juego
class GameScreen
{
public:
    void draw(sf::RenderWindow &window)
    {
        window.clear(sf::Color::Black);
        // Aquí dibuja la pantalla del juego
    }
};
//--------------------------------------------------------Game Mode ---------------------------------------------

void gameMode(sf::RenderWindow &window)
{
    // contador
    sf::Texture textures[6];
    for (int i = 0; i < 6; ++i)
    {
        if (!textures[i].loadFromFile("images/" + std::to_string(i) + ".png"))
        {
            std::cerr << "Error cargando la imagen " << i << ".png" << std::endl;
            return;
        }
    }

    sf::Texture iniciarTexture;
    sf::Texture incrementTexture;
    sf::Texture decrementTexture;
    if (!iniciarTexture.loadFromFile("images/go.png") || !incrementTexture.loadFromFile("images/imageincrement.png") || !decrementTexture.loadFromFile("images/imagendecrement.png"))
    {
        std::cerr << "Error cargando la imagen imageincrement.png o decrement" << std::endl;
        return;
    }

    int clicks = 0;
    sf::Sprite iniciarButton(iniciarTexture);
    iniciarButton.setPosition(420, 480);
    string lenguajeintermedio;

    // imagenes del contador
    int conty = 485;
    int counter = 0; // importante
    sf::Sprite numberSprite(textures[counter]);
    numberSprite.setPosition(620, conty);

    sf::Sprite incrementButton(incrementTexture);
    incrementButton.setPosition(620, conty - 20); // Encima del contador

    sf::Sprite decrementButton(decrementTexture);
    decrementButton.setPosition(620, conty + 43);

    // Cargar texturas
    std::vector<sf::Texture> buttonTexture(8);

    if (!buttonTexture[0].loadFromFile("images/vacio.png") ||
        !buttonTexture[5].loadFromFile("images/f1.png") ||
        !buttonTexture[3].loadFromFile("images/giroleft.png") ||
        !buttonTexture[2].loadFromFile("images/giroright.png") ||
        !buttonTexture[1].loadFromFile("images/avanzar.png") ||
        !buttonTexture[6].loadFromFile("images/bucle.png") ||
        !buttonTexture[4].loadFromFile("images/foco.png") ||
        !buttonTexture[7].loadFromFile("images/current.png"))
    {
        return;
    }

    std::vector<Button> buttons(6);
    buttons[0] = {sf::Sprite(buttonTexture[6]), 6};
    buttons[1] = {sf::Sprite(buttonTexture[5]), 5};
    buttons[2] = {sf::Sprite(buttonTexture[4]), 4};
    buttons[3] = {sf::Sprite(buttonTexture[3]), 3};
    buttons[4] = {sf::Sprite(buttonTexture[2]), 2};
    buttons[5] = {sf::Sprite(buttonTexture[1]), 1};

    for (size_t i = 0; i < buttons.size(); ++i)
    {
        buttons[i].sprite.setPosition(WINDOW_WIDTH - buttons[i].sprite.getGlobalBounds().width * (i + 1) - 10 * i, 10);
    }

    std::vector<int> pressedButtons;
    int mainbot[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // importante
    int f1bot[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int buclebot[4] = {0, 0, 0, 0};

    bool boolmain = false;
    bool boolf1 = false;
    bool boolbucle = false;
    bool clicDerechoPresionado = false;

    //-----------------------------------------------------------------------------------------------------------

    std::vector<sf::SoundBuffer> soundBuffers(3);
    std::vector<sf::Sound> sounds(3);

    // Cargar los archivos de audio
    if (!soundBuffers[0].loadFromFile("sonido_mapa.ogg") ||
        !soundBuffers[1].loadFromFile("sonido_mapa2.ogg") ||
        !soundBuffers[2].loadFromFile("sonido_mapa3.ogg"))
    {
        std::cerr << "Error al cargar los archivos de audio" << std::endl;
        return;
    }

    // Configurar los sonidos
    for (size_t i = 0; i < sounds.size(); ++i)
    {
        sounds[i].setVolume(25.f);
        sounds[i].setBuffer(soundBuffers[i]);
        sounds[i].setLoop(false); // No repetir en bucle
    }

    size_t currentSound = 0;
    bool soundPlaying = false;
    sf::Clock clock; // Para medir el tiempo

    int mapaActual = 0; // Índice del mapa actual

    // Crear la ventana
    window.setFramerateLimit(60);
    // Establecer el color gris oscuro
    sf::Color grisOscuro(169, 169, 169); // RGB para gris oscuro

    // Coordenadas de la vista isométrica
    Transform isoTransform;
    isoTransform.translate(300.f, 150.f);
    isoTransform.scale(1.f, 0.5f);
    isoTransform.rotate(45.f);

    // Cargar la textura del piso
    Texture texturaPiso;
    if (!texturaPiso.loadFromFile("images/loza_verde.png"))
    {
        return;
    }
    Texture texturaPiso2d;
    if (!texturaPiso2d.loadFromFile("images/loza_verde2d.png"))
    {
        return;
    }
    // Cargar la textura del bloque
    Texture texturaBloque;
    if (!texturaBloque.loadFromFile("images/loza_naranja.png"))
    {
        return;
    }
    Texture texturaBloque2d;
    if (!texturaBloque2d.loadFromFile("images/loza_naranja2d.png"))
    {
        return;
    }

    // Cargar la textura del punto final del mapa3D
    Texture texturaLozaAzul;
    if (!texturaLozaAzul.loadFromFile("images/loza_azul.png"))
    {
        return;
    }

    // Crear el sprite del piso

    Sprite tiles[gridSize][gridSize];

    crearSpritesPiso(tiles, mapas[mapaActual], texturaLozaAzul, texturaPiso);

    // Vector de sprites para los bloques
    vector<Sprite> bloques;
    // Vector para los bloques q se sobreponen al makibot
    vector<Sprite> bloques2;
    // Inicializar los sprites del piso y los bloques
    crearSpritesBloques(bloques, mapas[mapaActual], texturaBloque);

    // Figura del makibot en 2D
    CircleShape makibot2D;
    makibot2D.setRadius(5.f);
    makibot2D.setFillColor(Color::White);
    makibot2D.setOrigin(makibot2D.getRadius(), makibot2D.getRadius());
    makibot2D.setPosition(57.5f, 47.5f);

    // Cargar la textura del punto final del mapa2D
    Texture texturaLozaAzul2D;
    if (!texturaLozaAzul2D.loadFromFile("images/loza_azul2d.png"))
    {
        return;
    }

    // crear el minimapa en 2d
    Sprite tiles2d[gridSize][gridSize];
    configurarSprites(tiles2d, matrices2d[mapaActual], texturaBloque2d, texturaLozaAzul2D, texturaPiso2d);

    const int frameWidth = 40;
    const int frameHeight = 30;
    // SPRITES FRONTAL
    Texture spriteMaki;
    if (!spriteMaki.loadFromFile("images/sprite_carritos.png"))
    {
        return;
    }

    // Crear el sprite de la animación
    Sprite makibot;
    makibot.setTexture(spriteMaki);

    // Definir los rectángulos para cada fotograma de la animación

    vector<IntRect> framesF;
    framesF.push_back(IntRect(0, 0, frameWidth, frameHeight));
    framesF.push_back(IntRect(frameWidth, 0, frameWidth, frameHeight));
    framesF.push_back(IntRect(2 * frameWidth, 0, frameWidth, frameHeight));

    IntRect focoF = IntRect(3 * frameWidth, 0, frameWidth, frameHeight);

    vector<IntRect> framesB;
    framesB.push_back(IntRect(0, frameHeight, frameWidth, frameHeight));
    framesB.push_back(IntRect(frameWidth, frameHeight, frameWidth, frameHeight));
    framesB.push_back(IntRect(2 * frameWidth, frameHeight, frameWidth, frameHeight));

    IntRect focoB = IntRect(3 * frameWidth, frameHeight, frameWidth, frameHeight);

    // Ajustar la posición inicial del sprite para dibujarse desde la parte inferior
    makibot.setOrigin(20.0f, 30.f);
    makibot.setPosition(300.f, 160.f + yIso / 2.f);

    // primera aparicion del makibot
    makibot.setTextureRect(framesF[0]);

    // Variables para la animación
    int currentFrame = 0;
    Clock animationClock;
    float animationSpeed = 0.3f;

    // Variables para controlar el movimiento
    bool moving = false;
    Vector2f targetPosition = makibot.getPosition();

    bool miraNE = false;
    bool miraNO = false;
    bool miraSO = false;
    bool miraSE = true;

    // int CountMovimiento = 0;

    int contador = 0; // Inicialmente mirando hacia el Sur

    int contadorMovimientos = 0;
    int contadorMovf1 = 0;
    int contadorMovbucle = 0;

    int currentIteraciones = 0;

    bool girando = false;
    // Bucle principal
    bool booliniciar = false;

    int movimiento = 0;
    int movimientof1 = 0;
    int movimientoBucle = 0;

    int lastmov;
    int lastmovf1;
    int lastmovbucle;

    bool colisionando = false;

    MainPage mainPage;
    bool isGame = false;
    while (window.isOpen())
    {
        // Lógica de reproducción de sonido
        if (!soundPlaying)
        {
            sounds[currentSound].setVolume(5.f);
            sounds[currentSound].play();
            soundPlaying = true;
            clock.restart(); // Reiniciar el reloj
        }

        // Verificar si el sonido actual ha terminado
        if (clock.getElapsedTime() >= sounds[currentSound].getBuffer()->getDuration())
        {
            currentSound = (currentSound + 1) % sounds.size(); // Cambiar al siguiente sonido
            soundPlaying = false;                              // Permitir que el siguiente sonido se reproduzca
        }

        // gaaaaaaaaaaa
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            //-------------------------intrucciones-----------------------------------------
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (iniciarButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                {
                    guardarInstrucciones(mainbot, 12, f1bot, 8, buclebot, 4, "instrucciones.txt", counter);
                    lenguajeintermedio = txtConvertstring("instrucciones.txt");
                    analizadorSyx(lenguajeintermedio);
                    cout << lenguajeintermedio << endl;
                    booliniciar = true;
                }
                if (incrementButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                {
                    if (counter < 5)
                    {
                        counter++;
                        numberSprite.setTexture(textures[counter]);
                    }
                }

                if (decrementButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                {
                    if (counter > 0)
                    {
                        counter--;
                        numberSprite.setTexture(textures[counter]);
                    }
                }
            }
        }
        //-------------------------intrucciones-----------------------------------------
        // Procesar interacción del ratón

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (auto &button : buttons)
        {
            sf::FloatRect bounds = button.sprite.getGlobalBounds();
            if (bounds.contains(static_cast<sf::Vector2f>(mousePos)))
            {
                if (!button.isHovered)
                {
                    button.sprite.setScale(BUTTON_SCALE, BUTTON_SCALE);
                    button.isHovered = true;
                }

                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        clicDerechoPresionado = true;
                    }
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    if (event.mouseButton.button == sf::Mouse::Left && clicDerechoPresionado == true)
                    {
                        if (boolmain)
                        {
                            InsertarInstru(mainbot, 12, button.id);
                            imprimirArray(mainbot, 12);
                        }
                        if (boolf1)
                        {
                            InsertarInstru(f1bot, 8, button.id);
                            imprimirArray(f1bot, 8);
                        }
                        if (boolbucle)
                        {
                            InsertarInstru(buclebot, 4, button.id);
                            imprimirArray(buclebot, 4);
                        }
                        clicDerechoPresionado = false;
                    }
                }
            }
            else
            {
                if (button.isHovered)
                {
                    button.sprite.setScale(1.f, 1.f);
                    button.isHovered = false;
                }
            }
        }
        //-------------------------intrucciones-----------------------------------------

        if (booliniciar)
        {
            if (contadorMovimientos < sizeof(mainbot) / sizeof(mainbot[0]) && moving == false && girando == false && colisionando == false)
            {
                if (mainbot[contadorMovimientos] != 7)
                {
                    movimiento = mainbot[contadorMovimientos];
                }
                if (contadorMovimientos != 0)
                {
                    if (mainbot[contadorMovimientos - 1] == 7)
                    {
                        cout << "cambio " << endl;
                        mainbot[contadorMovimientos - 1] = lastmov;
                    }
                }
                // int movimiento = mainbot[contadorMovimientos];

                if (movimiento == 2 || movimiento == 3)
                {
                    lastmov = movimiento;
                    mainbot[contadorMovimientos] = 7;
                    // Cambia la dirección cíclicamente
                    updateDirection(contador, movimiento);
                    cout << contador << "----------------------" << endl;
                    miraNE = estados[contador].miraNE;
                    miraNO = estados[contador].miraNO;
                    miraSO = estados[contador].miraSO;
                    miraSE = estados[contador].miraSE;
                    girando = true;
                    contadorMovimientos++;
                }
                else if (movimiento == 1)
                {
                    lastmov = movimiento;
                    mainbot[contadorMovimientos] = 7;
                    // Mover en la dirección actual
                    Estado estado;
                    estado.miraNE = miraNE;
                    estado.miraNO = miraNO;
                    estado.miraSO = miraSO;
                    estado.miraSE = miraSE;

                    move2(targetPosition, moving, estado, xIso, yIso);
                    contadorMovimientos++;
                }
                else if (movimiento == 5)
                {
                    lastmov = movimiento;
                    mainbot[contadorMovimientos] = 7;

                    cout << "gaaaa" << endl;
                    if (contadorMovf1 < sizeof(f1bot) / sizeof(f1bot[0]) && moving == false && girando == false)
                    {

                        if (f1bot[contadorMovf1] != 7)
                        {
                            movimientof1 = f1bot[contadorMovf1];
                        }
                        if (contadorMovf1 != 0)
                        {
                            if (f1bot[contadorMovf1 - 1] == 7)
                            {
                                f1bot[contadorMovf1 - 1] = lastmovf1;
                            }
                        }

                        if (movimientof1 == 2 || movimientof1 == 3)
                        {
                            lastmovf1 = movimientof1;
                            f1bot[contadorMovf1] = 7;
                            // Cambia la dirección cíclicamente
                            updateDirection(contador, movimientof1);
                            cout << contador << "----------------------" << endl;
                            miraNE = estados[contador].miraNE;
                            miraNO = estados[contador].miraNO;
                            miraSO = estados[contador].miraSO;
                            miraSE = estados[contador].miraSE;
                            girando = true;
                            contadorMovf1++;
                        }
                        else if (movimientof1 == 1)
                        {
                            lastmovf1 = movimientof1;
                            f1bot[contadorMovf1] = 7;
                            // Mover en la dirección actual
                            Estado estado;
                            estado.miraNE = miraNE;
                            estado.miraNO = miraNO;
                            estado.miraSO = miraSO;
                            estado.miraSE = miraSE;

                            move2(targetPosition, moving, estado, xIso, yIso);
                            contadorMovf1++;
                        }
                        else if (movimientof1 == 0)
                        {
                            contadorMovf1 = 0;
                            contadorMovimientos++;
                        }

                        // Avanzar en el array de movimientos
                    }
                    else if (contadorMovf1 >= 8)
                    {
                        f1bot[contadorMovf1 - 1] = lastmovf1;
                        contadorMovf1 = 0;
                        contadorMovimientos++;
                    }
                }
                else if (movimiento == 6 && currentIteraciones < counter)
                {
                    lastmov = movimiento;
                    mainbot[contadorMovimientos] = 7;

                    cout << "bucleoco" << endl;
                    if (contadorMovbucle < sizeof(buclebot) / sizeof(buclebot[0]) && moving == false && girando == false)
                    {
                        if (buclebot[contadorMovbucle] != 7)
                        {
                            movimientoBucle = buclebot[contadorMovbucle];
                        }
                        if (contadorMovbucle != 0)
                        {
                            if (buclebot[contadorMovbucle - 1] == 7)
                            {
                                buclebot[contadorMovbucle - 1] = lastmovbucle;
                            }
                        }

                        if (movimientoBucle == 2 || movimientoBucle == 3)
                        {
                            lastmovbucle = movimientoBucle;
                            buclebot[contadorMovbucle] = 7;
                            // Cambia la dirección cíclicamente
                            updateDirection(contador, movimientoBucle);
                            cout << contador << "----------------------" << endl;
                            miraNE = estados[contador].miraNE;
                            miraNO = estados[contador].miraNO;
                            miraSO = estados[contador].miraSO;
                            miraSE = estados[contador].miraSE;
                            girando = true;
                            contadorMovbucle++;
                        }
                        else if (movimientoBucle == 1)
                        {
                            lastmovbucle = movimientoBucle;
                            buclebot[contadorMovbucle] = 7;
                            // Mover en la dirección actual
                            Estado estado;
                            estado.miraNE = miraNE;
                            estado.miraNO = miraNO;
                            estado.miraSO = miraSO;
                            estado.miraSE = miraSE;

                            move2(targetPosition, moving, estado, xIso, yIso);
                            contadorMovbucle++;
                        }
                        else if (movimientoBucle == 0)
                        {
                            contadorMovbucle = 0;
                            currentIteraciones++;
                        }

                        // Avanzar en el array de movimientos
                    }
                    else if (contadorMovbucle >= 4)
                    {
                        buclebot[contadorMovbucle - 1] = lastmovbucle;
                        contadorMovbucle = 0;
                        currentIteraciones++;
                    }
                }

                else if (currentIteraciones = counter)
                {
                    contadorMovimientos++;
                    currentIteraciones = 0;
                }
                else if (movimiento == 0)
                {
                    cout << "Llegue al final del array de movimientos" << endl;
                    contadorMovimientos++;
                    contadorMovimientos = 0;
                    booliniciar = false;
                }

                // Avanzar en el array de movimientos
                // contadorMovimientos++;
            }
            else if (!moving && !girando && !colisionando)
            {
                cout << "Llegue al final del array de movimientos" << endl;
                mainbot[contadorMovimientos - 1] = lastmov;
                contadorMovimientos = 0;
                booliniciar = false;
            }
        }

        // Actualizar la animación del sprite
        if (moving || colisionando)
        {
            // Verificar colisión con bloques
            Vector2f newPosition = targetPosition;
            int posXIso, posYISo;
            Vector2i indices = calculateGridIndices(newPosition, pos_origin);

            posXIso = indices.x;
            posYISo = indices.y;

            cout << posXIso << posYISo << endl;

            if (mapas[mapaActual][posXIso][posYISo] == -1 && posXIso != -1 && posYISo != -1)
            {
                bloques.clear();

                for (int i = 0; i < gridSize; ++i)
                {
                    for (int j = 0; j < gridSize; ++j)
                    {
                        tiles[i][j] = Sprite(); // Reasignar un sprite por defecto
                    }
                }

                for (int i = 0; i < gridSize; ++i)
                {
                    for (int j = 0; j < gridSize; ++j)
                    {
                        tiles2d[i][j] = Sprite(); // Reasignar un sprite por defecto
                    }
                }

                // Incrementar mapaActual y usar el operador módulo para reiniciar a 0 cuando se alcance el límite
                mapaActual = (mapaActual + 1) % 5;

                posXIso = 0;
                posYISo = 0;

                for (int i = 0; i < 12; ++i)
                {
                    mainbot[i] = 0;
                }
                for (int i = 0; i < 8; ++i)
                {
                    f1bot[i] = 0;
                }
                for (int i = 0; i < 4; ++i)
                {
                    buclebot[i] = 0;
                }
                contador = 0;
                cout << "hhhhh" << endl;
                makibot.setOrigin(20.f, 30.f);
                makibot.setPosition(300.f, 160.f + yIso / 2.f);

                // Actualizar targetPosition a la nueva posición inicial
                targetPosition = makibot.getPosition();

                crearSpritesPiso(tiles, mapas[mapaActual], texturaLozaAzul, texturaPiso);
                crearSpritesBloques(bloques, mapas[mapaActual], texturaBloque);
                configurarSprites(tiles2d, matrices2d[mapaActual], texturaBloque2d, texturaLozaAzul2D, texturaPiso2d);

                // Detener movimiento
                moving = false;
            }

            if (!colisionando)
            {
                if (!(posXIso >= 0 && posXIso < gridSize && posYISo >= 0 && posYISo < gridSize) ||
                    (mapas[mapaActual][posXIso][posYISo] != 0 && mapas[mapaActual][posXIso][posYISo] != -1))
                {
                    colisionando = true;
                    moving = false;
                    targetPosition = makibot.getPosition();
                }
                else
                {
                    if (animationClock.getElapsedTime().asSeconds() > animationSpeed)
                    {
                        if (miraNE || miraNO)
                        {
                            currentFrame = (currentFrame + 1) % framesB.size();
                            makibot.setTextureRect(framesB[currentFrame]);
                            cout << "arriba" << endl;
                        }
                        else if (miraSO || miraSE)
                        {
                            currentFrame = (currentFrame + 1) % framesF.size();
                            makibot.setTextureRect(framesF[currentFrame]);
                        }

                        animationClock.restart();
                    }

                    // movimeinto del car
                    Vector2f currentPosition = makibot.getPosition();
                    moveRobot(makibot, targetPosition, currentPosition, xIso, yIso);

                    // Detener el movimiento al alcanzar el objetivo

                    stopMovement(makibot, targetPosition, makibot.getPosition(), currentFrame, miraNE, miraNO, miraSO, miraSE, moving, framesB, framesF);
                    // posicion de makibot2D
                    makibot2D.setPosition(57.5f + 15.f * posXIso, 47.4f + 15.f * posYISo);

                    // mapa 2D , se verifica la posicion de los bloques y el makibot
                    updateBlocks(bloques2, mapas[mapaActual], gridSize, texturaBloque, lado, posXIso, posYISo);
                }
            }
            else if (colisionando == true)
            {
                if (animationClock.getElapsedTime().asSeconds() > 1.5)
                {
                    colisionando = false;
                    cout << "cambiando colisionando a false" << endl;
                    animationClock.restart();
                }
            }

            if (miraNE)
                makibot.setScale(1.f, 1.f);
            else if (miraNO)
                makibot.setScale(-1.f, 1.f);
            else if (miraSO)
                makibot.setScale(-1.f, 1.f);
            else if (miraSE)
                makibot.setScale(1.f, 1.f);
        }
        else if (girando)
        {
            if (animationClock.getElapsedTime().asSeconds() > 1.5)
            {
                if (miraNE || miraNO)
                {
                    makibot.setTextureRect(framesB[currentFrame]);
                }
                else if (miraSO || miraSE)
                {
                    makibot.setTextureRect(framesF[currentFrame]);
                }
                if (miraNE)
                    makibot.setScale(1.f, 1.f);
                else if (miraNO)
                    makibot.setScale(-1.f, 1.f);
                else if (miraSO)
                    makibot.setScale(-1.f, 1.f);
                else if (miraSE)
                    makibot.setScale(1.f, 1.f);
                girando = false;

                animationClock.restart();
            }
        }

        window.clear(grisOscuro);
        // dibujar piso 2d
        for (int i = 0; i < gridSize; ++i)
        {
            for (int j = 0; j < gridSize; ++j)
            {
                window.draw(tiles2d[i][j]);
            }
        }
        // Dibujar el piso y los bloques
        for (int i = 0; i < gridSize; ++i)
        {
            for (int j = 0; j < gridSize; ++j)
            {
                window.draw(tiles[i][j], isoTransform);
            }
        }
        for (auto &bloque : bloques)
        {
            window.draw(bloque, isoTransform);
        }

        window.draw(makibot);
        window.draw(makibot2D);

        for (auto &bloque : bloques2)
        {
            window.draw(bloque, isoTransform);
        }
        scaleOnHover(iniciarButton, window);

        scaleOnHover(incrementButton, window);
        scaleOnHover(decrementButton, window);

        window.draw(iniciarButton);
        window.draw(numberSprite);
        window.draw(incrementButton);
        window.draw(decrementButton);
        for (const auto &button : buttons)
        {
            window.draw(button.sprite);
        }
        dibujarImagenes(window, buttonTexture, mainbot, sizeof(mainbot) / sizeof(mainbot[0]), 720, 100, event, boolmain);
        dibujarImagenes(window, buttonTexture, f1bot, sizeof(f1bot) / sizeof(f1bot[0]), 720, 320, event, boolf1);
        dibujarImagenes(window, buttonTexture, buclebot, sizeof(buclebot) / sizeof(buclebot[0]), 720, 470, event, boolbucle);

        window.display();
    }

    return;
}

void customMode(sf::RenderWindow &window)
{

    // contador
    sf::Texture textures[6];
    for (int i = 0; i < 6; ++i)
    {
        if (!textures[i].loadFromFile("images/" + std::to_string(i) + ".png"))
        {
            std::cerr << "Error cargando la imagen " << i << ".png" << std::endl;
            return;
        }
    }

    sf::Texture iniciarTexture;
    sf::Texture incrementTexture;
    sf::Texture decrementTexture;
    if (!iniciarTexture.loadFromFile("images/go.png") || !incrementTexture.loadFromFile("images/imageincrement.png") || !decrementTexture.loadFromFile("images/imagendecrement.png"))
    {
        std::cerr << "Error cargando la imagen imageincrement.png o decrement" << std::endl;
        return;
    }

    int clicks = 0;
    sf::Sprite iniciarButton(iniciarTexture);
    iniciarButton.setPosition(420, 480);
    string lenguajeintermedio;

    // imagenes del contador
    int conty = 485;
    int counter = 0; // importante
    sf::Sprite numberSprite(textures[counter]);
    numberSprite.setPosition(620, conty);

    sf::Sprite incrementButton(incrementTexture);
    incrementButton.setPosition(620, conty - 20); // Encima del contador

    sf::Sprite decrementButton(decrementTexture);
    decrementButton.setPosition(620, conty + 43);

    // Cargar texturas
    std::vector<sf::Texture> buttonTexture(8);

    if (!buttonTexture[0].loadFromFile("images/vacio.png") ||
        !buttonTexture[5].loadFromFile("images/f1.png") ||
        !buttonTexture[3].loadFromFile("images/giroleft.png") ||
        !buttonTexture[2].loadFromFile("images/giroright.png") ||
        !buttonTexture[1].loadFromFile("images/avanzar.png") ||
        !buttonTexture[6].loadFromFile("images/bucle.png") ||
        !buttonTexture[4].loadFromFile("images/foco.png") ||
        !buttonTexture[7].loadFromFile("images/current.png"))
    {
        return;
    }

    std::vector<Button> buttons(6);
    buttons[0] = {sf::Sprite(buttonTexture[6]), 6};
    buttons[1] = {sf::Sprite(buttonTexture[5]), 5};
    buttons[2] = {sf::Sprite(buttonTexture[4]), 4};
    buttons[3] = {sf::Sprite(buttonTexture[3]), 3};
    buttons[4] = {sf::Sprite(buttonTexture[2]), 2};
    buttons[5] = {sf::Sprite(buttonTexture[1]), 1};

    for (size_t i = 0; i < buttons.size(); ++i)
    {
        buttons[i].sprite.setPosition(WINDOW_WIDTH - buttons[i].sprite.getGlobalBounds().width * (i + 1) - 10 * i, 10);
    }

    std::vector<int> pressedButtons;
    int mainbot[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // importante
    int f1bot[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    int buclebot[4] = {0, 0, 0, 0};

    bool boolmain = false;
    bool boolf1 = false;
    bool boolbucle = false;
    bool clicDerechoPresionado = false;

    //-----------------------------------------------------------------------------------------------------------

    std::vector<sf::SoundBuffer> soundBuffers(3);
    std::vector<sf::Sound> sounds(3);

    // Cargar los archivos de audio
    if (!soundBuffers[0].loadFromFile("sonido_mapa.ogg") ||
        !soundBuffers[1].loadFromFile("sonido_mapa2.ogg") ||
        !soundBuffers[2].loadFromFile("sonido_mapa3.ogg"))
    {
        std::cerr << "Error al cargar los archivos de audio" << std::endl;
        return;
    }

    // Configurar los sonidos
    for (size_t i = 0; i < sounds.size(); ++i)
    {
        sounds[i].setVolume(25.f);
        sounds[i].setBuffer(soundBuffers[i]);
        sounds[i].setLoop(false); // No repetir en bucle
    }

    size_t currentSound = 0;
    bool soundPlaying = false;
    sf::Clock clock; // Para medir el tiempo

    int mapaActual = 0; // Índice del mapa actual

    // Crear la ventana
    window.setFramerateLimit(60);
    // Establecer el color gris oscuro
    sf::Color grisOscuro(169, 169, 169); // RGB para gris oscuro

    // Coordenadas de la vista isométrica
    Transform isoTransform;
    isoTransform.translate(300.f, 150.f);
    isoTransform.scale(1.f, 0.5f);
    isoTransform.rotate(45.f);

    // Cargar la textura del piso
    Texture texturaPiso;
    if (!texturaPiso.loadFromFile("images/loza_verde.png"))
    {
        return;
    }
    Texture texturaPiso2d;
    if (!texturaPiso2d.loadFromFile("images/loza_verde2d.png"))
    {
        return;
    }
    // Cargar la textura del bloque
    Texture texturaBloque;
    if (!texturaBloque.loadFromFile("images/loza_naranja.png"))
    {
        return;
    }
    Texture texturaBloque2d;
    if (!texturaBloque2d.loadFromFile("images/loza_naranja2d.png"))
    {
        return;
    }

    // Cargar la textura del punto final del mapa3D
    Texture texturaLozaAzul;
    if (!texturaLozaAzul.loadFromFile("images/loza_azul.png"))
    {
        return;
    }

    // Crear el sprite del piso

    Sprite tiles[gridSize][gridSize];

    crearSpritesPiso(tiles, mapLoaded::mapa2[mapaActual], texturaLozaAzul, texturaPiso);

    // Vector de sprites para los bloques
    vector<Sprite> bloques;
    // Vector para los bloques q se sobreponen al makibot
    vector<Sprite> bloques2;
    // Inicializar los sprites del piso y los bloques
    crearSpritesBloques(bloques, mapLoaded::mapa2[mapaActual], texturaBloque);

    // Figura del makibot en 2D
    CircleShape makibot2D;
    makibot2D.setRadius(5.f);
    makibot2D.setFillColor(Color::White);
    makibot2D.setOrigin(makibot2D.getRadius(), makibot2D.getRadius());
    makibot2D.setPosition(57.5f, 47.5f);

    // Cargar la textura del punto final del mapa2D
    Texture texturaLozaAzul2D;
    if (!texturaLozaAzul2D.loadFromFile("images/loza_azul2d.png"))
    {
        return;
    }

    // crear el minimapa en 2d
    Sprite tiles2d[gridSize][gridSize];
    configurarSprites(tiles2d, mapLoaded::mapa2[mapaActual], texturaBloque2d, texturaLozaAzul2D, texturaPiso2d);

    const int frameWidth = 40;
    const int frameHeight = 30;
    // SPRITES FRONTAL
    Texture spriteMaki;
    if (!spriteMaki.loadFromFile("images/sprite_carritos.png"))
    {
        return;
    }

    // Crear el sprite de la animación
    Sprite makibot;
    makibot.setTexture(spriteMaki);

    // Definir los rectángulos para cada fotograma de la animación

    vector<IntRect> framesF;
    framesF.push_back(IntRect(0, 0, frameWidth, frameHeight));
    framesF.push_back(IntRect(frameWidth, 0, frameWidth, frameHeight));
    framesF.push_back(IntRect(2 * frameWidth, 0, frameWidth, frameHeight));

    IntRect focoF = IntRect(3 * frameWidth, 0, frameWidth, frameHeight);

    vector<IntRect> framesB;
    framesB.push_back(IntRect(0, frameHeight, frameWidth, frameHeight));
    framesB.push_back(IntRect(frameWidth, frameHeight, frameWidth, frameHeight));
    framesB.push_back(IntRect(2 * frameWidth, frameHeight, frameWidth, frameHeight));

    IntRect focoB = IntRect(3 * frameWidth, frameHeight, frameWidth, frameHeight);

    // Ajustar la posición inicial del sprite para dibujarse desde la parte inferior
    makibot.setOrigin(20.0f, 30.f);
    makibot.setPosition(300.f, 160.f + yIso / 2.f);

    // primera aparicion del makibot
    makibot.setTextureRect(framesF[0]);

    // Variables para la animación
    int currentFrame = 0;
    Clock animationClock;
    float animationSpeed = 0.3f;

    // Variables para controlar el movimiento
    bool moving = false;
    Vector2f targetPosition = makibot.getPosition();

    bool miraNE = false;
    bool miraNO = false;
    bool miraSO = false;
    bool miraSE = true;

    // int CountMovimiento = 0;

    int contador = 0; // Inicialmente mirando hacia el Sur

    int contadorMovimientos = 0;
    int contadorMovf1 = 0;
    int contadorMovbucle = 0;

    int currentIteraciones = 0;

    bool girando = false;
    // Bucle principal
    bool booliniciar = false;

    int movimiento = 0;
    int movimientof1 = 0;
    int movimientoBucle = 0;

    int lastmov;
    int lastmovf1;
    int lastmovbucle;

    bool colisionando = false;

    MainPage mainPage;
    bool isGame = false;

    //pause's variables
    Texture texturaLozaRojo2D;
    if (!texturaLozaRojo2D.loadFromFile("images/loza_rojo2d.png"))
    {
        return;
    }
    Sprite tilesPaused2d[gridSize][gridSize];
    


    sf::Texture gameTexture;
    sf::Sprite gameSprite;
    sf::RectangleShape overlayBox(sf::Vector2f(300, 200));
    overlayBox.setFillColor(sf::Color(0, 0, 0, 150)); 
    overlayBox.setPosition(250, 200); 
    
    sf::Texture overlayImageTexture;
    overlayImageTexture.loadFromFile("images/jump.png"); 
    sf::Sprite overlayImage(overlayImageTexture);
    overlayImage.setPosition(overlayBox.getPosition().x + 50, overlayBox.getPosition().y + 50);

    bool isPaused = false;




    while (window.isOpen())
    {
        // Lógica de reproducción de sonido
        if (!soundPlaying)
        {
            sounds[currentSound].setVolume(5.f);
            sounds[currentSound].play();
            soundPlaying = true;
            clock.restart(); // Reiniciar el reloj
        }

        // Verificar si el sonido actual ha terminado
        if (clock.getElapsedTime() >= sounds[currentSound].getBuffer()->getDuration())
        {
            currentSound = (currentSound + 1) % sounds.size(); // Cambiar al siguiente sonido
            soundPlaying = false;                              // Permitir que el siguiente sonido se reproduzca
        }

        // gaaaaaaaaaaa
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            //-------------------------intrucciones-----------------------------------------
            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (iniciarButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                {
                    guardarInstrucciones(mainbot, 12, f1bot, 8, buclebot, 4, "instrucciones.txt", counter);
                    lenguajeintermedio = txtConvertstring("instrucciones.txt");
                    analizadorSyx(lenguajeintermedio);
                    cout << lenguajeintermedio << endl;
                    booliniciar = true;
                }
                if (incrementButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                {
                    if (counter < 5)
                    {
                        counter++;
                        numberSprite.setTexture(textures[counter]);
                    }
                }

                if (decrementButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                {
                    if (counter > 0)
                    {
                        counter--;
                        numberSprite.setTexture(textures[counter]);
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
                isPaused = !isPaused; 
                if (isPaused) {
                    // Captura la pantalla actual del juego
                    gameTexture.create(window.getSize().x, window.getSize().y);
                    gameTexture.update(window);
                    gameSprite.setTexture(gameTexture);
                    vector<int> optPaths = optPath();
                    cout<<optPaths[1];
                    cout<<"antes f"<<endl;
                    configurarSpritesPaused(optPaths, tilesPaused2d, mapLoaded::mapa2[mapaActual], texturaBloque2d, texturaLozaAzul2D, texturaPiso2d, texturaLozaRojo2D);
                }
            }
        }
        //----------------------------------isPAused---------------------------------------------
        while (isPaused) {
            while (window.pollEvent(event)){
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S) {
                    isPaused = !isPaused; 
                    
                }
            }
            window.clear();
            window.draw(gameSprite); // Dibuja la imagen congelada del juego
            window.draw(overlayBox); // Dibuja el cuadro
            for (int i = 0; i < gridSize; ++i)
        {
            for (int j = 0; j < gridSize; ++j)
            {
                window.draw(tilesPaused2d[i][j]);
            }
        }
            
            window.display();
        }


        //-------------------------intrucciones-----------------------------------------
        // Procesar interacción del ratón

        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        for (auto &button : buttons)
        {
            sf::FloatRect bounds = button.sprite.getGlobalBounds();
            if (bounds.contains(static_cast<sf::Vector2f>(mousePos)))
            {
                if (!button.isHovered)
                {
                    button.sprite.setScale(BUTTON_SCALE, BUTTON_SCALE);
                    button.isHovered = true;
                }

                else if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        clicDerechoPresionado = true;
                    }
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    if (event.mouseButton.button == sf::Mouse::Left && clicDerechoPresionado == true)
                    {
                        if (boolmain)
                        {
                            InsertarInstru(mainbot, 12, button.id);
                            imprimirArray(mainbot, 12);
                        }
                        if (boolf1)
                        {
                            InsertarInstru(f1bot, 8, button.id);
                            imprimirArray(f1bot, 8);
                        }
                        if (boolbucle)
                        {
                            InsertarInstru(buclebot, 4, button.id);
                            imprimirArray(buclebot, 4);
                        }
                        clicDerechoPresionado = false;
                    }
                }
            }
            else
            {
                if (button.isHovered)
                {
                    button.sprite.setScale(1.f, 1.f);
                    button.isHovered = false;
                }
            }
        }
        //-------------------------intrucciones-----------------------------------------

        if (booliniciar)
        {
            if (contadorMovimientos < sizeof(mainbot) / sizeof(mainbot[0]) && moving == false && girando == false && colisionando == false)
            {
                if (mainbot[contadorMovimientos] != 7)
                {
                    movimiento = mainbot[contadorMovimientos];
                }
                if (contadorMovimientos != 0)
                {
                    if (mainbot[contadorMovimientos - 1] == 7)
                    {
                        cout << "cambio " << endl;
                        mainbot[contadorMovimientos - 1] = lastmov;
                    }
                }
                // int movimiento = mainbot[contadorMovimientos];

                if (movimiento == 2 || movimiento == 3)
                {
                    lastmov = movimiento;
                    mainbot[contadorMovimientos] = 7;
                    // Cambia la dirección cíclicamente
                    updateDirection(contador, movimiento);
                    cout << contador << "----------------------" << endl;
                    miraNE = estados[contador].miraNE;
                    miraNO = estados[contador].miraNO;
                    miraSO = estados[contador].miraSO;
                    miraSE = estados[contador].miraSE;
                    girando = true;
                    contadorMovimientos++;
                }
                else if (movimiento == 1)
                {
                    lastmov = movimiento;
                    mainbot[contadorMovimientos] = 7;
                    // Mover en la dirección actual
                    Estado estado;
                    estado.miraNE = miraNE;
                    estado.miraNO = miraNO;
                    estado.miraSO = miraSO;
                    estado.miraSE = miraSE;

                    move2(targetPosition, moving, estado, xIso, yIso);
                    contadorMovimientos++;
                }
                else if (movimiento == 5)
                {
                    lastmov = movimiento;
                    mainbot[contadorMovimientos] = 7;

                    cout << "gaaaa" << endl;
                    if (contadorMovf1 < sizeof(f1bot) / sizeof(f1bot[0]) && moving == false && girando == false)
                    {

                        if (f1bot[contadorMovf1] != 7)
                        {
                            movimientof1 = f1bot[contadorMovf1];
                        }
                        if (contadorMovf1 != 0)
                        {
                            if (f1bot[contadorMovf1 - 1] == 7)
                            {
                                f1bot[contadorMovf1 - 1] = lastmovf1;
                            }
                        }

                        if (movimientof1 == 2 || movimientof1 == 3)
                        {
                            lastmovf1 = movimientof1;
                            f1bot[contadorMovf1] = 7;
                            // Cambia la dirección cíclicamente
                            updateDirection(contador, movimientof1);
                            cout << contador << "----------------------" << endl;
                            miraNE = estados[contador].miraNE;
                            miraNO = estados[contador].miraNO;
                            miraSO = estados[contador].miraSO;
                            miraSE = estados[contador].miraSE;
                            girando = true;
                            contadorMovf1++;
                        }
                        else if (movimientof1 == 1)
                        {
                            lastmovf1 = movimientof1;
                            f1bot[contadorMovf1] = 7;
                            // Mover en la dirección actual
                            Estado estado;
                            estado.miraNE = miraNE;
                            estado.miraNO = miraNO;
                            estado.miraSO = miraSO;
                            estado.miraSE = miraSE;

                            move2(targetPosition, moving, estado, xIso, yIso);
                            contadorMovf1++;
                        }
                        else if (movimientof1 == 0)
                        {
                            contadorMovf1 = 0;
                            contadorMovimientos++;
                        }

                        // Avanzar en el array de movimientos
                    }
                    else if (contadorMovf1 >= 8)
                    {
                        f1bot[contadorMovf1 - 1] = lastmovf1;
                        contadorMovf1 = 0;
                        contadorMovimientos++;
                    }
                }
                else if (movimiento == 6 && currentIteraciones < counter)
                {
                    lastmov = movimiento;
                    mainbot[contadorMovimientos] = 7;

                    cout << "bucleoco" << endl;
                    if (contadorMovbucle < sizeof(buclebot) / sizeof(buclebot[0]) && moving == false && girando == false)
                    {
                        if (buclebot[contadorMovbucle] != 7)
                        {
                            movimientoBucle = buclebot[contadorMovbucle];
                        }
                        if (contadorMovbucle != 0)
                        {
                            if (buclebot[contadorMovbucle - 1] == 7)
                            {
                                buclebot[contadorMovbucle - 1] = lastmovbucle;
                            }
                        }

                        if (movimientoBucle == 2 || movimientoBucle == 3)
                        {
                            lastmovbucle = movimientoBucle;
                            buclebot[contadorMovbucle] = 7;
                            // Cambia la dirección cíclicamente
                            updateDirection(contador, movimientoBucle);
                            cout << contador << "----------------------" << endl;
                            miraNE = estados[contador].miraNE;
                            miraNO = estados[contador].miraNO;
                            miraSO = estados[contador].miraSO;
                            miraSE = estados[contador].miraSE;
                            girando = true;
                            contadorMovbucle++;
                        }
                        else if (movimientoBucle == 1)
                        {
                            lastmovbucle = movimientoBucle;
                            buclebot[contadorMovbucle] = 7;
                            // Mover en la dirección actual
                            Estado estado;
                            estado.miraNE = miraNE;
                            estado.miraNO = miraNO;
                            estado.miraSO = miraSO;
                            estado.miraSE = miraSE;

                            move2(targetPosition, moving, estado, xIso, yIso);
                            contadorMovbucle++;
                        }
                        else if (movimientoBucle == 0)
                        {
                            contadorMovbucle = 0;
                            currentIteraciones++;
                        }

                        // Avanzar en el array de movimientos
                    }
                    else if (contadorMovbucle >= 4)
                    {
                        buclebot[contadorMovbucle - 1] = lastmovbucle;
                        contadorMovbucle = 0;
                        currentIteraciones++;
                    }
                }

                else if (currentIteraciones = counter)
                {
                    contadorMovimientos++;
                    currentIteraciones = 0;
                }
                else if (movimiento == 0)
                {
                    cout << "Llegue al final del array de movimientos" << endl;
                    contadorMovimientos++;
                    contadorMovimientos = 0;
                    booliniciar = false;
                }

                // Avanzar en el array de movimientos
                // contadorMovimientos++;
            }
            else if (!moving && !girando && !colisionando)
            {
                cout << "Llegue al final del array de movimientos" << endl;
                mainbot[contadorMovimientos - 1] = lastmov;
                contadorMovimientos = 0;
                booliniciar = false;
            }
        }

        // Actualizar la animación del sprite
        if (moving || colisionando)
        {
            // Verificar colisión con bloques
            Vector2f newPosition = targetPosition;
            int posXIso, posYISo;
            Vector2i indices = calculateGridIndices(newPosition, pos_origin);

            posXIso = indices.x;
            posYISo = indices.y;

            cout << posXIso << posYISo << endl;

            if (mapLoaded::mapa2[mapaActual][posXIso][posYISo] == -1 && posXIso != -1 && posYISo != -1)
            {
                bloques.clear();

                for (int i = 0; i < gridSize; ++i)
                {
                    for (int j = 0; j < gridSize; ++j)
                    {
                        tiles[i][j] = Sprite(); // Reasignar un sprite por defecto
                    }
                }

                for (int i = 0; i < gridSize; ++i)
                {
                    for (int j = 0; j < gridSize; ++j)
                    {
                        tiles2d[i][j] = Sprite(); // Reasignar un sprite por defecto
                    }
                }

                // Incrementar mapaActual y usar el operador módulo para reiniciar a 0 cuando se alcance el límite
                mapaActual = (mapaActual + 1) % 1;

                posXIso = 0;
                posYISo = 0;

                for (int i = 0; i < 12; ++i)
                {
                    mainbot[i] = 0;
                }
                for (int i = 0; i < 8; ++i)
                {
                    f1bot[i] = 0;
                }
                for (int i = 0; i < 4; ++i)
                {
                    buclebot[i] = 0;
                }
                contador = 0;
                cout << "hhhhh" << endl;
                makibot.setOrigin(20.f, 30.f);
                makibot.setPosition(300.f, 160.f + yIso / 2.f);

                // Actualizar targetPosition a la nueva posición inicial
                targetPosition = makibot.getPosition();

                crearSpritesPiso(tiles, mapLoaded::mapa2[mapaActual], texturaLozaAzul, texturaPiso);
                crearSpritesBloques(bloques, mapLoaded::mapa2[mapaActual], texturaBloque);
                configurarSprites(tiles2d, matrices2d[mapaActual], texturaBloque2d, texturaLozaAzul2D, texturaPiso2d);

                // Detener movimiento
                moving = false;
            }   

            if (!colisionando)
            {
                if (!(posXIso >= 0 && posXIso < gridSize && posYISo >= 0 && posYISo < gridSize) ||
                    (mapLoaded::mapa2[mapaActual][posXIso][posYISo] != 0 && mapLoaded::mapa2[mapaActual][posXIso][posYISo] != -1))
                {
                    colisionando = true;
                    moving = false;
                    targetPosition = makibot.getPosition();
                }
                else
                {
                    if (animationClock.getElapsedTime().asSeconds() > animationSpeed)
                    {
                        if (miraNE || miraNO)
                        {
                            currentFrame = (currentFrame + 1) % framesB.size();
                            makibot.setTextureRect(framesB[currentFrame]);
                            cout << "arriba" << endl;
                        }
                        else if (miraSO || miraSE)
                        {
                            currentFrame = (currentFrame + 1) % framesF.size();
                            makibot.setTextureRect(framesF[currentFrame]);
                        }

                        animationClock.restart();
                    }

                    // movimeinto del car
                    Vector2f currentPosition = makibot.getPosition();
                    moveRobot(makibot, targetPosition, currentPosition, xIso, yIso);

                    // Detener el movimiento al alcanzar el objetivo

                    stopMovement(makibot, targetPosition, makibot.getPosition(), currentFrame, miraNE, miraNO, miraSO, miraSE, moving, framesB, framesF);
                    // posicion de makibot2D
                    makibot2D.setPosition(57.5f + 15.f * posXIso, 47.4f + 15.f * posYISo);

                    // mapa 2D , se verifica la posicion de los bloques y el makibot
                    updateBlocks(bloques2, mapLoaded::mapa2[mapaActual], gridSize, texturaBloque, lado, posXIso, posYISo);
                }
            }
            else if (colisionando == true)
            {
                if (animationClock.getElapsedTime().asSeconds() > 1.5)
                {
                    colisionando = false;
                    cout << "cambiando colisionando a false" << endl;
                    animationClock.restart();
                }
            }

            if (miraNE)
                makibot.setScale(1.f, 1.f);
            else if (miraNO)
                makibot.setScale(-1.f, 1.f);
            else if (miraSO)
                makibot.setScale(-1.f, 1.f);
            else if (miraSE)
                makibot.setScale(1.f, 1.f);
        }
        else if (girando)
        {
            if (animationClock.getElapsedTime().asSeconds() > 1.5)
            {
                if (miraNE || miraNO)
                {
                    makibot.setTextureRect(framesB[currentFrame]);
                }
                else if (miraSO || miraSE)
                {
                    makibot.setTextureRect(framesF[currentFrame]);
                }
                if (miraNE)
                    makibot.setScale(1.f, 1.f);
                else if (miraNO)
                    makibot.setScale(-1.f, 1.f);
                else if (miraSO)
                    makibot.setScale(-1.f, 1.f);
                else if (miraSE)
                    makibot.setScale(1.f, 1.f);
                girando = false;

                animationClock.restart();
            }
        }

        window.clear(grisOscuro);
        // dibujar piso 2d
        for (int i = 0; i < gridSize; ++i)
        {
            for (int j = 0; j < gridSize; ++j)
            {
                window.draw(tiles2d[i][j]);
            }
        }
        // Dibujar el piso y los bloques
        for (int i = 0; i < gridSize; ++i)
        {
            for (int j = 0; j < gridSize; ++j)
            {
                window.draw(tiles[i][j], isoTransform);
            }
        }
        for (auto &bloque : bloques)
        {
            window.draw(bloque, isoTransform);
        }

        window.draw(makibot);
        window.draw(makibot2D);

        for (auto &bloque : bloques2)
        {
            window.draw(bloque, isoTransform);
        }
        scaleOnHover(iniciarButton, window);

        scaleOnHover(incrementButton, window);
        scaleOnHover(decrementButton, window);

        window.draw(iniciarButton);
        window.draw(numberSprite);
        window.draw(incrementButton);
        window.draw(decrementButton);
        for (const auto &button : buttons)
        {
            window.draw(button.sprite);
        }
        dibujarImagenes(window, buttonTexture, mainbot, sizeof(mainbot) / sizeof(mainbot[0]), 720, 100, event, boolmain);
        dibujarImagenes(window, buttonTexture, f1bot, sizeof(f1bot) / sizeof(f1bot[0]), 720, 320, event, boolf1);
        dibujarImagenes(window, buttonTexture, buclebot, sizeof(buclebot) / sizeof(buclebot[0]), 720, 470, event, boolbucle);

        window.display();
    }

    return;
}

void textInputInterface(sf::RenderWindow &window)
{
    sf::Font font;
    if (!font.loadFromFile("digital.ttf"))
    {
        std::cerr << "Error al cargar la fuente" << std::endl;
        return;
    }

    sf::Text instructionText("Ingrese la direccion del archivo:", font, 20);
    instructionText.setPosition(50, 50);

    sf::Text inputText("", font, 20);
    inputText.setPosition(50, 100);
    inputText.setFillColor(sf::Color::White);

    std::string inputString;
    bool fileLoaded = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode == '\b')
                {
                    // Backspace
                    if (!inputString.empty())
                    {
                        inputString.pop_back();
                    }
                }
                else if (event.text.unicode == '\r')
                {
                    // Enter key, intentar cargar el archivo
                    fileLoaded = mapLoaded::loadFile(inputString);
                    if (fileLoaded)
                    {
                        std::cout << "Matriz cargada correctamente desde " << inputString << std::endl;
                        return;
                    }
                    else
                    {
                        std::cerr << "Error al cargar la matriz desde " << inputString << std::endl;
                    }
                }
                else if (event.text.unicode < 128)
                {
                    inputString += static_cast<char>(event.text.unicode);
                }
                inputText.setString(inputString);
            }
        }

        window.clear(sf::Color::Black);
        window.draw(instructionText);
        window.draw(inputText);
        window.display();
    }
}

//--------------------------------------MAIN---------------------------------------------------------------------
int main()
{
    sf::RenderWindow window(VideoMode(1000, 600), "Makibot");
    MainPage mainpage;
    int mode = 0;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            mainpage.handleEvent(event, mode, window);
        }
        mainpage.allSetPosition(window);

        if (mode == 1)
        {
            gameMode(window);
        }
        else if (mode == 2)
        {
            textInputInterface(window);

            customMode(window);
        }
        mainpage.draw(window);
        window.display();
    }
    return 0;
}