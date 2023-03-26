/*
 * File:         main.c
 * Authors:      Ana Rodrigues, Bruno Samuel e Pedro Lima
 * Date Created: 24/02/2023
 *
 * Description:  Arquivo principal do projeto com a implementação
 *               das funções de manipulação de arquivos, lógica do
 *               jogo e interface do(s) jogador(es).
 */

#include "raylib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define GAME_FRAMERATE 60
#define TIMING_SAMPLE 0.05f
#define FADE_OVER -1.0f
#define ALPHA_DISABLE 100.0f

#define MAX_PLAYER_NAME 3
#define MAX_RANKING_SIZE 5
#define MAX_LVL_NAME 30
#define LVL_WIDTH 30
#define LVL_HEIGHT 20
#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define ELEMENT_SIZE 40
#define HUD_FONT_SIZE 26
#define HUD_ORE_SIZE 8
#define MENU_FONT_SIZE 22
#define SPLASH_FONT_SIZE 100
#define SPLASH_FADEIN_TIME 0.5f
#define SPLASH_FADEOFF_TIME 1.0f
#define SPLASH_FADEOUT_TIME 1.0f
#define LAST_MINED_FADEIN_TIME 0.5f
#define LAST_MINED_FADEOFF_TIME 1.0f
#define LAST_MINED_FADEOUT_TIME 0.5f
#define ALPHA_MIN 65
#define ALPHA_MAX 90

#define MAX_ORE_NAME 20
#define ORE_COUNT 5
#define CAESIUM_COLOR (Color){226, 156, 100, 255}
#define GOLD_COLOR (Color){226, 209, 126, 255}
#define SILVER_COLOR (Color){190, 190, 190, 255}
#define TITANIUM_COLOR (Color){192, 219, 196, 255}
#define URANIUM_COLOR (Color){119, 193, 111, 255}

#define ELEMENT_COUNT 10
#define CHAR_PLAYER 'J'
#define CHAR_GOLD 'G'
#define CHAR_TITANIUM 'T'
#define CHAR_SILVER 'S'
#define CHAR_URANIUM 'U'
#define CHAR_CAESIUM 'C'
#define CHAR_EMPTY ' '
#define CHAR_EDGE 'B'
#define CHAR_DIRT 'X'
#define CHAR_LADDER 'H'
#define CHAR_PLAYER_LADDER 'E'

typedef enum fade
{
    FadeReset,
    FadeIn,
    FadeOff,
    FadeOut
} fade_t;

typedef enum menu_option
{
    StartGame = 1,
    Ranking,
    LevelEditor,
    Exit
} menu_option_t;

typedef enum gameover_option
{
    ResetGame = 1,
    ExitGame
} gameover_option_t;

typedef enum element_number
{
    Background,
    Dirt,
    Edge,
    HUD,
    Ladder,
    Ore,
    PlayerLadderPickaxe,
    PlayerLadder,
    PlayerPickaxe,
    Player
} element_number_t;

typedef enum ore_number
{
    Caesium,
    Gold,
    Silver,
    Titanium,
    Uranium
} ore_number_t;

typedef struct position
{
    int x;
    int y;
} position_t;

typedef struct ore
{
    char name[MAX_ORE_NAME + 1];
    Color nameColor;
    Texture2D texture;
} ore_t;

typedef struct player
{
    ore_t lastMined;
    position_t position;
    char name[MAX_PLAYER_NAME + 1];
    int score;
    int health;
    int energy;
    int ladders;
    int currentLevel;
    bool miningMode;
} player_t;

typedef struct level
{
    char elements[LVL_HEIGHT][LVL_WIDTH];
    Texture2D textures[ELEMENT_COUNT];
    ore_t ores[ORE_COUNT];
    int oreCount;
} level_t;

typedef struct ranking
{
    char name[MAX_PLAYER_NAME + 1];
    int score;
} ranking_t;

