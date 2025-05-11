#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include "mapas.h"
//------------------------------------------------------------------------------------------------------------
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include <array>

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

template <size_t N>
void imprimirArray(std::array<int, N> &array)
{
    std::cout << "Contenido del array: ";
    for (int i = 0; i < N; ++i)
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
template <size_t N>
void FIFOdelete(std::array<int, N> &array)
{
    for (int i = N - 1; i >= 0; --i)
    {
        if (array[i] != 0)
        {
            array[i] = 0;
            break;
        }
    }
}

// Función para dibujar las imágenes en base al array
template <size_t N>
void renderImagesBlocksWithControls(sf::RenderWindow &window, const std::vector<sf::Texture> &texturas, std::array<int, N> &array, int xx, int yy, sf::Event event, bool &estado, sf::Sound &clickSound)
{

    // Tamaño de cada imagen (asumimos que todas tienen el mismo tamaño)
    sf::Vector2u imageSize = texturas[0].getSize();
    int imageWidth = imageSize.x;
    int imageHeight = imageSize.y;

    int offsetX = xx; // Desplazamiento en x
    int offsetY = yy; // Desplazamiento en y

    for (int i = 0; i < N; ++i)
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

    sf::RectangleShape botonOFF(sf::Vector2f(esc, esc));
    sf::RectangleShape botonON(sf::Vector2f(esc2, esc2));
    botonOFF.setPosition(offsetX - 40, offsetY);
    botonON.setPosition(offsetX - 40, (offsetY) + 40);
    botonOFF.setFillColor(!estado ? sf::Color::Red : sf::Color::Red);
    botonON.setFillColor(!estado ? sf::Color::Green : sf::Color::Yellow);

    static bool buttonPressed = false;
    static bool button2Pressed = false;

    if (event.type == sf::Event::MouseButtonPressed)
    {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

        if (botonOFF.getGlobalBounds().contains(mousePosF) && !buttonPressed)
        {
            buttonPressed = true;
            FIFOdelete(array);
            botonOFF.setFillColor(sf::Color::White);
            esc = 28;
            clickSound.play();
            clickSound.setVolume(10.f);
        }

        if (botonON.getGlobalBounds().contains(mousePosF) && !button2Pressed)
        {
            button2Pressed = true;
            estado = !estado; // Cambia el estado
            esc2 = 28;
            clickSound.play();
            clickSound.setVolume(10.f);
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

    window.draw(botonOFF);
    window.draw(botonON);
}

struct Button
{
    sf::Sprite sprite;
    int id;
    bool isHovered = false;
};

template <size_t N>
void InsertarInstru(std::array<int, N> &array, int valor)
{
    for (size_t i = 0; i < N; ++i)
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
            if (matriz2D[i][j] == 1)
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
            cout << "arriba" << endl;
        }
        else if (miraSE || miraSO)
        {
            makibot.setTextureRect(framesF[currentFrame]);
            cout << "abajo" << endl;
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
struct Semaforo
{
    vector<IntRect> seccionSemaforo;
    float posX;
    float posY;
};
void crearSemaforos(vector<Semaforo> &semaforos, const int matriz3D[][gridSize], vector<IntRect> &seccionSemaforo)
{
    semaforos.clear();
    for (int i = 0; i < gridSize; i++)
    {
        for (int j = 0; j < gridSize; j++)
        {
            if (matriz3D[i][j] == -2)
            {
                cout << "cont" << endl;
                Semaforo sem;
                sem.posX = i * lado;
                sem.posY = j * lado;
                sem.seccionSemaforo = seccionSemaforo;

                semaforos.push_back(sem);
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

struct ParametrosNivel
{
    int state;       // 0: Rojo, 1: Naranja, 2: Verde
    int numeroCiclo; // cantidad de ciclos 1-5

    array<int, 12> mainbot;
    array<int, 8> f1bot;
    array<int, 4> buclebot;
    array<int, 8> ifbot;
    array<int, 8> elsebot;

    bool boolmain;
    bool boolf1;
    bool boolbucle;
    bool boolcondicional;

    int mapaActual;
    int currentFrame;
    bool moving;

    bool miraNE;
    bool miraNO;
    bool miraSO;
    bool miraSE;

    // makibot.setTextureRect(framesF[0]);

    int contador;
    //--------------------------------------------
    int contadorMovimientos;
    int contadorMovf1;
    int contadorMovbucle;
    int contadorMovIf;
    int contadorMovElse;

    int currentIteraciones = 0;

    bool girando;
    // Bucle principal
    bool booliniciar;

    int movimiento = 0;
    int movimientof1 = 0;
    int movimientoBucle = 0;
    int movimientoIf = 0;
    int movimientoElse = 0;

    int lastmov;
    int lastmovf1;
    int lastmovBucle;
    int lastmovIf;
    int lastmovElse;

    bool colisionando = false;

    bool ifCondition = true; // boton ifelse

    bool isBlockSemaforo = true;
    int laststate = -1;

    void reset1()
    {
        state = 0;
        numeroCiclo = 0;
        std::array<int, 12> mainbot;
        std::array<int, 8> f1bot;
        std::array<int, 4> buclebot;
        std::array<int, 8> ifbot;
        std::array<int, 8> elsebot;
        boolmain = false;
        boolf1 = false;
        boolbucle = false;
        boolcondicional = false;
        // mapaActual = 0;
        currentFrame = 0;
        moving = false;
        miraNE = false;
        miraNO = false;
        miraSO = false;
        miraSE = true;
        contador = 0;
        contadorMovimientos = 0;
        contadorMovf1 = 0;
        contadorMovbucle = 0;
        contadorMovIf = 0;
        contadorMovElse = 0;
        currentIteraciones = 0;
        girando = false;
        booliniciar = false;
        movimiento = 0;
        movimientof1 = 0;
        movimientoBucle = 0;
        movimientoIf = 0;
        movimientoElse = 0;
        lastmov = 0;
        lastmovf1 = 0;
        lastmovBucle = 0;
        lastmovIf = 0;
        lastmovElse = 0;
        colisionando = false;
        ifCondition = true;
        isBlockSemaforo = true;
        laststate = -1;
    }
    void resetall()
    {
        state = 0;
        numeroCiclo = 0;
        mainbot.fill(0);
        f1bot.fill(0);
        buclebot.fill(0);
        ifbot.fill(0);
        elsebot.fill(0);
        boolmain = false;
        boolf1 = false;
        boolbucle = false;
        boolcondicional = false;
        mapaActual = 0;
        currentFrame = 0;
        moving = false;
        miraNE = false;
        miraNO = false;
        miraSO = false;
        miraSE = true;
        contador = 0;
        contadorMovimientos = 0;
        contadorMovf1 = 0;
        contadorMovbucle = 0;
        contadorMovIf = 0;
        contadorMovElse = 0;
        currentIteraciones = 0;
        girando = false;
        booliniciar = false;
        movimiento = 0;
        movimientof1 = 0;
        movimientoBucle = 0;
        movimientoIf = 0;
        movimientoElse = 0;
        lastmov = 0;
        lastmovf1 = 0;
        lastmovBucle = 0;
        lastmovIf = 0;
        lastmovElse = 0;
        colisionando = false;
        ifCondition = true;
        isBlockSemaforo = true;
        laststate = -1;
    }
};

ParametrosNivel pNivel;
template <size_t N>
void reemplazar(array<int, N> arr)
{
    for (int i = 0; i < N; ++i)
    {
        if (arr[i] == 4 || arr[i] == 5 || arr[i] == 6)
        {
            arr[i] = 0;
        }
    }
}
int main()
{

    //--------------------------------------- Configuración del Semaforo --------------------------------------
    sf::Texture redTexture, orangeTexture, greenTexture;

    if (!redTexture.loadFromFile("images/srojo.png") ||
        !orangeTexture.loadFromFile("images/snaranja.png") ||
        !greenTexture.loadFromFile("images/sverde.png"))
    {
        std::cerr << "Error al cargar las imágenes del semáforo" << std::endl;
        return -1;
    }

    sf::Sprite SpriteSemaforo(redTexture);

    SpriteSemaforo.setPosition(30, 350);

    // Temporizador
    sf::Clock clockSemaforo;
    int state = 0;           // 0: Rojo, 1: Naranja, 2: Verde
    float redTime = 5.0f;    // 5 segundos en rojo
    float orangeTime = 2.0f; // 2 segundos en naranja
    float greenTime = 5.0f;  // 5 segundos en verde
    //--------------------------------------- Fin de configuración del Semaforo --------------------------------------

    //--------------------------------------- Configuración de sonidos --------------------------------------
    sf::SoundBuffer clickBuffer;
    sf::Sound clickSound;

    sf::SoundBuffer clickBuffer2;
    sf::Sound clickSound2;

    // Cargar y asignar el primer sonido
    if (!clickBuffer.loadFromFile("sounds/click1.ogg"))
    {
        std::cerr << "Error al cargar el sonido click1.ogg" << std::endl;
        return -1; // Maneja el error adecuadamente
    }
    clickSound.setBuffer(clickBuffer);

    // Cargar y asignar el segundo sonido
    if (!clickBuffer2.loadFromFile("sounds/click2.ogg"))
    {
        std::cerr << "Error al cargar el sonido click2.ogg" << std::endl;
        return -1; // Maneja el error adecuadamente
    }
    clickSound2.setBuffer(clickBuffer2);
    //--------------------------------------- Fin de configuración de sonidos --------------------------------------

    //-----------------------------------------------------------------------------------------------------------
    // contador
    sf::Texture textures[6];
    for (int i = 0; i < 6; ++i)
    {
        if (!textures[i].loadFromFile("images/" + std::to_string(i) + ".png"))
        {
            std::cerr << "Error cargando la imagen " << i << ".png" << std::endl;
            return -1;
        }
    }

    sf::Texture iniciarTexture;
    sf::Texture incrementTexture;
    sf::Texture decrementTexture;
    if (!iniciarTexture.loadFromFile("images/go.png") || !incrementTexture.loadFromFile("images/imageincrement.png") || !decrementTexture.loadFromFile("images/imagendecrement.png"))
    {
        std::cerr << "Error cargando la imagen imageincrement.png o decrement" << std::endl;
        return -1;
    }
 // Add pause/play button textures and sprite--------------------------------------------------------------------------
    sf::Texture pausaTextura;
    sf::Texture continuaTextura;
    sf::Texture nivel1Textura;
    sf::Texture menuTextura;
    if (!pausaTextura.loadFromFile("images/pausa.png") ||
        !continuaTextura.loadFromFile("images/continua.png") ||
        !nivel1Textura.loadFromFile("images/nivel1.png") ||
        !menuTextura.loadFromFile("images/menu.png"))
    {
        std::cerr << "Error cargando las imágenes de pausa" << std::endl;
        return -1;
    }

    sf::Sprite pausaButton(pausaTextura);
    pausaButton.setPosition(30, 650); // Position in top-left corner
    bool isPaused = false;
    //------------------------------------------------------------------------------------------------------------
    // imagenes de restart
    sf::Texture restart1Textura;
    sf::Texture restartaTextura;
    if (!restart1Textura.loadFromFile("images/restart_1.png") || !restartaTextura.loadFromFile("images/restart_a.png"))
    {
        std::cerr << "Error cargando la imagen restart.png" << std::endl;
        return -1;
    }
    sf::Sprite restart1Button(restart1Textura);
    restart1Button.setPosition(110, 650);

    sf::Sprite restartaButton(restartaTextura);
    restartaButton.setPosition(200, 650);

    //------------------------------------------------------------------------------------------------------------
    sf::Texture ifTexture;
    sf::Texture elseTexture;
    if (!ifTexture.loadFromFile("images/if.png") || !elseTexture.loadFromFile("images/else.png"))
    {
        std::cerr << "Error cargando la imagen if.png o else.png" << std::endl;
        return -1;
    }
    // imagen if and else

    sf::Sprite IfandElseSprite(ifTexture);
    IfandElseSprite.setPosition(620, 590);

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
        return -1;
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
    pNivel.mainbot.fill(0);
    pNivel.f1bot.fill(0);
    pNivel.buclebot.fill(0);
    pNivel.ifbot.fill(0);
    pNivel.elsebot.fill(0);

    pNivel.boolmain = false;
    pNivel.boolf1 = false;
    pNivel.boolbucle = false;
    pNivel.boolcondicional = false;
    bool clicDerechoPresionado = false;

    //-----------------------------------------------------------------------------------------------------------

    std::vector<sf::SoundBuffer> soundBuffers(3);
    std::vector<sf::Sound> sounds(3);

    // Cargar los archivos de audio
    if (!soundBuffers[0].loadFromFile("sounds/sonido_mapa.ogg") ||
        !soundBuffers[1].loadFromFile("sounds/sonido_mapa2.ogg") ||
        !soundBuffers[2].loadFromFile("sounds/sonido_mapa3.ogg"))
    {
        std::cerr << "Error al cargar los archivos de audio" << std::endl;
        return -1;
    }

    // Configurar los sonidos
    for (size_t i = 0; i < sounds.size(); ++i)
    {
        sounds[i].setVolume(3.f);
        sounds[i].setBuffer(soundBuffers[i]);
        sounds[i].setLoop(false); // No repetir en bucle
    }

    size_t currentSound = 0;
    bool soundPlaying = false;
    sf::Clock clock; // Para medir el tiempo

    pNivel.mapaActual = 0; // Índice del mapa actual

    // Crear la ventana
    RenderWindow window(VideoMode(1000, 750), "Makibot");
    window.setFramerateLimit(60);
    // Establecer el color gris oscuro
    sf::Color grisOscuro(169, 169, 169); // RGB para pNivel.gris oscuro

    // Coordenadas de la vista isométrica
    Transform isoTransform;
    isoTransform.translate(300.f, 150.f);
    isoTransform.scale(1.f, 0.5f);
    isoTransform.rotate(45.f);

    // Cargar la textura del piso
    Texture texturaPiso;
    if (!texturaPiso.loadFromFile("images/loza_verde.png"))
    {
        return -1;
    }
    Texture texturaPiso2d;
    if (!texturaPiso2d.loadFromFile("images/loza_verde2d.png"))
    {
        return -1;
    }
    // Cargar la textura del bloque
    Texture texturaBloque;
    if (!texturaBloque.loadFromFile("images/loza_naranja.png"))
    {
        return -1;
    }
    Texture texturaBloque2d;
    if (!texturaBloque2d.loadFromFile("images/loza_naranja2d.png"))
    {
        return -1;
    }

    // Cargar la textura del punto final del mapa3D
    Texture texturaLozaAzul;
    if (!texturaLozaAzul.loadFromFile("images/loza_azul.png"))
    {
        return -1;
    }

    // Crear el sprite del piso

    Sprite tiles[gridSize][gridSize];

    crearSpritesPiso(tiles, mapas[pNivel.mapaActual], texturaLozaAzul, texturaPiso);

    // Vector de sprites para los bloques
    vector<Sprite> bloques;
    // Vector para los bloques q se sobreponen al makibot
    vector<Sprite> bloques2;
    // Inicializar los sprites del piso y los bloques
    crearSpritesBloques(bloques, mapas[pNivel.mapaActual], texturaBloque);

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
        return -1;
    }

    // crear el minimapa en 2d
    Sprite tiles2d[gridSize][gridSize];
    configurarSprites(tiles2d, matrices2d[pNivel.mapaActual], texturaBloque2d, texturaLozaAzul2D, texturaPiso2d);

    const int frameWidth = 40;
    const int frameHeight = 30;
    // SPRITES FRONTAL
    Texture spriteMaki;
    if (!spriteMaki.loadFromFile("images/sprite_carritos.png"))
    {
        return -1;
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
    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    Texture spriteSemaforo;
    if (!spriteSemaforo.loadFromFile("images/semaforo.png"))
    {
        return -1;
    }
    vector<IntRect> framesSemaforo;
    framesSemaforo.push_back(IntRect(0, 0, 54, 54));
    framesSemaforo.push_back(IntRect(54, 0, 54 * 2, 54));
    framesSemaforo.push_back(IntRect(2 * 54, 0, 54 * 3, 54));
    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    vector<Semaforo> texSemaforo;
    crearSemaforos(texSemaforo, matrices2d[pNivel.mapaActual], framesSemaforo);
    cout << texSemaforo.size() << endl;

    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------

    // Ajustar la posición inicial del sprite para dibujarse desde la parte inferior
    makibot.setOrigin(20.0f, 30.f);
    makibot.setPosition(300.f, 160.f + yIso / 2.f);

    // primera aparicion del makibot
    makibot.setTextureRect(framesF[0]);

    // Variables para la animación
    pNivel.currentFrame = 0;
    Clock animationClock;
    float animationSpeed = 0.3f;

    // Variables para controlar el movimiento
    bool moving = false;
    Vector2f targetPosition = makibot.getPosition();

    bool miraNE = false;
    bool miraNO = false;
    bool miraSO = false;
    bool miraSE = true;

    int contdebug = 0;
    // int CountMovimiento = 0;

    int contador = 0; // Inicialmente mirando hacia el Sur

    pNivel.contadorMovimientos = 0;
    pNivel.contadorMovf1 = 0;
    pNivel.contadorMovbucle = 0;
    pNivel.contadorMovIf = 0;
    pNivel.contadorMovElse = 0;

    pNivel.currentIteraciones = 0;

    pNivel.girando = false;
    // Bucle principal
    pNivel.booliniciar = false;

    pNivel.movimiento = 0;
    pNivel.movimientof1 = 0;
    pNivel.movimientoBucle = 0;
    pNivel.movimientoIf = 0;
    pNivel.movimientoElse = 0;

    pNivel.lastmov;
    pNivel.lastmovf1;
    pNivel.lastmovBucle;
    pNivel.lastmovIf;
    pNivel.lastmovElse;

    pNivel.colisionando = false;

    pNivel.ifCondition = true;

    pNivel.isBlockSemaforo = true;
    pNivel.laststate = -1;

    // menu--------------------------------------------------------------------------------------
    sf::Sprite menuSprite(menuTextura);
    menuSprite.setPosition(400, 200); // Position in top-left corner
    sf::Sprite continuaSprite(continuaTextura);
    continuaSprite.setPosition(475, 250); // Position in top-left corner
    sf::Sprite nivel1Sprite(nivel1Textura);
    nivel1Sprite.setPosition(465, 350); // Position in top-left corner
    //-------------------------------------------------------------------------
    while (window.isOpen())
    {
        // Lógica de reproducción de sonido
        if (!soundPlaying)
        {
            sounds[currentSound].setVolume(2.f);
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

                // Add menu/pausa button check
                if (pausaButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) && !isPaused)
                {
                    isPaused = !isPaused;
                    clickSound.play();
                }
                else if (continuaSprite.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y) && isPaused)
                {
                    isPaused = !isPaused;
                    clickSound.play();
                }
                if (!isPaused)
                {
                    if (restart1Button.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                    {
                        pNivel.reset1();
                        contador = 0;
                        makibot.setOrigin(20.f, 60.f);
                        makibot.setPosition(300.f, 160.f + yIso / 2.f);
                        makibot.setTextureRect(framesF[0]);
                        targetPosition = makibot.getPosition();
                        clickSound.play();
                    }
                    if (restartaButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                    {
                        pNivel.resetall();
                        contador = 0;
                        makibot.setOrigin(20.f, 60.f);
                        makibot.setPosition(300.f, 160.f + yIso / 2.f);
                        makibot.setTextureRect(framesF[0]);
                        targetPosition = makibot.getPosition();

                        crearSpritesPiso(tiles, mapas[pNivel.mapaActual], texturaLozaAzul, texturaPiso);
                        bloques.clear();
                        crearSpritesBloques(bloques, mapas[pNivel.mapaActual], texturaBloque);
                        configurarSprites(tiles2d, matrices2d[pNivel.mapaActual], texturaBloque2d, texturaLozaAzul2D, texturaPiso2d);

                        updateBlocks(bloques2, mapas[pNivel.mapaActual], gridSize, texturaBloque, lado, 0, 0);
                        clickSound.play();
                    }
                    if (iniciarButton.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                    {
                        clickSound2.play();

                        cout << lenguajeintermedio << endl;
                        pNivel.booliniciar = true;
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
                    if (IfandElseSprite.getGlobalBounds().contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
                    {
                        pNivel.ifCondition = !pNivel.ifCondition;
                        if (pNivel.ifCondition)
                            IfandElseSprite.setTexture(ifTexture);
                        else
                            IfandElseSprite.setTexture(elseTexture);
                    }
                }
            }
        }
        //--------------------------------Semaforo-----------------------------------------
        // Actualizar temporizador
        float timeSemaforo = clockSemaforo.getElapsedTime().asSeconds();

        // Cambiar de estado según el tiempo
        if (state == 0 && timeSemaforo >= redTime)
        {
            state = 1;
            SpriteSemaforo.setTexture(orangeTexture);
            clockSemaforo.restart();
        }
        else if (state == 1 && timeSemaforo >= orangeTime)
        {
            state = 2;
            SpriteSemaforo.setTexture(greenTexture);
            clockSemaforo.restart();
        }
        else if (state == 2 && timeSemaforo >= greenTime)
        {
            SpriteSemaforo.setTexture(redTexture);
            state = 0;
            clockSemaforo.restart();
        }
        //--------------------------------Fin_Semaforo-----------------------------------------

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
                    reemplazar(pNivel.f1bot);
                    reemplazar(pNivel.buclebot);
                    reemplazar(pNivel.ifbot);
                    reemplazar(pNivel.elsebot);
                    if (event.mouseButton.button == sf::Mouse::Left && clicDerechoPresionado == true)
                    {

                        if (pNivel.boolmain)
                        {
                            InsertarInstru(pNivel.mainbot, button.id);
                            imprimirArray(pNivel.mainbot);
                        }
                        if (pNivel.boolf1)
                        {
                            InsertarInstru(pNivel.f1bot, button.id);
                            imprimirArray(pNivel.f1bot);
                        }
                        if (pNivel.boolbucle)
                        {
                            InsertarInstru(pNivel.buclebot, button.id);
                            imprimirArray(pNivel.buclebot);
                        }
                        if (pNivel.boolcondicional)
                        {
                            if (pNivel.ifCondition)
                            {
                                InsertarInstru(pNivel.ifbot, button.id);
                                imprimirArray(pNivel.ifbot);
                            }
                            else
                            {
                                InsertarInstru(pNivel.elsebot, button.id);
                                imprimirArray(pNivel.elsebot);
                            }
                        }
                        clickSound.play();
                        clickSound.setVolume(7.f);
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
        bool boolsemaforo = true;
        if (pNivel.booliniciar)
        {
            cout << "cant mov" << pNivel.contadorMovimientos << moving << endl;

            if (pNivel.contadorMovimientos < sizeof(pNivel.mainbot) / sizeof(pNivel.mainbot[0]) && moving == false && pNivel.girando == false && pNivel.colisionando == false)
            {
                if (pNivel.mainbot[pNivel.contadorMovimientos] != 7)
                {
                    // al parecer en el mainbot no esta el 4
                    // ingresa  aui pero el movimiento lo marca como 0 y no como el 4 q es el foco
                    cout << "movimiento numero" << pNivel.mainbot[pNivel.contadorMovimientos] << endl;
                    pNivel.movimiento = pNivel.mainbot[pNivel.contadorMovimientos];
                    cout << "mov distinto7" << pNivel.movimiento << endl;
                }
                if (pNivel.contadorMovimientos != 0)
                {
                    if (pNivel.mainbot[pNivel.contadorMovimientos - 1] == 7)
                    {
                        cout << "cambio lm " << endl;
                        pNivel.mainbot[pNivel.contadorMovimientos - 1] = pNivel.lastmov;
                    }
                }
                // int movimiento = mainbot[contadorMovimientos];

                if (pNivel.movimiento == 2 || pNivel.movimiento == 3)
                {
                    cout << "mov" << pNivel.movimiento << endl;

                    pNivel.lastmov = pNivel.movimiento;
                    pNivel.mainbot[pNivel.contadorMovimientos] = 7;
                    // Cambia la dirección cíclicamente
                    updateDirection(contador, pNivel.movimiento);
                    cout << contador << "----------------------" << endl;
                    miraNE = estados[contador].miraNE;
                    miraNO = estados[contador].miraNO;
                    miraSO = estados[contador].miraSO;
                    miraSE = estados[contador].miraSE;
                    pNivel.girando = true;
                    pNivel.contadorMovimientos++;
                }
                else if (pNivel.movimiento == 1)
                {
                    cout << "mov" << pNivel.movimiento << endl;

                    pNivel.lastmov = pNivel.movimiento;
                    pNivel.mainbot[pNivel.contadorMovimientos] = 7;
                    // Mover en la dirección actual
                    Estado estado;
                    estado.miraNE = miraNE;
                    estado.miraNO = miraNO;
                    estado.miraSO = miraSO;
                    estado.miraSE = miraSE;

                    move2(targetPosition, moving, estado, xIso, yIso);
                    pNivel.contadorMovimientos++;
                }
                //
                else if (pNivel.movimiento == 4)
                {
                    cout << "mov4" << endl;
                    if (pNivel.laststate == -1)
                    {
                        pNivel.laststate = state;
                    }

                    if (pNivel.laststate == 2 || pNivel.laststate == 1 && pNivel.isBlockSemaforo)
                    {
                        pNivel.lastmov = pNivel.movimiento;
                        pNivel.mainbot[pNivel.contadorMovimientos] = 7;

                        cout << "entre en condicional" << endl;
                        if (pNivel.contadorMovIf < sizeof(pNivel.ifbot) / sizeof(pNivel.ifbot[0]) && moving == false && pNivel.girando == false)
                        {

                            if (pNivel.ifbot[pNivel.contadorMovIf] != 7)
                            {
                                pNivel.movimientoIf = pNivel.ifbot[pNivel.contadorMovIf];
                            }
                            if (pNivel.contadorMovIf != 0)
                            {
                                if (pNivel.ifbot[pNivel.contadorMovIf - 1] == 7)
                                {
                                    pNivel.ifbot[pNivel.contadorMovIf - 1] = pNivel.lastmovIf;
                                }
                            }

                            if (pNivel.movimientoIf == 2 || pNivel.movimientoIf == 3)
                            {
                                pNivel.lastmovIf = pNivel.movimientoIf;
                                pNivel.ifbot[pNivel.contadorMovIf] = 7;
                                // Cambia la dirección cíclicamente
                                updateDirection(contador, pNivel.movimientoIf);
                                cout << contador << "----------------------" << endl;
                                miraNE = estados[contador].miraNE;
                                miraNO = estados[contador].miraNO;
                                miraSO = estados[contador].miraSO;
                                miraSE = estados[contador].miraSE;
                                pNivel.girando = true;
                                pNivel.contadorMovIf++;
                            }
                            else if (pNivel.movimientoIf == 1)
                            {
                                pNivel.lastmovIf = pNivel.movimientoIf;
                                pNivel.ifbot[pNivel.contadorMovIf] = 7;
                                // Mover en la dirección actual
                                Estado estado;
                                estado.miraNE = miraNE;
                                estado.miraNO = miraNO;
                                estado.miraSO = miraSO;
                                estado.miraSE = miraSE;

                                move2(targetPosition, moving, estado, xIso, yIso);
                                pNivel.contadorMovIf++;
                            }
                            else if (pNivel.movimientoIf == 0)
                            {
                                pNivel.contadorMovIf = 0;
                                pNivel.contadorMovimientos++;
                            }

                            // Avanzar en el array de movimientos
                        }
                        else if (pNivel.contadorMovIf >= 8)
                        {
                            pNivel.ifbot[pNivel.contadorMovIf - 1] = pNivel.lastmovIf;
                            pNivel.contadorMovIf = 0;
                            pNivel.contadorMovimientos++;
                            pNivel.laststate = -1;
                        }
                    } //-----------------------------ELSE-----------------------------------
                    else if (pNivel.laststate == 0 && pNivel.isBlockSemaforo)
                    {
                        pNivel.lastmov = pNivel.movimiento;
                        pNivel.mainbot[pNivel.contadorMovimientos] = 7;

                        cout << "entre en condicional" << endl;
                        if (pNivel.contadorMovElse < sizeof(pNivel.elsebot) / sizeof(pNivel.elsebot[0]) && moving == false && pNivel.girando == false)
                        {
                            if (pNivel.elsebot[pNivel.contadorMovElse] != 7)
                            {
                                pNivel.movimientoElse = pNivel.elsebot[pNivel.contadorMovElse];
                            }
                            if (pNivel.contadorMovElse != 0)
                            {
                                if (pNivel.elsebot[pNivel.contadorMovElse - 1] == 7)
                                {
                                    pNivel.elsebot[pNivel.contadorMovElse - 1] = pNivel.lastmovElse;
                                }
                            }

                            if (pNivel.movimientoElse == 2 || pNivel.movimientoElse == 3)
                            {
                                pNivel.lastmovElse = pNivel.movimientoElse;
                                pNivel.elsebot[pNivel.contadorMovElse] = 7;
                                // Cambia la dirección cíclicamente
                                updateDirection(contador, pNivel.movimientoElse);
                                cout << contador << "----------------------" << endl;
                                miraNE = estados[contador].miraNE;
                                miraNO = estados[contador].miraNO;
                                miraSO = estados[contador].miraSO;
                                miraSE = estados[contador].miraSE;
                                pNivel.girando = true;
                                pNivel.contadorMovElse++;
                            }
                            else if (pNivel.movimientoElse == 1)
                            {
                                pNivel.lastmovElse = pNivel.movimientoElse;
                                pNivel.elsebot[pNivel.contadorMovElse] = 7;
                                // Mover en la dirección actual
                                Estado estado;
                                estado.miraNE = miraNE;
                                estado.miraNO = miraNO;
                                estado.miraSO = miraSO;
                                estado.miraSE = miraSE;

                                move2(targetPosition, moving, estado, xIso, yIso);
                                pNivel.contadorMovElse++;
                            }
                            else if (pNivel.movimientoElse == 0)
                            {
                                pNivel.contadorMovElse = 0;
                                pNivel.contadorMovimientos++;
                            }

                            // Avanzar en el array de movimientos
                        }
                        else if (pNivel.contadorMovElse >= 8)
                        {
                            pNivel.elsebot[pNivel.contadorMovElse - 1] = pNivel.lastmovElse;
                            pNivel.contadorMovElse = 0;
                            pNivel.contadorMovimientos++;
                            int laststate = -1;
                        }
                    }
                }
                else if (pNivel.movimiento == 5)
                {
                    pNivel.lastmov = pNivel.movimiento;
                    pNivel.mainbot[pNivel.contadorMovimientos] = 7;

                    cout << "gaaaa" << endl;
                    if (pNivel.contadorMovf1 < sizeof(pNivel.f1bot) / sizeof(pNivel.f1bot[0]) && moving == false && pNivel.girando == false)
                    {

                        if (pNivel.f1bot[pNivel.contadorMovf1] != 7)
                        {
                            pNivel.movimientof1 = pNivel.f1bot[pNivel.contadorMovf1];
                        }
                        if (pNivel.contadorMovf1 != 0)
                        {
                            if (pNivel.f1bot[pNivel.contadorMovf1 - 1] == 7)
                            {
                                pNivel.f1bot[pNivel.contadorMovf1 - 1] = pNivel.lastmovf1;
                            }
                        }

                        if (pNivel.movimientof1 == 2 || pNivel.movimientof1 == 3)
                        {
                            pNivel.lastmovf1 = pNivel.movimientof1;
                            pNivel.f1bot[pNivel.contadorMovf1] = 7;
                            // Cambia la dirección cíclicamente
                            updateDirection(contador, pNivel.movimientof1);
                            cout << contador << "----------------------" << endl;
                            miraNE = estados[contador].miraNE;
                            miraNO = estados[contador].miraNO;
                            miraSO = estados[contador].miraSO;
                            miraSE = estados[contador].miraSE;
                            pNivel.girando = true;
                            pNivel.contadorMovf1++;
                        }
                        else if (pNivel.movimientof1 == 1)
                        {
                            pNivel.lastmovf1 = pNivel.movimientof1;
                            pNivel.f1bot[pNivel.contadorMovf1] = 7;
                            // Mover en la dirección actual
                            Estado estado;
                            estado.miraNE = miraNE;
                            estado.miraNO = miraNO;
                            estado.miraSO = miraSO;
                            estado.miraSE = miraSE;

                            move2(targetPosition, moving, estado, xIso, yIso);
                            pNivel.contadorMovf1++;
                        }
                        else if (pNivel.movimientof1 == 0)
                        {
                            pNivel.contadorMovf1 = 0;
                            pNivel.contadorMovimientos++;
                        }

                        // Avanzar en el array de movimientos
                    }
                    else if (pNivel.contadorMovf1 >= 8)
                    {
                        pNivel.f1bot[pNivel.contadorMovf1 - 1] = pNivel.lastmovf1;
                        pNivel.contadorMovf1 = 0;
                        pNivel.contadorMovimientos++;
                    }
                }
                else if (pNivel.movimiento == 6 && pNivel.currentIteraciones < counter)
                {
                    pNivel.lastmov = pNivel.movimiento;
                    pNivel.mainbot[pNivel.contadorMovimientos] = 7;

                    cout << "bucleoco" << endl;
                    if (pNivel.contadorMovbucle < sizeof(pNivel.buclebot) / sizeof(pNivel.buclebot[0]) && moving == false && pNivel.girando == false)
                    {
                        if (pNivel.buclebot[pNivel.contadorMovbucle] != 7)
                        {
                            pNivel.movimientoBucle = pNivel.buclebot[pNivel.contadorMovbucle];
                        }
                        if (pNivel.contadorMovbucle != 0)
                        {
                            if (pNivel.buclebot[pNivel.contadorMovbucle - 1] == 7)
                            {
                                pNivel.buclebot[pNivel.contadorMovbucle - 1] = pNivel.lastmovBucle;
                            }
                        }

                        if (pNivel.movimientoBucle == 2 || pNivel.movimientoBucle == 3)
                        {
                            pNivel.lastmovBucle = pNivel.movimientoBucle;
                            pNivel.buclebot[pNivel.contadorMovbucle] = 7;
                            // Cambia la dirección cíclicamente
                            updateDirection(contador, pNivel.movimientoBucle);
                            cout << contador << "----------------------" << endl;
                            miraNE = estados[contador].miraNE;
                            miraNO = estados[contador].miraNO;
                            miraSO = estados[contador].miraSO;
                            miraSE = estados[contador].miraSE;
                            pNivel.girando = true;
                            pNivel.contadorMovbucle++;
                        }
                        else if (pNivel.movimientoBucle == 1)
                        {
                            pNivel.lastmovBucle = pNivel.movimientoBucle;
                            pNivel.buclebot[pNivel.contadorMovbucle] = 7;
                            // Mover en la dirección actual
                            Estado estado;
                            estado.miraNE = miraNE;
                            estado.miraNO = miraNO;
                            estado.miraSO = miraSO;
                            estado.miraSE = miraSE;

                            move2(targetPosition, moving, estado, xIso, yIso);
                            pNivel.contadorMovbucle++;
                        }
                        else if (pNivel.movimientoBucle == 0)
                        {
                            pNivel.contadorMovbucle = 0;
                            pNivel.currentIteraciones++;
                        }

                        // Avanzar en el array de movimientos
                    }

                    else if (pNivel.contadorMovbucle >= 4)
                    {
                        pNivel.buclebot[pNivel.contadorMovbucle - 1] = pNivel.lastmovBucle;
                        pNivel.contadorMovbucle = 0;
                        pNivel.currentIteraciones++;
                    }
                }

                else if (pNivel.movimiento == 0 && contdebug == 1)
                {
                    contdebug = 0;
                    cout << "Llegue al final del array de movimientos 0000" << endl;
                    pNivel.mainbot[pNivel.contadorMovimientos - 1] = pNivel.lastmov;

                    pNivel.contadorMovimientos = 0;
                    pNivel.booliniciar = false;
                    pNivel.contadorMovimientos = 0;
                    pNivel.contadorMovf1 = 0;
                    pNivel.contadorMovbucle = 0;
                    pNivel.booliniciar = false;
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
                    // mapaActual = (mapaActual + 1) % 5;

                    // posXIso = 0;
                    // posYISo = 0;

                    for (int i = 0; i < 12; ++i)
                    {
                        pNivel.mainbot[i] = 0;
                    }
                    for (int i = 0; i < 8; ++i)
                    {
                        pNivel.f1bot[i] = 0;
                    }
                    for (int i = 0; i < 4; ++i)
                    {
                        pNivel.buclebot[i] = 0;
                    }
                    for (int i = 0; i < 8; ++i)
                    {
                        pNivel.ifbot[i] = 0;
                    }
                    for (int i = 0; i < 8; ++i)
                    {
                        pNivel.elsebot[i] = 0;
                    }
                    contador = 0;
                    cout << "hhhhh" << endl;
                    miraNE = false;
                    miraNO = false;
                    miraSO = false;
                    miraSE = true;
                    makibot.setOrigin(20.f, 30.f);
                    makibot.setPosition(300.f, 160.f + yIso / 2.f);

                    // Actualizar targetPosition a la nueva posición inicial
                    targetPosition = makibot.getPosition();
                    miraNE = false;
                    miraNO = false;
                    miraSO = false;
                    miraSE = true;

                    crearSpritesPiso(tiles, mapas[pNivel.mapaActual], texturaLozaAzul, texturaPiso);
                    crearSpritesBloques(bloques, mapas[pNivel.mapaActual], texturaBloque);
                    configurarSprites(tiles2d, matrices2d[pNivel.mapaActual], texturaBloque2d, texturaLozaAzul2D, texturaPiso2d);

                    // Detener movimiento
                    moving = false;
                }
                else if (pNivel.movimiento == 0 && contdebug == 0)
                {
                    cout << ":CC INTENTO" << endl;
                    miraNE = false;
                    miraNO = false;
                    miraSO = false;
                    miraSE = true;
                    contdebug++;
                    if (miraNE || miraNO)
                    {
                        makibot.setTextureRect(framesB[pNivel.currentFrame]);
                    }
                    else if (miraSO || miraSE)
                    {
                        makibot.setTextureRect(framesF[pNivel.currentFrame]);
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
                else if (pNivel.currentIteraciones == counter)
                {
                    cout << "ups entre conter" << endl;
                    pNivel.contadorMovimientos++;
                    pNivel.currentIteraciones = 0;
                }

                // Avanzar en el array de movimientos
                // pNivel.contadorMovimientos++;
            }
            else if (!moving && !pNivel.girando && !pNivel.colisionando)
            {
                cout << "Llegue al final del array de movimientos" << endl;
                pNivel.mainbot[pNivel.contadorMovimientos - 1] = pNivel.lastmov;
                pNivel.contadorMovimientos = 0;
                pNivel.booliniciar = false;
            }
        }

        // Actualizar la animación del sprite
        if (moving || pNivel.colisionando)
        {
            // Verificar colisión con bloques
            Vector2f newPosition = targetPosition;
            int posXIso, posYISo;
            Vector2i indices = calculateGridIndices(newPosition, pos_origin);

            posXIso = indices.x;
            posYISo = indices.y;

            cout << posXIso << posYISo << endl;

            if (mapas[pNivel.mapaActual][posXIso][posYISo] == -1 && posXIso != -1 && posYISo != -1)
            {
                pNivel.contadorMovimientos = 0;
                pNivel.contadorMovf1 = 0;
                pNivel.contadorMovbucle = 0;
                pNivel.booliniciar = false;
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
                pNivel.mapaActual = (pNivel.mapaActual + 1) % 5;

                posXIso = 0;
                posYISo = 0;

                for (int i = 0; i < 12; ++i)
                {
                    pNivel.mainbot[i] = 0;
                }
                for (int i = 0; i < 8; ++i)
                {
                    pNivel.f1bot[i] = 0;
                }
                for (int i = 0; i < 4; ++i)
                {
                    pNivel.buclebot[i] = 0;
                }
                for (int i = 0; i < 8; ++i)
                {
                    pNivel.ifbot[i] = 0;
                }
                for (int i = 0; i < 8; ++i)
                {
                    pNivel.elsebot[i] = 0;
                }
                contador = 0;
                cout << "hhhhh" << endl;

                miraNE = false;
                miraNO = false;
                miraSO = false;
                miraSE = true;

                if (miraNE || miraNO)
                {
                    makibot.setTextureRect(framesB[pNivel.currentFrame]);
                }
                else if (miraSO || miraSE)
                {
                    makibot.setTextureRect(framesF[pNivel.currentFrame]);
                }
                if (miraNE)
                    makibot.setScale(1.f, 1.f);
                else if (miraNO)
                    makibot.setScale(-1.f, 1.f);
                else if (miraSO)
                    makibot.setScale(-1.f, 1.f);
                else if (miraSE)
                    makibot.setScale(1.f, 1.f);

                makibot.setOrigin(20.f, 30.f);
                makibot.setPosition(300.f, 160.f + yIso / 2.f);

                // Actualizar targetPosition a la nueva posición inicial
                targetPosition = makibot.getPosition();

                crearSpritesPiso(tiles, mapas[pNivel.mapaActual], texturaLozaAzul, texturaPiso);
                crearSpritesBloques(bloques, mapas[pNivel.mapaActual], texturaBloque);
                configurarSprites(tiles2d, matrices2d[pNivel.mapaActual], texturaBloque2d, texturaLozaAzul2D, texturaPiso2d);

                // Detener movimiento
                moving = false;
            }

            if (!pNivel.colisionando)
            {
                if (!(posXIso >= 0 && posXIso < gridSize && posYISo >= 0 && posYISo < gridSize) ||
                    (mapas[pNivel.mapaActual][posXIso][posYISo] != 0 && mapas[pNivel.mapaActual][posXIso][posYISo] != -1 && mapas[pNivel.mapaActual][posXIso][posYISo] != -2))
                {
                    pNivel.colisionando = true;
                    moving = false;
                    targetPosition = makibot.getPosition();
                }

                else
                {

                    if (mapas[pNivel.mapaActual][posXIso][posYISo] == -2)
                    {
                        cout << "cambio if-else" << pNivel.laststate << endl;
                        pNivel.isBlockSemaforo == 1;
                    }
                    if (animationClock.getElapsedTime().asSeconds() > animationSpeed)
                    {
                        if (miraNE || miraNO)
                        {
                            pNivel.currentFrame = (pNivel.currentFrame + 1) % framesB.size();
                            makibot.setTextureRect(framesB[pNivel.currentFrame]);
                            cout << "arriba" << endl;
                        }
                        else if (miraSO || miraSE)
                        {
                            pNivel.currentFrame = (pNivel.currentFrame + 1) % framesF.size();
                            makibot.setTextureRect(framesF[pNivel.currentFrame]);
                        }

                        animationClock.restart();
                    }

                    // movimeinto del car
                    Vector2f currentPosition = makibot.getPosition();
                    moveRobot(makibot, targetPosition, currentPosition, xIso, yIso);

                    // Detener el movimiento al alcanzar el objetivo

                    stopMovement(makibot, targetPosition, makibot.getPosition(), pNivel.currentFrame, miraNE, miraNO, miraSO, miraSE, moving, framesB, framesF);
                    // posicion de makibot2D
                    makibot2D.setPosition(57.5f + 15.f * posXIso, 47.4f + 15.f * posYISo);
                    cout << "salida del moving: " << moving << endl;
                    // mapa 2D , se verifica la posicion de los bloques y el makibot
                    updateBlocks(bloques2, mapas[pNivel.mapaActual], gridSize, texturaBloque, lado, posXIso, posYISo);
                }
            }
            else if (pNivel.colisionando == true)
            {
                if (animationClock.getElapsedTime().asSeconds() > 1.5)
                {
                    pNivel.colisionando = false;
                    cout << "cambiando pNivel.colisionando a false" << endl;
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
        else if (pNivel.girando)
        {
            if (animationClock.getElapsedTime().asSeconds() > 1.5)
            {
                if (miraNE || miraNO)
                {
                    makibot.setTextureRect(framesB[pNivel.currentFrame]);
                }
                else if (miraSO || miraSE)
                {
                    makibot.setTextureRect(framesF[pNivel.currentFrame]);
                }
                if (miraNE)
                    makibot.setScale(1.f, 1.f);
                else if (miraNO)
                    makibot.setScale(-1.f, 1.f);
                else if (miraSO)
                    makibot.setScale(-1.f, 1.f);
                else if (miraSE)
                    makibot.setScale(1.f, 1.f);
                pNivel.girando = false;

                animationClock.restart();
            }
        }

        window.clear(grisOscuro);

        // sprite semaforo
        window.draw(SpriteSemaforo);

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
        //---------dibujar semaforo---------------
        for (auto &sema : texSemaforo)
        {

            Sprite sSema(spriteSemaforo);
            sSema.setTextureRect(framesSemaforo[0]);
            sSema.setPosition(sema.posX, sema.posY);
            // cout<<"sem dibuj";
            window.draw(sSema, isoTransform);
        }

        //---------end dibujar semaforo---------------
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
        window.draw(IfandElseSprite);
        for (const auto &button : buttons)
        {
            window.draw(button.sprite);
        }
        renderImagesBlocksWithControls(window, buttonTexture, pNivel.mainbot, 720, 100, event, pNivel.boolmain, clickSound2);
        renderImagesBlocksWithControls(window, buttonTexture, pNivel.f1bot, 720, 320, event, pNivel.boolf1, clickSound2);
        renderImagesBlocksWithControls(window, buttonTexture, pNivel.buclebot, 720, 470, event, pNivel.boolbucle, clickSound2);
        if (pNivel.ifCondition)
            renderImagesBlocksWithControls(window, buttonTexture, pNivel.ifbot, 720, 570, event, pNivel.boolcondicional, clickSound2);
        else
            renderImagesBlocksWithControls(window, buttonTexture, pNivel.elsebot, 720, 570, event, pNivel.boolcondicional, clickSound2);
        window.display();
    }

    return 0;
}
//...........