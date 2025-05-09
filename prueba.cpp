void reiniciar_parametros(){

    contadorMovimientos = 0;
    contadorMovf1 = 0;
    contadorMovbucle = 0;
    booliniciar = false;
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
    //mapaActual = (mapaActual + 1) % 5;

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
    for (int i = 0; i < 8; ++i)
    {
        ifbot[i] = 0;
    }
    for (int i = 0; i < 8; ++i)
    {
        elsebot[i] = 0;
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