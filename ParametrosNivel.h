#pragma once
#include <iostream>
#include <array>
using namespace std;

struct ParametrosNivel
{
    int state;       // 0: Rojo, 1: Naranja, 2: Verde
    int numeroCiclo; // cantidad de ciclos 1-10

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