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
    DrawText("CUSTOM LEVELS", (SCREEN_WIDTH / 2 - MeasureText("CUSTOM LEVELS", 32) / 2), 26, 32, RAYWHITE);

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

    // Desenhar opção de sair
    DrawText("EXIT", (SCREEN_WIDTH / 2 - MeasureText("EXIT", MENU_FONT_SIZE) / 2), 750, MENU_FONT_SIZE, RAYWHITE);
    if (selectedOption == EXIT_CUSTOM_LEVELS_MENU)
        DrawText("- EXIT -", (SCREEN_WIDTH / 2 - MeasureText("- EXIT -", MENU_FONT_SIZE) / 2), 749, MENU_FONT_SIZE,
                 RAYWHITE);
}

void drawCustomLevelsTextBox(const char *levelName, int nameSize, int maxNameSize, bool blinkUnderscore)
{
    Rectangle outerBox = {170, 220, 860, 360};
    Rectangle innerBox = {315, 380, 570, 76};

    // Desenhar fundo
    DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Fade(BLACK, 0.75f));

    // Desenhar caixas
    DrawRectangleRec(outerBox, DARKGRAY);
    DrawRectangleLinesEx(outerBox, 2, LIGHTGRAY);
    DrawRectangleRec(innerBox, BLACK);
    DrawRectangleLinesEx(innerBox, 2, RAYWHITE);

    // Desenhar texto
    DrawText("SAVE NEW LEVEL", (SCREEN_WIDTH / 2 - MeasureText("SAVE NEW LEVEL", 32) / 2), 250, 32, GOLD_COLOR);
    DrawText("Insert level name: ", (SCREEN_WIDTH / 2 - MeasureText("Insert level name: ", 24) / 2), 340, 24,
             LIGHTGRAY);

    // Desenhar cada caractere digitado para o nome do jogador
    DrawText(levelName, (innerBox.x + 28), (innerBox.y + 19), 38, RAYWHITE);

    // Desenhar undescore piscante
    if (blinkUnderscore && (nameSize != maxNameSize))
        DrawText("_", (innerBox.x + 31 + MeasureText(levelName, 38)), (innerBox.y + 19), 38, RAYWHITE);

    if (nameSize)
    {
        // Desenhar opção de confirmar desbloqueada
        DrawText("- CONFIRM -", (SCREEN_WIDTH / 2 - MeasureText("- CONFIRM -", 26) / 2), 515, 26, RAYWHITE);
    }
    else
    {
        // Desenhar opção de confirmar bloqueada
        DrawText("CONFIRM", (SCREEN_WIDTH / 2 - MeasureText("CONFIRM", 26) / 2), 515, 26, GRAY);
    }
}

