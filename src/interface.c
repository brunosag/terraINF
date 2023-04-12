#ifndef INTERFACE_C
#define INTERFACE_C

#include "../include/interface.h"

Image createLevelMiniature(level_t *level, player_t *player)
{
    Image miniature;

    // Desenhar nível sem HUD para salvar miniatura
    BeginDrawing();
    ClearBackground(BLACK);
    drawLevel(level, player);
    EndDrawing();
    SwapScreenBuffer();

    // Salvar miniatura no tamanho adequado
    miniature = LoadImageFromScreen();
    ImageResizeNN(&miniature, 120, 80);

    // Limpar tela depois de tirar screenshot
    BeginDrawing();
    ClearBackground(BLACK);
    EndDrawing();

    return miniature;
}

void drawCustomLevelHUD(level_t *level, player_t *player)
{
    static float alphaIntensity = 0.0f;

    // Desenhar fundo da HUD
    DrawTexture(level->textures[HUD], 0, 0, WHITE);

    // Desenhar informações da HUD
    DrawText(TextFormat("%i", player->health), 66, 8, HUD_FONT_SIZE, RAYWHITE);
    DrawText(TextFormat("%i", player->energy), 177, 8, HUD_FONT_SIZE, RAYWHITE);
    DrawText(TextFormat("%i", player->ladders), 311, 8, HUD_FONT_SIZE, RAYWHITE);
    DrawText(TextFormat("%i", player->score), (1089 - MeasureText(TextFormat("%i", player->score), 28)), 8, 28,
             RAYWHITE);
    DrawText(TextFormat("/%i", level->maxScore), 1095, 14, 20, DARKGRAY);

    // Desenhar último item minerado com efeito de pulsar
    DrawTexture(player->lastMined.texture, (645 - MeasureText(TextFormat(player->lastMined.name), HUD_FONT_SIZE) / 2),
                12, Fade(WHITE, alphaIntensity));
    DrawText(player->lastMined.name, (675 - MeasureText(TextFormat(player->lastMined.name), HUD_FONT_SIZE) / 2), 8,
             HUD_FONT_SIZE, Fade(player->lastMined.nameColor, alphaIntensity));
    alphaIntensity = fadeTimer(false, LAST_MINED_FADEIN_TIME, LAST_MINED_FADEOFF_TIME, LAST_MINED_FADEOUT_TIME);
}

void drawCustomLevelsMenu(Texture2D background, custom_levels_menu_t *menuData, int customLevelsAmount,
                          int selectedOption)
{
    // Criar caixas de seleção de nível
    Rectangle levelBox[customLevelsAmount];

    // Desenhar fundo e título
    DrawTexture(background, 0, 0, WHITE);
    DrawText("NIVEIS CUSTOMIZADOS", (SCREEN_WIDTH / 2 - MeasureText("NIVEIS CUSTOMIZADOS", 32) / 2), 26, 32, RAYWHITE);

    // Determinar a posição da informação de cada nível na tela
    for (int i = 0; i < customLevelsAmount; i++)
    {
        levelBox[customLevelsAmount - i - 1].width = 3.0f * SCREEN_WIDTH / 5.0f;
        levelBox[customLevelsAmount - i - 1].x = SCREEN_WIDTH / 5.0f;
        levelBox[customLevelsAmount - i - 1].height = 110;
        levelBox[customLevelsAmount - i - 1].y =
            (SCREEN_HEIGHT / 2.0f) - (levelBox[customLevelsAmount - i - 1].height / 2.0f) -
            (levelBox[customLevelsAmount - i - 1].height / 2.0f + 10) * (customLevelsAmount - 1) +
            (levelBox[customLevelsAmount - i - 1].height + 20) * i;

        // Desenhar as informações de cada nível na tela
        DrawTexture(menuData[customLevelsAmount - i - 1].miniature, levelBox[customLevelsAmount - i - 1].x + 15,
                    levelBox[customLevelsAmount - i - 1].y + 15, WHITE);
        DrawText(menuData[customLevelsAmount - i - 1].name, levelBox[customLevelsAmount - i - 1].x + 155,
                 levelBox[customLevelsAmount - i - 1].y + levelBox[customLevelsAmount - i - 1].height / 4.0f + 4,
                 MENU_FONT_SIZE, RAYWHITE);
        DrawText(menuData[customLevelsAmount - i - 1].dateCreated, levelBox[customLevelsAmount - i - 1].x + 155,
                 levelBox[customLevelsAmount - i - 1].y + levelBox[customLevelsAmount - i - 1].height / 2.0f + 12,
                 MENU_FONT_SIZE - 4, GRAY);
    }

    // Desenhar borda de seleção do respectivo nível
    if (selectedOption != EXIT_CUSTOM_LEVELS_MENU)
        DrawRectangleLinesEx(levelBox[selectedOption], 3.0f, RAYWHITE);

    // Desenhar seleção de opção sair
    DrawText("SAIR", (SCREEN_WIDTH / 2 - MeasureText("SAIR", MENU_FONT_SIZE) / 2), 749, MENU_FONT_SIZE, RAYWHITE);
    if (selectedOption == EXIT_CUSTOM_LEVELS_MENU)
        DrawText("- SAIR -", (SCREEN_WIDTH / 2 - MeasureText("- SAIR -", MENU_FONT_SIZE) / 2), 750, MENU_FONT_SIZE,
                 RAYWHITE);
}

