#ifndef MAIN_C
#define MAIN_C

#include "../include/main.h"

int main()
{
    // Inicializar jogador
    player_t player = {{0}, {1, 2}, {'\0'}, 0, 3, 100, 20, 1, false};

    // Inicializar janela do jogo
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TerraINF");
    SetTargetFPS(GAME_FRAMERATE);

    menu_option_t selected;
    while (!WindowShouldClose())
    {
        // Ler opção do usuário e direcionar à respectiva tela
        selected = startMenu();
        switch (selected)
        {
        case StartGame:
            startGame(&player);
            break;
        case Ranking:
            startRanking();
            break;
        case LevelEditor:
            break;
        case Exit:
            CloseWindow();
            break;
        }
    }

    return 0;
}

gameover_option_t gameOver(level_t *level, player_t *player)
{
    gameover_option_t selected = ResetGame;

    // Tentar abrir o arquivo de ranking existente
    ranking_t players[MAX_RANKING_SIZE];
    if (!readRankingFile("ranking/ranking.bin", players))
    {
        // Verificar criação do arquivo com sucesso
        if(!createRankingFile("ranking/ranking.bin", MAX_RANKING_SIZE))
            readRankingFile("ranking/ranking.bin", players);
    }

    // Verificar se a pontuação do jogador é maior que alguma já existente
    int firstAlteredPosition = 0; // Nula para indicar nenhuma alteração
    for(int i = MAX_RANKING_SIZE - 1; i >= 0; i--)
        if(player->score > players[i].score)
            firstAlteredPosition = players[i].position;

    bool confirmed = false;
    // Reiniciar temporização
    uninterruptTimer(true, 0.0f);
    while (!confirmed)
    {
        // Se passaram GAMEOVER_NAME_DELAY segundos e houve alguma posição alterada no ranking
        if(uninterruptTimer(false, GAMEOVER_NAME_DELAY) && firstAlteredPosition > 0 && firstAlteredPosition <= MAX_RANKING_SIZE)
        {
            highscore(player);
        }
        else
        {
            // Verificar navegação de seleção
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            {
                if (selected < ExitGame)
                    selected++;
            }
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            {
                if (selected > ResetGame)
                    selected--;
            }

            // Verificar confirmação de seleção
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            {
                // Confirmar seleção
                confirmed = true;
            }

            BeginDrawing();
            ClearBackground(BLACK);

            drawGameOverScreen(level, player, selected, ALPHA_DISABLE);
            EndDrawing();
        }
    }

    // Atualizar as posições do vetor local de ranking
    updateRankingPositions(player, players, MAX_RANKING_SIZE, firstAlteredPosition);

    // Caso haja alterações nas posições do ranking
    if(firstAlteredPosition > 0 && firstAlteredPosition <= rankingSize)
    {
        for(int i = (firstAlteredPosition - 1); i < MAX_RANKING_SIZE; i++)
            writeRankingPosition("ranking/ranking.bin", &players[i]);
        firstAlteredPosition = 0;
    }

    // Retornar opção selecionada se confirmado
    return confirmed ? selected : ExitGame;
}

void highScore(player_t *player)
{
    int letterCount = 0;
    float frameCounter = 0;
    bool blinkUnderscore = false;

    bool nameConfirmed = false;
    while(!nameConfirmed)
    {
        // Ler caractere do teclado (Unicode)
        int key = GetCharPressed();

        // Checar se foram pressionadas mais de uma tecla
        while (key > 0)
        {
            // Ler apenas letras do alfabeto (maiúsculas e minúsculas)
            if (((key >= 65 && key <= 90) || (key >= 122 && key <= 90)) && (letterCount < MAX_PLAYER_NAME))
            {
                // Converte as letras para maiúsculas
                player->name[letterCount] = toupper((char)key);
                player->name[letterCount + 1] = '\0';
                letterCount++;
            }

            // Checar próximo caractere do buffer
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0)
                letterCount = 0;
            players->name[letterCount] = '\0';
        }

        if((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) && letterCount == MAX_PLAYER_NAME)
            nameConfirmed = true;

        frameCounter++;
        if (frameCounter == GAME_FRAMERATE * BLINK_TIME)
        {
            blinkUnderscore = !blinkUnderscore;
            frameCounter = 0;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        drawGameOverScreen(level, player, selected, ALPHA_DISABLE);

        EndDrawing();

    }
}

