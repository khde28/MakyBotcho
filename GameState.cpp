#include "GameState.h"
#include "MenuState.h"
#include "constantes.h"

using namespace std;
using namespace Constantes;
GameState::GameState(GameStatus &status) : State(status)
{
    estrellaTextura.loadFromFile("images/estrella.png");
    estrellaSprite.setTexture(estrellaTextura);

    estrellaSprite.setOrigin(estrellaTextura.getSize().x / 2.0f, estrellaTextura.getSize().y / 2.0f);

    redSemaforoTextura.loadFromFile("images/srojo.png");
    yellowSemaforoTextura.loadFromFile("images/snaranja.png");
    greenSemaforoTextura.loadFromFile("images/sverde.png");

    semaforoSprite.setTexture(redSemaforoTextura);
    semaforoSprite.setPosition(30, 350);

    iniciarTextura.loadFromFile("images/go.png");
    iniciarSprite.setTexture(iniciarTextura);
    iniciarSprite.setPosition(420, 480);

    numeroContadorTextura[0].loadFromFile("images/0.png");
    numeroContadorTextura[1].loadFromFile("images/1.png");
    numeroContadorTextura[2].loadFromFile("images/2.png");
    numeroContadorTextura[3].loadFromFile("images/3.png");
    numeroContadorTextura[4].loadFromFile("images/4.png");
    numeroContadorTextura[5].loadFromFile("images/5.png");
    numeroContadorSprite.setTexture(numeroContadorTextura[contadorBucle]);
    numeroContadorSprite.setPosition(620, 485);

    incrementTexture.loadFromFile("images/imageincrement.png");
    incrementeSprite.setTexture(incrementTexture);
    incrementeSprite.setPosition(620, 465);

    decrementTexture.loadFromFile("images/imagedecrement.png");
    decrementSprite.setTexture(decrementTexture);
    decrementSprite.setPosition(620, 528);

    pausaSprite.setTexture(pausaTextura);
    pausaSprite.setPosition(30, 650);

    restart1Textura.loadFromFile("images/restart_1.png");
    restart1Sprite.setTexture(restart1Textura);
    restart1Sprite.setPosition(110, 650);

    restartaTextura.loadFromFile("images/restart_a.png");
    restartaSprite.setTexture(restartaTextura);
    restartaSprite.setPosition(200, 650);

    ifTextura.loadFromFile("images/if.png");
    elseTextura.loadFromFile("images/else.png");
    ifAndElseSprite.setTexture(ifTextura);
    ifAndElseSprite.setPosition(620, 590);

    instruccionesTexturas[0].loadFromFile("images/vacio.png");
    instruccionesTexturas[1].loadFromFile("images/avanzar.png");
    instruccionesTexturas[2].loadFromFile("images/giroright.png");
    instruccionesTexturas[3].loadFromFile("images/giroleft.png");
    instruccionesTexturas[4].loadFromFile("images/foco.png");
    instruccionesTexturas[5].loadFromFile("images/f1.png");
    instruccionesTexturas[6].loadFromFile("images/bucle.png");
    instruccionesTexturas[7].loadFromFile("images/current.png");

    for (int i = 0; i < 6; i++)
    {
        instruccionesBotones.emplace_back(sf::Sprite(instruccionesTexturas[i + 1]), i + 1);
        instruccionesBotones[i + 1].sprite.setPosition(WINDOW_WIDTH - instruccionesBotones[i + 1].sprite.getGlobalBounds().width * (6 - i) - 10 * (5 - i), 10);
    }

    gameStatus.pNivel.mainbot.fill(0);
    gameStatus.pNivel.f1bot.fill(0);
    gameStatus.pNivel.buclebot.fill(0);
    gameStatus.pNivel.ifbot.fill(0);
    gameStatus.pNivel.elsebot.fill(0);

    gameStatus.pNivel.boolmain = false;
    gameStatus.pNivel.boolf1 = false;
    gameStatus.pNivel.boolbucle = false;
    gameStatus.pNivel.boolcondicional = false;

    gameStatus.pNivel.mapaActual = 0;

    isoTransform.translate(300.f, 150.f);
    isoTransform.scale(1.f, 0.5f);
    isoTransform.rotate(45.f);

    texturaPiso3d.loadFromFile("images/loza_verde.png");
    texturaPiso2d.loadFromFile("images/loza_verde2d.png");
    texturaBloque3d.loadFromFile("images/loza_naranja.png");
    texturaBloque2d.loadFromFile("images/loza_naranja2d.png");
    texturaMeta3d.loadFromFile("images/loza_azul.png");
    texturaMeta2d.loadFromFile("images/loza_azul.png");
    texturaDecision3d.loadFromFile("images/loza_rojo.png");
    texturaDecision2d.loadFromFile("images/loza_rojo2d.png");

    createMapaPiso3d(mapas3d[gameStatus.pNivel.mapaActual]);
    createMapaBloque3d(mapas3d[gameStatus.pNivel.mapaActual]);
}

void GameState::createMapaPiso3d(const int mapa[GRIDSIZE][GRIDSIZE])
{
    for (int i = 0; i < GRIDSIZE; ++i)
    {
        for (int j = 0; j < GRIDSIZE; ++j)
        {
            if (mapa[i][j] == -1)
            {
                mapaPiso3d[i][j].setTexture(texturaMeta3d);
            }
            else if (mapa[i][j] == -2)
            {
                mapaPiso3d[i][j].setTexture(texturaDecision3d);
            }
            else
            {
                mapaPiso3d[i][j].setTexture(texturaPiso3d);
            }
            mapaPiso3d[i][j].setPosition(i * LADOCUADRADO, j * LADOCUADRADO);
        }
    }
}

void GameState::createMapaBloque3d(const int mapa[GRIDSIZE][GRIDSIZE]){
    //bloques3dBack.clear();
    //bloques3dBack.reserve(GRIDSIZE * GRIDSIZE * 5);
     for (int i = 0; i < GRIDSIZE; ++i)
    {
        for (int j = 0; j < GRIDSIZE; ++j)
        {
            int altura = mapa[i][j];
            
            if (altura > 0)
            {
                for (int k = 1; k <= altura; ++k)
                {
                    sf::Sprite bloque(texturaBloque3d);
                    bloque.setPosition(
                        i * LADOCUADRADO - (8.0f * k), 
                        j * LADOCUADRADO - (8.0f * k)
                    );
                    bloques3dBack.push_back(bloque);
                }
            }
        }
    }
}

void GameState::handleInput(sf::RenderWindow &window, StateManager &manager)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed)
        {
        }
    }
}

void GameState::update(float dt) {}
void GameState::render(sf::RenderWindow &window)
{
    window.clear(sf::Color(50, 50, 50, 200));
    window.display();
}