void drawCustomLevelsTextBox(const char *levelName, int nameSize, int maxNameSize, bool blinkUnderscore)
{
    Rectangle backgroundBox = {(SCREEN_WIDTH / 6), (SCREEN_HEIGHT / 6), (2 * SCREEN_WIDTH / 3),
                               (2 * SCREEN_HEIGHT / 3)};
    Rectangle textBox = {(backgroundBox.x + backgroundBox.width / 2 - ((MENU_FONT_SIZE - 6) * maxNameSize) / 2),
                         (backgroundBox.y + backgroundBox.height / 2 - (MENU_FONT_SIZE + 10) / 2),
                         ((MENU_FONT_SIZE - 6) * maxNameSize), (MENU_FONT_SIZE + 10)};

    // Desenhar fundos
    DrawRectangleRec(backgroundBox, Fade(BLACK, 0.75f));
    DrawRectangleLines((int)backgroundBox.x, (int)backgroundBox.y, (int)backgroundBox.width, (int)backgroundBox.height,
                       DARKGRAY);

    // Desenhar textos
    DrawText("SALVAR NOVO NÍVEL",
             (backgroundBox.x + backgroundBox.width / 2 - MeasureText("SALVAR NOVO NÍVEL", 30) / 2), 220, 30, RAYWHITE);
    DrawText("Digite o nome do nível:",
             (backgroundBox.x + backgroundBox.width / 2 - MeasureText("Digite o nome do nível:", MENU_FONT_SIZE) / 2),
             290, MENU_FONT_SIZE, RAYWHITE);

    // Desenhar caixa de texto
    DrawRectangleRec(textBox, DARKGRAY);
    DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RAYWHITE);

    // Desenhar cada caractere digitado para o nome do nível
    DrawText(levelName, (int)textBox.x + 5, (int)textBox.y + 8, MENU_FONT_SIZE, RAYWHITE);

    if (!nameSize)
    {
        // Desenhar undescore piscante
        if (blinkUnderscore)
            DrawText("_", (int)textBox.x + 8 + MeasureText(levelName, MENU_FONT_SIZE), (int)textBox.y + 12,
                     MENU_FONT_SIZE, RAYWHITE);
        // Desenhar opção de confirmar bloqueada
        DrawText("CONFIRMAR",
                 (backgroundBox.x + backgroundBox.width / 2 - MeasureText("CONFIRMAR", MENU_FONT_SIZE) / 2), 500,
                 MENU_FONT_SIZE, DARKGRAY);
    }
    else
    {
        // Desenhar undescore piscante
        if (blinkUnderscore)
            DrawText("_", (int)textBox.x + 8 + MeasureText(levelName, MENU_FONT_SIZE), (int)textBox.y + 12,
                     MENU_FONT_SIZE, RAYWHITE);
        // Desenhar opção de confirmar desbloqueada
        DrawText("- CONFIRMAR -",
                 (backgroundBox.x + backgroundBox.width / 2 - MeasureText("- CONFIRMAR -", MENU_FONT_SIZE) / 2), 500,
                 MENU_FONT_SIZE, DARKGRAY);
        DrawText("- CONFIRMAR -",
                 (backgroundBox.x + backgroundBox.width / 2 - MeasureText("- CONFIRMAR -", MENU_FONT_SIZE) / 2), 501,
                 MENU_FONT_SIZE, RAYWHITE);
    }
}