void startGame(player_t *player)
{
    // ---------------------------------------------------------------------------------------- //
    // Initialize                                                                               //
    // ---------------------------------------------------------------------------------------- //

    // Inicializar a vida do jogador e a tecla pressionada por esse
    player->health = 3;
    player->currentLevel = 1;
    KeyboardKey direction = KEY_S;

    // Carregar nível inicial
    level_t level;
    int currentLevel = player->currentLevel;
    loadLevel(&level, player);
    drawSplashScreen(player);
    gameover_option_t gameOverOption = ResetGame;
    while (!(WindowShouldClose() || gameOverOption == ExitGame))
    {
        // ------------------------------------------------------------------------------------ //
        // Update                                                                               //
        // ------------------------------------------------------------------------------------ //

        // Verificar se as condições de gameover ocorreram
        if(!player->health || !level.oreCount)
        {
            gameOverOption = gameOver(&level, player);
            if (gameOverOption == ResetGame)
            {
                player->health = 3;
                player->currentLevel = 1;
                currentLevel = player->currentLevel;
                loadLevel(&level, player);
                drawSplashScreen(player);
            }
        }

        // Verificar troca de nível
        if (currentLevel != player->currentLevel)
        {
            currentLevel = player->currentLevel;
            loadLevel(&level, player);
            drawSplashScreen(player);
        }

        // Verificar movimentação
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
        {
            moveHorizontal(&level, player, 1);
            direction = KEY_D;
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
        {
            moveHorizontal(&level, player, -1);
            direction = KEY_A;
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
        {
            moveVertical(&level, player, -1);
            direction = KEY_W;
        }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
        {
            moveVertical(&level, player, 1);
            direction = KEY_S;
        }

        // Verificar modo mineração
        if (IsKeyPressed(KEY_ONE))
            player->miningMode = !player->miningMode;

        // Verificar mineração
        if (IsKeyPressed(KEY_SPACE) && player->miningMode)
            mine(&level, player, direction);

        // Verificar posicionamento de escada
        if (IsKeyPressed(KEY_LEFT_SHIFT))
            placeLadder(&level, player);

        // ------------------------------------------------------------------------------------ //
        // Draw                                                                                 //
        // ------------------------------------------------------------------------------------ //
        BeginDrawing();
        ClearBackground(BLACK);

        // Desenhar texturas do nível
        drawLevel(&level, player, ALPHA_DISABLE);

        EndDrawing();
    }
}

menu_option_t startMenu(void)
{
    // Carregar sprites
    Texture2D menuTexture = LoadTexture("backgrounds/menu.png");

    // Selecionar primeira opção do menu
    static menu_option_t selected = StartGame;

    bool confirmed = false;
    while (!(WindowShouldClose() || confirmed))
    {
        // Verificar navegação de seleção
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            if (selected < Exit)
                selected++;
        }
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            if (selected > StartGame)
                selected--;
        }

        // Verificar confirmação de seleção
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
        {
            // Confirmar seleção
            confirmed = true;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        drawMenuScreen(menuTexture, selected);

        EndDrawing();
    }

    // Retornar opção selecionada se confirmado
    return confirmed ? selected : Exit;
}

ranking_option_t startRanking(void)
{
    // Tentar abrir o arquivo de ranking existente
    ranking_t players[MAX_RANKING_SIZE];
    if (!readRankingFile("ranking/ranking.bin", players))
    {
        // Verificar criação do arquivo com sucesso
        if(!createRankingFile("ranking/ranking.bin", MAX_RANKING_SIZE))
            readRankingFile("ranking/ranking.bin", players);
    }

    ranking_option_t selected = ExitRanking;

    bool confirmed = false;
    while (!confirmed)
    {
        // Verificar confirmação de seleção
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
        {
            // Confirmar seleção
            confirmed = true;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        drawRankingScreen(players, MAX_RANKING_SIZE, selected);

        EndDrawing();
    }

    return selected;
}

#endif