void drawEditorHUD(level_t *level, editor_option_t selected, editor_option_t hovered)
{
    DrawTexture(level->textures[EditorHUD], 0, 0, WHITE);
    DrawText("Level Editor", 22, 11, EDITOR_FONT_SIZE, RAYWHITE);
    DrawText("SAVE", 1106, 11, EDITOR_FONT_SIZE, RAYWHITE);

    // Desenhar slot selecionado
    if (selected == Save || hovered == Save)
        DrawText("- SAVE -", 1088, 10, EDITOR_FONT_SIZE, RAYWHITE);
    if (hovered != Save)
        DrawTexture(level->textures[SlotHovered], 453 + (37 * hovered), 2, WHITE);
    if (selected != Save)
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
        DrawText("GAME OVER", (SCREEN_WIDTH / 2 - MeasureText("GAME OVER", ENDGAME_TITLE_FONT_SIZE) / 2), 233,
                 ENDGAME_TITLE_FONT_SIZE, RED);
        DrawText("NO LIVES LEFT", (SCREEN_WIDTH / 2 - MeasureText("NO LIVES LEFT", 26) / 2), 360, 26, GRAY);
    }
    else
    {
        DrawText("GAME OVER", (SCREEN_WIDTH / 2 - MeasureText("GAME OVER", ENDGAME_TITLE_FONT_SIZE) / 2), 233,
                 ENDGAME_TITLE_FONT_SIZE, DARKBLUE);
        DrawText("NO ORES LEFT", (SCREEN_WIDTH / 2 - MeasureText("NO ORES LEFT", 26) / 2), 360, 26, GRAY);
    }

    // Desenhar opções
    DrawText("RESTART GAME", (SCREEN_WIDTH / 2 - MeasureText("RESTART GAME", MENU_FONT_SIZE) / 2), 475, MENU_FONT_SIZE,
             RAYWHITE);
    DrawText("EXIT GAME", (SCREEN_WIDTH / 2 - MeasureText("EXIT GAME", MENU_FONT_SIZE) / 2), 541, MENU_FONT_SIZE,
             RAYWHITE);

    // Desenhar opção selecionada
    switch (selectedOption)
    {
    case ResetGame:
        DrawText("- RESTART GAME -", (SCREEN_WIDTH / 2 - MeasureText("- RESTART GAME -", MENU_FONT_SIZE) / 2), 474,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case ExitGame:
        DrawText("- EXIT GAME -", (SCREEN_WIDTH / 2 - MeasureText("- EXIT GAME -", MENU_FONT_SIZE) / 2), 540,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    }
}

void drawHighScoreTextBox(player_t *player, int nameSize, int maxNameSize, bool blinkUnderscore)
{
    Rectangle outerBox = {256, 205, 690, 390};
    Rectangle innerBox = {466, 361, 270, 101};

    // Desenhar caixas
    DrawRectangleRec(outerBox, DARKGRAY);
    DrawRectangleLinesEx(outerBox, 2, LIGHTGRAY);
    DrawRectangleRec(innerBox, BLACK);
    DrawRectangleLinesEx(innerBox, 2, RAYWHITE);

    // Desenhar texto
    DrawText("NEW HIGH SCORE!", (SCREEN_WIDTH / 2 - MeasureText("NEW HIGH SCORE!", 32) / 2), 238, 32, GOLD_COLOR);
    DrawText("Insert your name: ", (SCREEN_WIDTH / 2 - MeasureText("Insert your name: ", 24) / 2), 325, 24, LIGHTGRAY);

    // Desenhar cada caractere digitado para o nome do jogador
    if (nameSize == 3)
        DrawText(player->name, (SCREEN_WIDTH / 2 - MeasureText(player->name, 72) / 2), 379, 72, RAYWHITE);
    else
        DrawText(player->name, (SCREEN_WIDTH / 2 - MeasureText(TextFormat("%s_", player->name), 72) / 2), 379, 72,
                 RAYWHITE);

    if (nameSize < maxNameSize)
    {
        // Desenhar undescore piscante
        if (blinkUnderscore)
            DrawText("_",
                     ((SCREEN_WIDTH / 2) - (MeasureText("_", 72) / 2) + (MeasureText(player->name, 72) / 2) +
                      (nameSize ? 4 : 0)),
                     379, 72, RAYWHITE);

        // Desenhar opção de confirmar bloqueada
        DrawText("CONFIRM", (SCREEN_WIDTH / 2 - MeasureText("CONFIRM", 26) / 2), 527, 26, GRAY);
    }
    else
    {
        // Desenhar opção de confirmar desbloqueada
        DrawText("- CONFIRM -", (SCREEN_WIDTH / 2 - MeasureText("- CONFIRM -", 26) / 2), 527, 26, RAYWHITE);
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
    DrawText(TextFormat("Level %i", player->currentLevel), 1080, 8, HUD_FONT_SIZE, RAYWHITE);

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
    DrawText("START GAME", (SCREEN_WIDTH / 2 - MeasureText("START GAME", MENU_FONT_SIZE) / 2), 354, MENU_FONT_SIZE,
             RAYWHITE);
    DrawText("LEVEL EDITOR", (SCREEN_WIDTH / 2 - MeasureText("LEVEL EDITOR", MENU_FONT_SIZE) / 2), 416, MENU_FONT_SIZE,
             RAYWHITE);
    DrawText("CUSTOM LEVELS", (SCREEN_WIDTH / 2 - MeasureText("CUSTOM LEVELS", MENU_FONT_SIZE) / 2), 478,
             MENU_FONT_SIZE, RAYWHITE);
    DrawText("RANKING", (SCREEN_WIDTH / 2 - MeasureText("RANKING", MENU_FONT_SIZE) / 2), 540, MENU_FONT_SIZE, RAYWHITE);
    DrawText("EXIT", (SCREEN_WIDTH / 2 - MeasureText("EXIT", MENU_FONT_SIZE) / 2), 602, MENU_FONT_SIZE, RAYWHITE);

    // Desenhar opção selecionada
    switch (selectedOption)
    {
    case StartGame:
        DrawText("- START GAME -", (SCREEN_WIDTH / 2 - MeasureText("- START GAME -", MENU_FONT_SIZE) / 2), 353,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case LevelEditor:
        DrawText("- LEVEL EDITOR -", (SCREEN_WIDTH / 2 - MeasureText("- LEVEL EDITOR -", MENU_FONT_SIZE) / 2), 415,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case CustomLevels:
        DrawText("- CUSTOM LEVELS -", (SCREEN_WIDTH / 2 - MeasureText("- CUSTOM LEVELS -", MENU_FONT_SIZE) / 2), 477,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case Ranking:
        DrawText("- RANKING -", (SCREEN_WIDTH / 2 - MeasureText("- RANKING -", MENU_FONT_SIZE) / 2), 539,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case Exit:
        DrawText("- EXIT -", (SCREEN_WIDTH / 2 - MeasureText("- EXIT -", MENU_FONT_SIZE) / 2), 601, MENU_FONT_SIZE,
                 RAYWHITE);
        break;
    }
}

void drawRankingScreen(Texture2D background, ranking_t *players, int rankingSize, ranking_option_t selectedOption)
{
    // Desenhar fundo e título
    DrawTexture(background, 0, 0, WHITE);
    DrawText("RANKING", (SCREEN_WIDTH / 2 - MeasureText("RANKING", 32) / 2), 26, 32, RAYWHITE);

    // Desenhar subtítulos
    DrawText("PLACEMENT", (SCREEN_WIDTH / 7), MENU_FONT_SIZE + 242, MENU_FONT_SIZE, GOLD_COLOR);
    DrawText("SCORE", (SCREEN_WIDTH / 2 - MeasureText("SCORE", MENU_FONT_SIZE) / 2), MENU_FONT_SIZE + 242,
             MENU_FONT_SIZE, GOLD_COLOR);
    DrawText("NAME", (6 * SCREEN_WIDTH / 7 - MeasureText("NAME", MENU_FONT_SIZE)), MENU_FONT_SIZE + 242, MENU_FONT_SIZE,
             GOLD_COLOR);

    // Desenhar jogadores do ranking
    for (int i = 0; i < rankingSize; i++)
    {
        DrawText(TextFormat("%d °", players[i].position), (SCREEN_WIDTH / 7), (304 + (MENU_FONT_SIZE + 20) * i),
                 MENU_FONT_SIZE, RAYWHITE);
        DrawText(TextFormat("%d", players[i].score),
                 (SCREEN_WIDTH / 2 - MeasureText(TextFormat("%d", players[i].score), MENU_FONT_SIZE) / 2),
                 (304 + (MENU_FONT_SIZE + 20) * i), MENU_FONT_SIZE, RAYWHITE);
        DrawText(TextFormat("%s", players[i].name),
                 (6 * SCREEN_WIDTH / 7 - MeasureText(TextFormat("%s", players->name), MENU_FONT_SIZE)),
                 (304 + (MENU_FONT_SIZE + 20) * i), MENU_FONT_SIZE, RAYWHITE);
    }

    // Desenhar opção de sair
    DrawText("EXIT", (SCREEN_WIDTH / 2 - MeasureText("EXIT", MENU_FONT_SIZE) / 2), 750, MENU_FONT_SIZE, RAYWHITE);
    DrawText("- EXIT -", (SCREEN_WIDTH / 2 - MeasureText("- EXIT -", MENU_FONT_SIZE) / 2), 749, MENU_FONT_SIZE,
             RAYWHITE);
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
        DrawText(TextFormat("Level %i", player->currentLevel),
                 (SCREEN_WIDTH / 2 - MeasureText(TextFormat("Level %i", player->currentLevel), SPLASH_FONT_SIZE) / 2),
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
    DrawText("YOU WIN!", (SCREEN_WIDTH / 2 - MeasureText("YOU WIN!", ENDGAME_TITLE_FONT_SIZE) / 2), 273,
             ENDGAME_TITLE_FONT_SIZE, GREEN);

    // Desenhar opções
    DrawText("RESTART GAME", (SCREEN_WIDTH / 2 - MeasureText("RESTART GAME", MENU_FONT_SIZE) / 2), 436, MENU_FONT_SIZE,
             RAYWHITE);
    DrawText("EXIT GAME", (SCREEN_WIDTH / 2 - MeasureText("EXIT GAME", MENU_FONT_SIZE) / 2), 502, MENU_FONT_SIZE,
             RAYWHITE);

    // Desenhar opção selecionada
    switch (selectedOption)
    {
    case ResetGame:
        DrawText("- RESTART GAME -", (SCREEN_WIDTH / 2 - MeasureText("- RESTART GAME -", MENU_FONT_SIZE) / 2), 435,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    case ExitGame:
        DrawText("- EXIT GAME -", (SCREEN_WIDTH / 2 - MeasureText("- EXIT GAME -", MENU_FONT_SIZE) / 2), 501,
                 MENU_FONT_SIZE, RAYWHITE);
        break;
    }
}

#endif