void drawEditorHUD(level_t *level, editor_option_t selected)
{
    DrawTexture(level->textures[EditorHUD], 0, 0, WHITE);
    DrawText("Editor de Nível", 22, 11, EDITOR_FONT_SIZE, RAYWHITE);
    DrawText("SALVAR", 1082, 11, EDITOR_FONT_SIZE, RAYWHITE);

    // Desenhar slot selecionado
    if (selected == Save)
        DrawText("- SALVAR -", 1064, 10, EDITOR_FONT_SIZE, RAYWHITE);
    else
        DrawTexture(level->textures[SlotSelected], 453 + (37 * selected), 2, WHITE);
}

void drawEditorLevel(level_t *level)
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
            case CHAR_SILVER:
                currentTexture = level->textures[SilverOre];
                break;
            case CHAR_GOLD:
                currentTexture = level->textures[GoldOre];
                break;
            case CHAR_TITANIUM:
                currentTexture = level->textures[TitaniumOre];
                break;
            case CHAR_CAESIUM:
                currentTexture = level->textures[CaesiumOre];
                break;
            case CHAR_URANIUM:
                currentTexture = level->textures[UraniumOre];
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
            case CHAR_PLAYER:
                currentTexture = level->textures[Player];
                break;
            }

            // Desenhar elemento
            DrawTexture(currentTexture, (j * ELEMENT_SIZE), (i * ELEMENT_SIZE), WHITE);
        }
    }
}

