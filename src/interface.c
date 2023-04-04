#ifndef INTERFACE_C
#define INTERFACE_C

#include "../include/interface.h"

void drawGameOverScreen(level_t *level, player_t *player, gameover_option_t selectedOption)
{
    // Desenhar o nível no fundo com um nível de transparência
    drawLevel(level, player, 0.25f);

    // Desenhar o título da tela
    if (!player->health)
    {
        DrawText("FIM DE JOGO", (SCREEN_WIDTH / 2 - MeasureText("FIM DE JOGO", GAMEOVER_TITLE_FONT_SIZE) / 2), 300,
                GAMEOVER_TITLE_FONT_SIZE, RED);
        DrawText("Vidas Esgotadas",
                 (SCREEN_WIDTH / 2 - MeasureText("Vidas Esgotadas", MENU_FONT_SIZE) / 2), 450,
                 MENU_FONT_SIZE, RAYWHITE);
    }
    else
    {
        DrawText("FIM DE JOGO", (SCREEN_WIDTH / 2 - MeasureText("FIM DE JOGO", GAMEOVER_TITLE_FONT_SIZE) / 2), 300,
                GAMEOVER_TITLE_FONT_SIZE, DARKBLUE);
        DrawText("Impossível Continuar",
                 (SCREEN_WIDTH / 2 - MeasureText("Impossível Continuar", MENU_FONT_SIZE) / 2), 450,
                 MENU_FONT_SIZE, RAYWHITE);
    }

    // Desenhar opções
    DrawText("REINICIAR JOGO",
             (SCREEN_WIDTH / 2 - MeasureText("REINICIAR JOGO", MENU_FONT_SIZE) / 2), 526,
             MENU_FONT_SIZE, RAYWHITE);
    DrawText("SAIR DO JOGO", (SCREEN_WIDTH / 2 - MeasureText("SAIR DO JOGO", MENU_FONT_SIZE) / 2),
             592, MENU_FONT_SIZE, RAYWHITE);

    // Desenhar opção selecionada
    switch (selectedOption)
    {
    case ResetGame:
        DrawText("- REINICIAR JOGO -",
                 (SCREEN_WIDTH / 2 - MeasureText("- REINICIAR JOGO -", MENU_FONT_SIZE) / 2), 527,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case ExitGame:
        DrawText("- SAIR DO JOGO -",
                 (SCREEN_WIDTH / 2 - MeasureText("- SAIR DO JOGO -", MENU_FONT_SIZE) / 2), 593,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    }
}

void drawHUD(player_t *player, float alpha)
{
    static float alphaIntensity = 0.0f;

    DrawText(TextFormat("%i", player->health), 66, 8, HUD_FONT_SIZE, Fade(RAYWHITE, alpha));
    DrawText(TextFormat("%i", player->energy), 177, 8, HUD_FONT_SIZE, Fade(RAYWHITE, alpha));
    DrawText(TextFormat("%i", player->ladders), 311, 8, HUD_FONT_SIZE, Fade(RAYWHITE, alpha));

    // Caso queira, aplicar efeito de pulsar no último ítem minerado
    if(alpha == ALPHA_DISABLE)
    {
        DrawTexture(player->lastMined.texture,
                    (590 - MeasureText(TextFormat(player->lastMined.name), HUD_FONT_SIZE) / 2), 12,
                    Fade(WHITE, alphaIntensity));
        DrawText(player->lastMined.name,
                (620 - MeasureText(TextFormat(player->lastMined.name), HUD_FONT_SIZE) / 2), 8,
                HUD_FONT_SIZE, Fade(player->lastMined.nameColor, alphaIntensity));
        alphaIntensity = fadeTimer(false, LAST_MINED_FADEIN_TIME, LAST_MINED_FADEOFF_TIME, LAST_MINED_FADEOUT_TIME);
    }
    // Caso contrário, aplicar alpha da função
    else
    {
        DrawTexture(player->lastMined.texture,
                    (590 - MeasureText(TextFormat(player->lastMined.name), HUD_FONT_SIZE) / 2), 12,
                    Fade(WHITE, alpha));
        DrawText(player->lastMined.name,
                (620 - MeasureText(TextFormat(player->lastMined.name), HUD_FONT_SIZE) / 2), 8,
                HUD_FONT_SIZE, Fade(player->lastMined.nameColor, alpha));
    }

    DrawText(TextFormat("%i", player->score),
             (956 - MeasureText(TextFormat("%i", player->score), 28)), 8, 28, Fade(RAYWHITE, alpha));
    DrawText(TextFormat("/%i", (int)(1000 * pow(2, player->currentLevel - 1))), 962, 14, 20,
             Fade(DARKGRAY, alpha));
    DrawText(TextFormat("Nível %i", player->currentLevel), 1080, 8, HUD_FONT_SIZE, Fade(RAYWHITE, alpha));
}

void drawLevel(level_t *level, player_t *player, float alpha)
{
    // Desenhar texturas com base na matriz
    Texture2D currentTexture;
    for (int i = 0; i < LVL_HEIGHT; i++)
    {
        for (int j = 0; j < LVL_WIDTH; j++)
        {
            // Verificar elemento atual na matriz
            switch (level->elements[i][j])
            {
            case CHAR_GOLD:
            case CHAR_TITANIUM:
            case CHAR_SILVER:
            case CHAR_URANIUM:
            case CHAR_CAESIUM:
                currentTexture = level->textures[Ore];
                break;
            case CHAR_DIRT:
                currentTexture = level->textures[Dirt];
                break;
            case CHAR_EMPTY:
                currentTexture = level->textures[Background];
            break;
            case CHAR_EDGE:
                currentTexture = level->textures[Edge];
                break;
            case CHAR_LADDER:
                currentTexture = level->textures[Ladder];
                break;
            case CHAR_PLAYER_LADDER:
                if (player->miningMode)
                    currentTexture = level->textures[PlayerLadderPickaxe];
                else
                    currentTexture = level->textures[Ladder];
                break;
            case CHAR_PLAYER:
                if (player->miningMode)
                    currentTexture = level->textures[PlayerPickaxe];
                else
                    currentTexture = level->textures[Player];
                break;
            }

            // Desenhar elemento
            DrawTexture(currentTexture, (j * ELEMENT_SIZE), (i * ELEMENT_SIZE), Fade(WHITE, alpha));
        }
    }

    // Desenhar HUD
    DrawTexture(level->textures[HUD], 0, 0, Fade(WHITE, alpha));
    drawHUD(player, alpha);
}

void drawMenuScreen(Texture2D menuTexture, menu_option_t selectedOption)
{
    // Desenhar fundo do menu
    DrawTexture(menuTexture, 0, 0, WHITE);

    // Desenhar opções
    DrawText("INICIAR JOGO", (SCREEN_WIDTH / 2 - MeasureText("INICIAR JOGO", MENU_FONT_SIZE) / 2),
             394, MENU_FONT_SIZE, RAYWHITE);
    DrawText("RANKING DE PONTOS",
             (SCREEN_WIDTH / 2 - MeasureText("RANKING DE PONTOS", MENU_FONT_SIZE) / 2), 460,
             MENU_FONT_SIZE, RAYWHITE);
    DrawText("EDITOR DE NIVEL",
             (SCREEN_WIDTH / 2 - MeasureText("EDITOR DE NIVEL", MENU_FONT_SIZE) / 2), 526,
             MENU_FONT_SIZE, RAYWHITE);
    DrawText("SAIR", (SCREEN_WIDTH / 2 - MeasureText("SAIR", MENU_FONT_SIZE) / 2), 592,
             MENU_FONT_SIZE, RAYWHITE);

    // Desenhar opção selecionada
    switch (selectedOption)
    {
    case StartGame:
        DrawText("- INICIAR JOGO -",
                 (SCREEN_WIDTH / 2 - MeasureText("- INICIAR JOGO -", MENU_FONT_SIZE) / 2), 395,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case Ranking:
        DrawText("- RANKING DE PONTOS -",
                 (SCREEN_WIDTH / 2 - MeasureText("- RANKING DE PONTOS -", MENU_FONT_SIZE) / 2), 461,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case LevelEditor:
        DrawText("- EDITOR DE NIVEL -",
                 (SCREEN_WIDTH / 2 - MeasureText("- EDITOR DE NIVEL -", MENU_FONT_SIZE) / 2), 527,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case Exit:
        DrawText("- SAIR -", (SCREEN_WIDTH / 2 - MeasureText("- SAIR -", MENU_FONT_SIZE) / 2), 593,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    }
}

void drawRankingScreen(ranking_t *players, int rankingSize, ranking_option_t selectedOption)
{
    // Desenhar título
    DrawText("RANKING", (SCREEN_WIDTH / 2 - MeasureText("RANKING", MENU_FONT_SIZE) / 2),
             MENU_FONT_SIZE + 20, MENU_FONT_SIZE, LIGHTGRAY);

    // Desenhar subtítulos
    DrawText("POSIÇÃO", (SCREEN_WIDTH / 7),
             MENU_FONT_SIZE + 62, MENU_FONT_SIZE, GOLD_COLOR);
    DrawText("PONTUAÇÃO", (SCREEN_WIDTH / 2 - MeasureText("PONTUAÇÃO", MENU_FONT_SIZE) / 2),
             MENU_FONT_SIZE + 62, MENU_FONT_SIZE, GOLD_COLOR);
    DrawText("NOME", (6 * SCREEN_WIDTH / 7 - MeasureText("NOME", MENU_FONT_SIZE)),
             MENU_FONT_SIZE + 62, MENU_FONT_SIZE, GOLD_COLOR);

    // Desenhar jogadores do ranking
    for(int i = 0; i < rankingSize; i++)
    {
        DrawText(TextFormat("%d °", players[i].position), (SCREEN_WIDTH / 7),
                 (124 + (MENU_FONT_SIZE + 20) * i), MENU_FONT_SIZE, RAYWHITE);
        DrawText(TextFormat("%d", players[i].score), (SCREEN_WIDTH / 2 - MeasureText(TextFormat("%d", players[i].score), MENU_FONT_SIZE) / 2),
                 (124 + (MENU_FONT_SIZE + 20) * i), MENU_FONT_SIZE, RAYWHITE);
        DrawText(TextFormat("%s", players[i].name), (6 * SCREEN_WIDTH / 7 - MeasureText(TextFormat("%s", players->name), MENU_FONT_SIZE)),
                 (124 + (MENU_FONT_SIZE + 20) * i), MENU_FONT_SIZE, RAYWHITE);
    }

    // Desenhar opções
    DrawText("SAIR", (SCREEN_WIDTH / 2 - MeasureText("SAIR", MENU_FONT_SIZE) / 2), (124 + (MENU_FONT_SIZE + 20) * rankingSize),
             MENU_FONT_SIZE, RAYWHITE);

    // Desenhar opção selecionada
    switch (selectedOption)
    {
    case ExitRanking:
        DrawText("- SAIR -", (SCREEN_WIDTH / 2 - MeasureText("- SAIR -", MENU_FONT_SIZE) / 2), (124 + (MENU_FONT_SIZE + 20) * rankingSize),
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    }
}

void drawSplashScreen(player_t *player)
{
    // Desenhar splash screen
    Texture2D splashTexture = LoadTexture("backgrounds/splash.png");
    float alphaIntensity = 0.0f;
    alphaIntensity = fadeTimer(true, 0.0f, 0.0f, 0.0f);
    while(alphaIntensity != FADE_OVER)
    {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(splashTexture, 0, 0, Fade(WHITE, alphaIntensity));
        DrawText(TextFormat("Nível %i", player->currentLevel),
                (SCREEN_WIDTH / 2 - MeasureText(TextFormat("Nível %i", player->currentLevel), SPLASH_FONT_SIZE) / 2),
                (SCREEN_HEIGHT - SPLASH_FONT_SIZE) / 2, SPLASH_FONT_SIZE, Fade(RAYWHITE, alphaIntensity));

        alphaIntensity = fadeTimer(false, SPLASH_FADEIN_TIME, SPLASH_FADEOFF_TIME, SPLASH_FADEOUT_TIME);
        
        EndDrawing();
    }
}

#endif