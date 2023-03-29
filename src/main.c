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

menu_option_t startMenu()
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

        // Desenhar fundo do menu
        DrawTexture(menuTexture, 0, 0, WHITE);

        // Desenhar opções
        DrawText("INICIAR JOGO",
                 (SCREEN_WIDTH / 2 - MeasureText("INICIAR JOGO", MENU_FONT_SIZE) / 2), 394,
                 MENU_FONT_SIZE, RAYWHITE);
        DrawText("RANKING DE PONTOS",
                 (SCREEN_WIDTH / 2 - MeasureText("RANKING DE PONTOS", MENU_FONT_SIZE) / 2), 460,
                 MENU_FONT_SIZE, RAYWHITE);
        DrawText("EDITOR DE NIVEL",
                 (SCREEN_WIDTH / 2 - MeasureText("EDITOR DE NIVEL", MENU_FONT_SIZE) / 2), 526,
                 MENU_FONT_SIZE, RAYWHITE);
        DrawText("SAIR", (SCREEN_WIDTH / 2 - MeasureText("SAIR", MENU_FONT_SIZE) / 2), 592,
                 MENU_FONT_SIZE, RAYWHITE);

        // Desenhar opção selecionada
        switch (selected)
        {
        case StartGame:
            DrawText("- INICIAR JOGO -",
                     (SCREEN_WIDTH / 2 - MeasureText("- INICIAR JOGO -", MENU_FONT_SIZE) / 2), 395,
                     MENU_FONT_SIZE, RAYWHITE);
            break;
        case Ranking:
            DrawText("- RANKING DE PONTOS -",
                     (SCREEN_WIDTH / 2 - MeasureText("- RANKING DE PONTOS -", MENU_FONT_SIZE) / 2),
                     461, MENU_FONT_SIZE, RAYWHITE);
            break;
        case LevelEditor:
            DrawText("- EDITOR DE NIVEL -",
                     (SCREEN_WIDTH / 2 - MeasureText("- EDITOR DE NIVEL -", MENU_FONT_SIZE) / 2),
                     527, MENU_FONT_SIZE, RAYWHITE);
            break;
        case Exit:
            DrawText("- SAIR -", (SCREEN_WIDTH / 2 - MeasureText("- SAIR -", MENU_FONT_SIZE) / 2),
                     593, MENU_FONT_SIZE, RAYWHITE);
            break;
        }

        EndDrawing();
    }

    // Retornar opção selecionada se confirmado
    return confirmed ? selected : Exit;
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

gameover_option_t gameOver(level_t *level, player_t *player)
{
    gameover_option_t selected = ResetGame;
    
    bool confirmed = false;
    while (!confirmed)
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

        // Desenhar o nível no fundo com um nível de transparência
        drawLevel(level, player, 0.25f);

        // Desenhar o título da tela
        if(!player->health)
        {
            DrawText("FIM DE JOGO",
                    (SCREEN_WIDTH / 2 - MeasureText("FIM DE JOGO", 100) / 2), 300,
                    100, RED);
            DrawText("Vidas Esgotadas",
                    (SCREEN_WIDTH / 2 - MeasureText("Vidas Esgotadas", MENU_FONT_SIZE) / 2), 450,
                    MENU_FONT_SIZE, RAYWHITE);
        }
        else
        {
            DrawText("FIM DE JOGO",
                    (SCREEN_WIDTH / 2 - MeasureText("FIM DE JOGO", 100) / 2), 300,
                    100, DARKBLUE);
            DrawText("Impossível Continuar",
                    (SCREEN_WIDTH / 2 - MeasureText("Impossível Continuar", MENU_FONT_SIZE) / 2), 450,
                    MENU_FONT_SIZE, RAYWHITE);
        }

        // Desenhar opções
        DrawText("REINICIAR JOGO",
                 (SCREEN_WIDTH / 2 - MeasureText("REINICIAR JOGO", MENU_FONT_SIZE) / 2), 526,
                 MENU_FONT_SIZE, RAYWHITE);
        DrawText("SAIR DO JOGO",
                 (SCREEN_WIDTH / 2 - MeasureText("SAIR DO JOGO", MENU_FONT_SIZE) / 2), 592,
                 MENU_FONT_SIZE, RAYWHITE);

        // Desenhar opção selecionada
        switch (selected)
        {
        case ResetGame:
            DrawText("- REINICIAR JOGO -",
                     (SCREEN_WIDTH / 2 - MeasureText("- REINICIAR JOGO -", MENU_FONT_SIZE) / 2), 
                     527, MENU_FONT_SIZE, RAYWHITE);
            break;
        case ExitGame:
            DrawText("- SAIR DO JOGO -",
                     (SCREEN_WIDTH / 2 - MeasureText("- SAIR DO JOGO -", MENU_FONT_SIZE) / 2),
                     593, MENU_FONT_SIZE, RAYWHITE);
            break;
        }

        EndDrawing();
    }

    // Retornar opção selecionada se confirmado
    return confirmed ? selected : ExitGame;

/*    // Verificar se arquivo de ranking existe
    FILE *rankingFile = fopen("ranking/ranking.bin", "rb+");
    if (rankingFile == NULL)
        rankingFile = createRankingFile(MAX_RANKING_SIZE);

    fclose(rankingFile);
*/
}

#endif