void drawGameOverScreen(level_t *level, player_t *player, endgame_option_t selectedOption)
{
    // Desenhar fundo
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.75f));

    // Desenhar o título da tela
    if (!player->health)
    {
        DrawText("FIM DE JOGO", (SCREEN_WIDTH / 2 - MeasureText("FIM DE JOGO", ENDGAME_TITLE_FONT_SIZE) / 2), 300,
                 ENDGAME_TITLE_FONT_SIZE, RED);
        DrawText("Vidas esgotadas.", (SCREEN_WIDTH / 2 - MeasureText("Vidas Esgotadas", MENU_FONT_SIZE) / 2), 450,
                 MENU_FONT_SIZE, RAYWHITE);
    }
    else
    {
        DrawText("FIM DE JOGO", (SCREEN_WIDTH / 2 - MeasureText("FIM DE JOGO", ENDGAME_TITLE_FONT_SIZE) / 2), 300,
                 ENDGAME_TITLE_FONT_SIZE, DARKBLUE);
        DrawText("Impossível continuar.", (SCREEN_WIDTH / 2 - MeasureText("Impossível Continuar", MENU_FONT_SIZE) / 2),
                 450, MENU_FONT_SIZE, RAYWHITE);
    }

    // Desenhar opções
    DrawText("REINICIAR JOGO", (SCREEN_WIDTH / 2 - MeasureText("REINICIAR JOGO", MENU_FONT_SIZE) / 2), 526,
             MENU_FONT_SIZE, RAYWHITE);
    DrawText("SAIR DO JOGO", (SCREEN_WIDTH / 2 - MeasureText("SAIR DO JOGO", MENU_FONT_SIZE) / 2), 592, MENU_FONT_SIZE,
             RAYWHITE);

    // Desenhar opção selecionada
    switch (selectedOption)
    {
    case ResetGame:
        DrawText("- REINICIAR JOGO -", (SCREEN_WIDTH / 2 - MeasureText("- REINICIAR JOGO -", MENU_FONT_SIZE) / 2), 525,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case ExitGame:
        DrawText("- SAIR DO JOGO -", (SCREEN_WIDTH / 2 - MeasureText("- SAIR DO JOGO -", MENU_FONT_SIZE) / 2), 591,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    }
}

void drawHighScoreTextBox(player_t *player, int nameSize, int maxNameSize, bool blinkUnderscore)
{
    Rectangle backgroundBox = {(SCREEN_WIDTH / 6), (SCREEN_HEIGHT / 6), (2 * SCREEN_WIDTH / 3),
                               (2 * SCREEN_HEIGHT / 3)};
    Rectangle textBox = {(backgroundBox.x + backgroundBox.width / 2 - ((MENU_FONT_SIZE - 3) * maxNameSize) / 2),
                         (backgroundBox.y + backgroundBox.height / 2 - (MENU_FONT_SIZE + 10) / 2),
                         ((MENU_FONT_SIZE - 3) * maxNameSize), (MENU_FONT_SIZE + 10)};

    // Desenhar fundos
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_WIDTH, Fade(BLACK, 0.75f));
    DrawRectangleRec(backgroundBox, Fade(BLACK, 0.75f));
    DrawRectangleLines((int)backgroundBox.x, (int)backgroundBox.y, (int)backgroundBox.width, (int)backgroundBox.height,
                       DARKGRAY);

    // Desenhar texto
    DrawText("NOVO RECORDE ATINGIDO!",
             (backgroundBox.x + backgroundBox.width / 2 - MeasureText("NOVO RECORDE ATINGIDO!", MENU_FONT_SIZE) / 2),
             220, MENU_FONT_SIZE, GREEN);
    DrawText("Digite seu nome:",
             (backgroundBox.x + backgroundBox.width / 2 - MeasureText("Digite seu nome:", MENU_FONT_SIZE) / 2), 270,
             MENU_FONT_SIZE, RAYWHITE);

    // Desenhar caixa de texto
    DrawRectangleRec(textBox, DARKGRAY);
    DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RAYWHITE);

    // Desenhar cada caractere digitado para o nome do jogador
    DrawText(player->name, (int)textBox.x + 5, (int)textBox.y + 8, MENU_FONT_SIZE, RAYWHITE);

    if (nameSize < maxNameSize)
    {
        // Desenhar undescore piscante
        if (blinkUnderscore)
            DrawText("_", (int)textBox.x + 8 + MeasureText(player->name, MENU_FONT_SIZE), (int)textBox.y + 12,
                     MENU_FONT_SIZE, RAYWHITE);

        // Desenhar opção de confirmar bloqueada
        DrawText("CONFIRMAR",
                 (backgroundBox.x + backgroundBox.width / 2 - MeasureText("CONFIRMAR", MENU_FONT_SIZE) / 2), 500,
                 MENU_FONT_SIZE, DARKGRAY);
    }
    else
    {
        // Desenhar opção de confirmar desbloqueada
        DrawText("- CONFIRMAR -",
                 (backgroundBox.x + backgroundBox.width / 2 - MeasureText("- CONFIRMAR -", MENU_FONT_SIZE) / 2), 500,
                 MENU_FONT_SIZE, RAYWHITE);
    }
}

void drawHUD(level_t *level, player_t *player)
{
    static float alphaIntensity = 0.0f;

    // Desenhar fundo da HUD
    DrawTexture(level->textures[HUD], 0, 0, WHITE);

    // Desenhar informações da HUD
    DrawText(TextFormat("%i", player->health), 66, 8, HUD_FONT_SIZE, RAYWHITE);
    DrawText(TextFormat("%i", player->energy), 177, 8, HUD_FONT_SIZE, RAYWHITE);
    DrawText(TextFormat("%i", player->ladders), 311, 8, HUD_FONT_SIZE, RAYWHITE);
    DrawText(TextFormat("%i", player->score), (956 - MeasureText(TextFormat("%i", player->score), 28)), 8, 28,
             RAYWHITE);
    DrawText(TextFormat("/%i", level->maxScore), 962, 14, 20, DARKGRAY);
    DrawText(TextFormat("Nível %i", player->currentLevel), 1080, 8, HUD_FONT_SIZE, RAYWHITE);

    // Desenhar último item minerado com efeito de pulsar
    DrawTexture(player->lastMined.texture, (590 - MeasureText(TextFormat(player->lastMined.name), HUD_FONT_SIZE) / 2),
                12, Fade(WHITE, alphaIntensity));
    DrawText(player->lastMined.name, (620 - MeasureText(TextFormat(player->lastMined.name), HUD_FONT_SIZE) / 2), 8,
             HUD_FONT_SIZE, Fade(player->lastMined.nameColor, alphaIntensity));
    alphaIntensity = fadeTimer(false, LAST_MINED_FADEIN_TIME, LAST_MINED_FADEOFF_TIME, LAST_MINED_FADEOUT_TIME);
}