float fadeTimer(bool reset, float fadeInTime, float fadeOffTime, float fadeOutTime);
void loadLevel(level_t *level, player_t *player);
void drawLevel(level_t *level, player_t *player, float alpha);
void updateEnergy(player_t *player, int offset);
int getFallSize(level_t *level, int x, int y);
void moveHorizontal(level_t *level, player_t *player, int offset);
void moveVertical(level_t *level, player_t *player, int offset);
void mine(level_t *level, player_t *player, int direction);
void placeLadder(level_t *level, player_t *player);
void drawHUD(player_t *player, float alpha);
menu_option_t startMenu(void);
void startGame(player_t *player);
void generateRandomName(char *name, int nameLength);
FILE *createRankingFile(int rankingSize);
gameover_option_t gameOver(level_t *level, player_t *player);

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
            }
        }

        // Verificar troca de nível
        if (currentLevel != player->currentLevel)
        {
            currentLevel = player->currentLevel;
            loadLevel(&level, player);
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

float fadeTimer(bool reset, float fadeInTime, float fadeOffTime, float fadeOutTime)
{
    static float alpha = FADE_OVER;
    static float fadeTimer = 0.0f;
    static float fadeSample = 0.0f;
    static fade_t fadeState = FadeReset;

    // Caso não queira-se reiniciar a contagem do timer
    if(!reset)
    {
        // Caso a função já tenha sido completa, reiniciar ciclo de fade
        if (fadeState == FadeReset)
        {
            alpha = 0.0f;
            fadeState = FadeIn;
        }

        // Cronometrar o tempo de fade in, fade off e fade out
        fadeTimer += GetFrameTime();
        if (fadeTimer >= TIMING_SAMPLE)
        {
            fadeTimer = 0.0f;
            fadeSample += TIMING_SAMPLE;
            switch (fadeState)
            {
            case FadeReset:
                alpha = 0.0f;
                fadeTimer = 0.0f;
                fadeSample = 0.0f;
                fadeState = FadeIn;
                break;
            case FadeIn:
                alpha = fadeSample / (fadeInTime);
                if(alpha >= 1.0f)
                {
                    fadeSample = 0.0f;
                    fadeState++;
                }
                break;
            case FadeOff:
                alpha = 1.0f;
                if(fadeSample >= fadeOffTime)
                {
                    fadeSample = 0.0f;
                    fadeState++;
                }
                break;
            case FadeOut:
                alpha = 1.0f - fadeSample / (fadeOutTime);
                if(alpha <= 0.0f)
                {
                    alpha = FADE_OVER;
                    fadeTimer = 0.0f;
                    fadeSample = 0.0f;
                    fadeState = FadeReset;
                }
                break;
            }
        }
    }
    else
    {
        alpha = 0.0f;
        fadeTimer = 0.0f;
        fadeSample = 0.0f;
        fadeState = FadeReset;
    }
    
    // Retornar a transparência atualizada a cada chamada
    return alpha;
}

void loadLevel(level_t *level, player_t *player)
{
    // Reiniciar status do jogador e nivel
    player->lastMined = (ore_t) {0};
    player->score = 0;
    player->energy = 100;
    player->ladders = 20;
    player->miningMode = false;
    level->oreCount = 0;
    
    // Carregar nome e cor dos minérios da HUD
    level->ores[Caesium].nameColor = CAESIUM_COLOR;
    level->ores[Gold].nameColor = GOLD_COLOR;
    level->ores[Silver].nameColor = SILVER_COLOR;
    level->ores[Titanium].nameColor = TITANIUM_COLOR;
    level->ores[Uranium].nameColor = URANIUM_COLOR;

    snprintf(level->ores[Caesium].name, MAX_ORE_NAME, "Césio");
    snprintf(level->ores[Gold].name, MAX_ORE_NAME, "Ouro");
    snprintf(level->ores[Silver].name, MAX_ORE_NAME, "Prata");
    snprintf(level->ores[Titanium].name, MAX_ORE_NAME, "Titânio");
    snprintf(level->ores[Uranium].name, MAX_ORE_NAME, "Urânio");
    
    // Carregar sprites
    level->textures[Background] = LoadTexture("sprites/background.png");
    level->textures[Dirt] = LoadTexture("sprites/dirt.png");
    level->textures[Edge] = LoadTexture("sprites/edge.png");
    level->textures[HUD] = LoadTexture("sprites/hud.png");
    level->textures[Ladder] = LoadTexture("sprites/ladder.png");
    level->textures[Ore] = LoadTexture("sprites/ore.png");
    level->textures[PlayerLadderPickaxe] = LoadTexture("sprites/player_ladder_pickaxe.png");
    level->textures[PlayerLadder] = LoadTexture("sprites/player_ladder.png");
    level->textures[PlayerPickaxe] = LoadTexture("sprites/player_pickaxe.png");
    level->textures[Player] = LoadTexture("sprites/player.png");
    level->ores[Caesium].texture = LoadTexture("sprites/caesium_ore.png");
    level->ores[Gold].texture = LoadTexture("sprites/gold_ore.png");
    level->ores[Silver].texture = LoadTexture("sprites/silver_ore.png");
    level->ores[Titanium].texture = LoadTexture("sprites/titanium_ore.png");
    level->ores[Uranium].texture = LoadTexture("sprites/uranium_ore.png");

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

    // Ajustar nome do arquivo
    char filename[MAX_LVL_NAME + 1] = {'\0'};
    snprintf(filename, sizeof(filename) - 1, "levels/nivel%d.txt", player->currentLevel);

    // Abrir arquivo texto do nível
    FILE *levelFile = fopen((const char *)filename, "r");
    if (levelFile != NULL)
    {
        // Ler caracteres do arquivo e transferir para matriz
        for (int i = 0; i < LVL_HEIGHT; i++)
        {
            for (int j = 0; j < LVL_WIDTH; j++)
            {
                level->elements[i][j] = fgetc(levelFile);
                switch(level->elements[i][j])
                {
                case CHAR_PLAYER:
                    player->position.x = j;
                    player->position.y = i;
                    break;
                case CHAR_CAESIUM:
                case CHAR_GOLD:
                case CHAR_SILVER:
                case CHAR_TITANIUM:
                case CHAR_URANIUM:
                    level->oreCount++;
                    break;
                }
            }

            // Pular caractere de nova linha
            fseek(levelFile, 2, SEEK_CUR);
        }

        // Fechar arquivo texto do nível
        fclose(levelFile);
    }
    else
        printf("Erro ao ler o arquivo da matriz do nivel.");
}

void updateEnergy(player_t *player, int offset)
{
    player->energy += offset;
    if (player->energy <= 20)
    {
        // Retirar vida e restaurar energia
        player->health--;
        player->energy = 100;
    }
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

void updateScore(player_t *player, int offset)
{
    player->score += offset;

    // Verificar se jogador passou de fase
    if (player->score >= (int)(1000 * pow(2, player->currentLevel - 1)))
        player->currentLevel++;
}

int getFallSize(level_t *level, int x, int y)
{
    int fallSize = 0;

    // Aumentar tamanho da queda a cada bloco vazio abaixo do destino
    while (level->elements[y + 1][x] == CHAR_EMPTY)
    {
        y++;
        fallSize++;
    }
    return fallSize;
}

void moveHorizontal(level_t *level, player_t *player, int offset)
{
    // Verificar se bloco destino livre
    char *target = &level->elements[player->position.y][player->position.x + offset];
    if (*target == CHAR_EMPTY || *target == CHAR_PLAYER || *target == CHAR_LADDER ||
        *target == CHAR_PLAYER_LADDER)
    {
        // Verificar tamanho da queda causada pelo movimento
        int fallSize = getFallSize(level, (player->position.x + offset), player->position.y);

        // Alterar bloco atual na matriz
        if (level->elements[player->position.y][player->position.x] == CHAR_PLAYER)
            level->elements[player->position.y][player->position.x] = CHAR_EMPTY;
        else
            level->elements[player->position.y][player->position.x] = CHAR_LADDER;

        // Alterar bloco alvo na matriz
        if (level->elements[player->position.y + fallSize][player->position.x + offset] == CHAR_EMPTY)
            level->elements[player->position.y + fallSize][player->position.x + offset] = CHAR_PLAYER;
        else
            level->elements[player->position.y + fallSize][player->position.x + offset] = CHAR_PLAYER_LADDER;

        // Alterar valores posição do jogador
        player->position.x += offset;
        player->position.y += fallSize;

        // Retirar vida se queda maior que 3 blocos
        if (fallSize > 3)
            player->health--;
    }
}

void moveVertical(level_t *level, player_t *player, int offset)
{
    // Verificar se bloco atual e destino possuem escadas
    if (level->elements[player->position.y][player->position.x] == CHAR_PLAYER_LADDER &&
        level->elements[player->position.y + offset][player->position.x] == CHAR_LADDER)
    {
        // Alterar matriz
        level->elements[player->position.y][player->position.x] = CHAR_LADDER;
        level->elements[player->position.y + offset][player->position.x] = CHAR_PLAYER_LADDER;

        // Alterar valores posição do jogador
        player->position.y += offset;
    }
}

void mine(level_t *level, player_t *player, int direction)
{
    // Verificar bloco alvo
    char *block = NULL;
    switch (direction)
    {
    case (KEY_W):
    case (KEY_UP):
        block = &level->elements[player->position.y - 1][player->position.x];
        break;
    case (KEY_S):
    case (KEY_DOWN):
        block = &level->elements[player->position.y + 1][player->position.x];
        break;
    case (KEY_D):
    case (KEY_RIGHT):
        block = &level->elements[player->position.y][player->position.x + 1];
        break;
    case (KEY_A):
    case (KEY_LEFT):
        block = &level->elements[player->position.y][player->position.x - 1];
        break;
    }

    // Verificar se bloco alvo é minerável
    if (*block == CHAR_DIRT || *block == CHAR_TITANIUM || *block == CHAR_GOLD ||
        *block == CHAR_SILVER || *block == CHAR_CAESIUM || *block == CHAR_URANIUM)
    {
        // Atualizar energia, score e último minério do jogador
        switch (*block)
        {
        case CHAR_DIRT:
            updateEnergy(player, -3);
            break;
        case CHAR_TITANIUM:
            updateEnergy(player, 30);
            updateScore(player, 150);
            player->lastMined = level->ores[Titanium];
            level->oreCount--;
            break;
        case CHAR_GOLD:
            updateEnergy(player, 20);
            updateScore(player, 100);
            player->lastMined = level->ores[Gold];
            level->oreCount--;
            break;
        case CHAR_SILVER:
            updateEnergy(player, 10);
            updateScore(player, 50);
            player->lastMined = level->ores[Silver];
            level->oreCount--;
            break;
        case CHAR_CAESIUM:
            updateEnergy(player, -20);
            player->lastMined = level->ores[Caesium];
            level->oreCount--;
            break;
        case CHAR_URANIUM:
            updateEnergy(player, -30);
            player->lastMined = level->ores[Uranium];
            level->oreCount--;
            break;
        }

        // Remover bloco
        *block = CHAR_EMPTY;

        // Lidar com queda se houver
        moveHorizontal(level, player, 0);
    }
}

void placeLadder(level_t *level, player_t *player)
{
    if (player->ladders > 0)
    {
        int distance = 0;

        // Verificar escadas já posicionadas e definir alvo
        while (level->elements[player->position.y - distance][player->position.x] == CHAR_PLAYER_LADDER ||
               level->elements[player->position.y - distance][player->position.x] == CHAR_LADDER)
        {
            distance++;
        }
        char *target = &level->elements[player->position.y - distance][player->position.x];

        // Verificar se alvo é jogador
        if (*target == CHAR_EMPTY)
        {
            *target = CHAR_LADDER;
            player->ladders--;
        }

        // Verificar se alvo é vazio
        if (*target == CHAR_PLAYER)
        {
            *target = CHAR_PLAYER_LADDER;
            player->ladders--;
        }
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

void generateRandomName(char *name, int nameLength)
{
    srand(time(NULL));
    for (int i = 0; i < nameLength; i++)
        name[i] = ALPHA_MIN + (rand() % (ALPHA_MAX - ALPHA_MIN + 1));
}

FILE *createRankingFile(int rankingSize)
{
    FILE *file = fopen("ranking/ranking.bin", "wb");

    // Verificar a abertura do arquivo
    if(file != NULL)
    {
        ranking_t rankingPlaceholder = {{0}, 0};

        // Guardar no primeiro byte do arquivo a quantidade de jogadores no ranking
        fwrite(&rankingSize, sizeof(rankingSize), 1, file);

        for (int i = 0; i < rankingSize; i++)
        {
            generateRandomName(rankingPlaceholder.name, MAX_PLAYER_NAME);
            fwrite(rankingPlaceholder.name, sizeof(rankingPlaceholder.name), 1, file);
            fwrite(&rankingPlaceholder.score, sizeof(rankingPlaceholder.score), 1, file);
        }

        rewind(file);
    }

    return file;
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