void drawLevel(level_t *level, player_t *player)
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
                    currentTexture = level->textures[PlayerLadder];
                break;
            case CHAR_PLAYER:
                if (player->miningMode)
                    currentTexture = level->textures[PlayerPickaxe];
                else
                    currentTexture = level->textures[Player];
                break;
            }

            // Desenhar elemento
            DrawTexture(currentTexture, (j * ELEMENT_SIZE), (i * ELEMENT_SIZE), WHITE);
        }
    }
}

void drawMenuScreen(Texture2D menuTexture, menu_option_t selectedOption)
{
    // Desenhar fundo do menu
    DrawTexture(menuTexture, 0, 0, WHITE);

    // Desenhar opções
    DrawText("INICIAR JOGO", (SCREEN_WIDTH / 2 - MeasureText("INICIAR JOGO", MENU_FONT_SIZE) / 2), 354, MENU_FONT_SIZE,
             RAYWHITE);
    DrawText("EDITOR DE NIVEL", (SCREEN_WIDTH / 2 - MeasureText("EDITOR DE NIVEL", MENU_FONT_SIZE) / 2), 416,
             MENU_FONT_SIZE, RAYWHITE);
    DrawText("NIVEIS CUSTOMIZADOS", (SCREEN_WIDTH / 2 - MeasureText("NIVEIS CUSTOMIZADOS", MENU_FONT_SIZE) / 2), 478,
             MENU_FONT_SIZE, RAYWHITE);
    DrawText("RANKING DE PONTOS", (SCREEN_WIDTH / 2 - MeasureText("RANKING DE PONTOS", MENU_FONT_SIZE) / 2), 540,
             MENU_FONT_SIZE, RAYWHITE);
    DrawText("SAIR", (SCREEN_WIDTH / 2 - MeasureText("SAIR", MENU_FONT_SIZE) / 2), 602, MENU_FONT_SIZE, RAYWHITE);

    // Desenhar opção selecionada
    switch (selectedOption)
    {
    case StartGame:
        DrawText("- INICIAR JOGO -", (SCREEN_WIDTH / 2 - MeasureText("- INICIAR JOGO -", MENU_FONT_SIZE) / 2), 353,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case LevelEditor:
        DrawText("- EDITOR DE NIVEL -", (SCREEN_WIDTH / 2 - MeasureText("- EDITOR DE NIVEL -", MENU_FONT_SIZE) / 2),
                 415, MENU_FONT_SIZE, RAYWHITE);
        break;
    case CustomLevels:
        DrawText("- NIVEIS CUSTOMIZADOS -",
                 (SCREEN_WIDTH / 2 - MeasureText("- NIVEIS CUSTOMIZADOS -", MENU_FONT_SIZE) / 2), 477, MENU_FONT_SIZE,
                 RAYWHITE);
        break;
    case Ranking:
        DrawText("- RANKING DE PONTOS -", (SCREEN_WIDTH / 2 - MeasureText("- RANKING DE PONTOS -", MENU_FONT_SIZE) / 2),
                 539, MENU_FONT_SIZE, RAYWHITE);
        break;
    case Exit:
        DrawText("- SAIR -", (SCREEN_WIDTH / 2 - MeasureText("- SAIR -", MENU_FONT_SIZE) / 2), 601, MENU_FONT_SIZE,
                 RAYWHITE);
        break;
    }
}

void drawRankingScreen(ranking_t *players, int rankingSize, ranking_option_t selectedOption)
{
    // Desenhar título
    DrawText("RANKING", (SCREEN_WIDTH / 2 - MeasureText("RANKING", MENU_FONT_SIZE) / 2), MENU_FONT_SIZE + 20,
             MENU_FONT_SIZE, LIGHTGRAY);

    // Desenhar subtítulos
    DrawText("POSIÇÃO", (SCREEN_WIDTH / 7), MENU_FONT_SIZE + 62, MENU_FONT_SIZE, GOLD_COLOR);
    DrawText("PONTUAÇÃO", (SCREEN_WIDTH / 2 - MeasureText("PONTUAÇÃO", MENU_FONT_SIZE) / 2), MENU_FONT_SIZE + 62,
             MENU_FONT_SIZE, GOLD_COLOR);
    DrawText("NOME", (6 * SCREEN_WIDTH / 7 - MeasureText("NOME", MENU_FONT_SIZE)), MENU_FONT_SIZE + 62, MENU_FONT_SIZE,
             GOLD_COLOR);

    // Desenhar jogadores do ranking
    for (int i = 0; i < rankingSize; i++)
    {
        DrawText(TextFormat("%d °", players[i].position), (SCREEN_WIDTH / 7), (124 + (MENU_FONT_SIZE + 20) * i),
                 MENU_FONT_SIZE, RAYWHITE);
        DrawText(TextFormat("%d", players[i].score),
                 (SCREEN_WIDTH / 2 - MeasureText(TextFormat("%d", players[i].score), MENU_FONT_SIZE) / 2),
                 (124 + (MENU_FONT_SIZE + 20) * i), MENU_FONT_SIZE, RAYWHITE);
        DrawText(TextFormat("%s", players[i].name),
                 (6 * SCREEN_WIDTH / 7 - MeasureText(TextFormat("%s", players->name), MENU_FONT_SIZE)),
                 (124 + (MENU_FONT_SIZE + 20) * i), MENU_FONT_SIZE, RAYWHITE);
    }

    // Desenhar opções
    DrawText("SAIR", (SCREEN_WIDTH / 2 - MeasureText("SAIR", MENU_FONT_SIZE) / 2),
             (124 + (MENU_FONT_SIZE + 20) * rankingSize), MENU_FONT_SIZE, RAYWHITE);

    // Desenhar opção selecionada
    switch (selectedOption)
    {
    case ExitRanking:
        DrawText("- SAIR -", (SCREEN_WIDTH / 2 - MeasureText("- SAIR -", MENU_FONT_SIZE) / 2),
                 (124 + (MENU_FONT_SIZE + 20) * rankingSize), MENU_FONT_SIZE, RAYWHITE);
        break;
    }
}

void drawSplashScreen(player_t *player, Music *music)
{
    // Desenhar splash screen
    Texture2D splashTexture = LoadTexture("resources/backgrounds/splash.png");
    float alphaIntensity = 0.0f;
    alphaIntensity = fadeTimer(true, 0.0f, 0.0f, 0.0f);
    while (alphaIntensity != FADE_OVER)
    {
        UpdateMusicStream(*music);
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

void drawWinScreen(endgame_option_t selectedOption)
{
    // Desenhar fundo
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.75f));

    // Desenhar título da tela
    DrawText("VOCÊ VENCEU!", (SCREEN_WIDTH / 2 - MeasureText("VOCÊ VENCEU!", ENDGAME_TITLE_FONT_SIZE) / 2), 300,
             ENDGAME_TITLE_FONT_SIZE, GREEN);

    // Desenhar opções
    DrawText("REINICIAR JOGO", (SCREEN_WIDTH / 2 - MeasureText("REINICIAR JOGO", MENU_FONT_SIZE) / 2), 526,
             MENU_FONT_SIZE, RAYWHITE);
    DrawText("SAIR DO JOGO", (SCREEN_WIDTH / 2 - MeasureText("SAIR DO JOGO", MENU_FONT_SIZE) / 2), 592, MENU_FONT_SIZE,
             RAYWHITE);

    // Desenhar opção selecionada
    switch (selectedOption)
    {
    case ResetGame:
        DrawText("- REINICIAR JOGO -", (SCREEN_WIDTH / 2 - MeasureText("- REINICIAR JOGO -", MENU_FONT_SIZE) / 2), 525,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case ExitGame:
        DrawText("- SAIR DO JOGO -", (SCREEN_WIDTH / 2 - MeasureText("- SAIR DO JOGO -", MENU_FONT_SIZE) / 2), 591,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    }
}

#endif