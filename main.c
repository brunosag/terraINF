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
#include <stdbool.h>
#include <stdio.h>
#define MAX_PLAYER_NAME 3
#define MAX_ORE_NAME 20
#define MAX_LVL_NAME 30
#define LVL_WIDTH 30
#define LVL_HEIGHT 20
#define HUD_FONT_SIZE 26
#define HUD_ORE_SIZE 8

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

typedef struct position
{
    int x;
    int y;
} position_t;

typedef struct ore
{
    char oreName[MAX_ORE_NAME + 1];
    Color oreNameColor;
    Texture2D oreTexture;
} ore_t;
typedef struct player
{
    position_t position;
    char name[MAX_PLAYER_NAME + 1];
    int score;
    int health;
    int energy;
    int ladders;
    ore_t lastMined;
    bool miningMode;
} player_t;

void loadLevel(char (*level)[LVL_WIDTH], int currentLevel, player_t *player);
void reduceHealth(player_t *player);
void updateEnergy(player_t *player, int offset);
void updateScore(player_t *player, int offset);
void updateLastMined(player_t *player, ore_t lastMined);
int getFallSize(char (*level)[LVL_WIDTH], int x, int y);
void moveHorizontal(char (*level)[LVL_WIDTH], player_t *player, int offset);
void moveVertical(char (*level)[LVL_WIDTH], player_t *player, int offset);
void mine(char (*level)[LVL_WIDTH], player_t *player, int direction);
void placeLadder(char (*level)[LVL_WIDTH], player_t *player);
void drawHUD(player_t *player, int currentLevel);

// Quando migrar para vários arquivos, fazer um arquivo separado para "tabelas globais"
ore_t caesiumOre = {"Cesio", BROWN, (Texture2D) {0, 0, 0, 0, 0}};
ore_t goldOre = {"Ouro", GOLD, (Texture2D) {0, 0, 0, 0, 0}};
ore_t silverOre = {"Prata", LIGHTGRAY, (Texture2D) {0, 0, 0, 0, 0}};
ore_t stoneOre = {"Nenhum Item Minerado", RAYWHITE, (Texture2D) {0, 0, 0, 0, 0}};
ore_t titaniumOre = {"Titanio", DARKGRAY, (Texture2D) {0, 0, 0, 0, 0}};
ore_t uraniumOre = {"Uranio", LIME, (Texture2D) {0, 0, 0, 0, 0}};

int main()
{
    // ---------------------------------------------------------------------------------------- //
    // Initialize                                                                               //
    // ---------------------------------------------------------------------------------------- //
    const int screenWidth = 1200;
    const int screenHeight = 800;
    const int elementSize = 40;

    int i, j;

    // Inicializar janela do jogo
    InitWindow(screenWidth, screenHeight, "TerraINF");

    // Carregar sprites
    Texture2D backgroundTexture = LoadTexture("sprites/background.png");
    Texture2D borderTexture = LoadTexture("sprites/border.png");
    Texture2D dirtTexture = LoadTexture("sprites/dirt.png");
    Texture2D HUDTexture = LoadTexture("sprites/hud.png");
    Texture2D ladderTexture = LoadTexture("sprites/ladder.png");
    Texture2D oreTexture = LoadTexture("sprites/ore.png");
    caesiumOre.oreTexture = LoadTexture("sprites/caesium_ore.png");
    goldOre.oreTexture = LoadTexture("sprites/gold_ore.png");
    silverOre.oreTexture = LoadTexture("sprites/silver_ore.png");
    stoneOre.oreTexture = LoadTexture("sprites/stone_ore.png");
    titaniumOre.oreTexture = LoadTexture("sprites/titanium_ore.png");
    uraniumOre.oreTexture = LoadTexture("sprites/uranium_ore.png");
    Texture2D playerLadderPickaxeTexture = LoadTexture("sprites/player_ladder_pickaxe.png");
    Texture2D playerLadderTexture = LoadTexture("sprites/player_ladder.png");
    Texture2D playerPickaxeTexture = LoadTexture("sprites/player_pickaxe.png");
    Texture2D playerTexture = LoadTexture("sprites/player.png");

    // Inicializar jogador
    player_t player = {{1, 2}, {'\0'}, 0, 3, 100, 20, stoneOre, false};

    // Carregar nível inicial
    int currentLevel = 1;
    char level[LVL_HEIGHT][LVL_WIDTH];
    loadLevel(level, currentLevel, &player);

    KeyboardKey direction = KEY_S;

    while (!WindowShouldClose())
    {
        // ------------------------------------------------------------------------------------ //
        // Update                                                                               //
        // ------------------------------------------------------------------------------------ //

        // Verificar movimentação
        if (IsKeyPressed(KEY_D))
        {
            moveHorizontal(level, &player, 1);
            direction = KEY_D;
        }
        if (IsKeyPressed(KEY_A))
        {
            moveHorizontal(level, &player, -1);
            direction = KEY_A;
        }
        if (IsKeyPressed(KEY_W))
        {
            moveVertical(level, &player, -1);
            direction = KEY_W;
        }
        if (IsKeyPressed(KEY_S))
        {
            moveVertical(level, &player, 1);
            direction = KEY_S;
        }

        // Verificar modo mineração
        if (IsKeyPressed(KEY_ONE))
            player.miningMode = !player.miningMode;

        // Verificar mineração
        if (IsKeyPressed(KEY_SPACE) && player.miningMode)
            mine(level, &player, direction);

        // Verificar posicionamento de escada
        if (IsKeyPressed(KEY_LEFT_SHIFT))
            placeLadder(level, &player);

        // ------------------------------------------------------------------------------------ //
        // Draw                                                                                 //
        // ------------------------------------------------------------------------------------ //
        BeginDrawing();

        // Desenhar texturas com base na matriz
        for (i = 0; i < LVL_HEIGHT; i++)
        {
            for (j = 0; j < LVL_WIDTH; j++)
            {
                // Verificar elemento atual na matriz
                Texture2D currentTexture;
                switch (level[i][j])
                {
                case CHAR_GOLD:
                case CHAR_TITANIUM:
                case CHAR_SILVER:
                case CHAR_URANIUM:
                case CHAR_CAESIUM:
                    currentTexture = oreTexture;
                    break;
                case CHAR_DIRT:
                    currentTexture = dirtTexture;
                    break;
                case CHAR_EMPTY:
                    currentTexture = backgroundTexture;
                    break;
                case CHAR_EDGE:
                    currentTexture = borderTexture;
                    break;
                case CHAR_LADDER:
                    currentTexture = ladderTexture;
                    break;
                case CHAR_PLAYER_LADDER:
                    if (player.miningMode)
                        currentTexture = playerLadderPickaxeTexture;
                    else
                        currentTexture = playerLadderTexture;
                    break;
                case CHAR_PLAYER:
                    if (player.miningMode)
                        currentTexture = playerPickaxeTexture;
                    else
                        currentTexture = playerTexture;
                    break;
                }

                // Desenhar elemento
                DrawTexture(currentTexture, (j * elementSize), (i * elementSize), WHITE);
            }
        }

        // Desenhar HUD
        DrawTexture(HUDTexture, 0, 0, WHITE);
        drawHUD(&player, currentLevel);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}

void loadLevel(char (*level)[LVL_WIDTH], int currentLevel, player_t *player)
{
    // Ajustar nome do arquivo
    char filename[MAX_LVL_NAME + 1] = {'\0'};
    snprintf(filename, sizeof(filename), "nivel%d.txt", currentLevel);

    // Abrir arquivo texto do nível
    FILE *levelFile = fopen((const char *) filename, "r");
    if (levelFile != NULL)
    {
        // Ler caracteres do arquivo e transferir para matriz
        for (int i = 0; i < LVL_HEIGHT; i++)
        {
            for (int j = 0; j < LVL_WIDTH; j++)
            {
                fread(&level[i][j], sizeof(char), 1, levelFile);
                if(level[i][j] == CHAR_PLAYER)
                {
                    player->position.x = j;
                    player->position.y = i;
                }
            }

            // Pular caractere de nova linha
            fseek(levelFile, 2, SEEK_CUR);
        }

        // Fechar arquivo texto do nível
        fclose(levelFile);
    }
    else
        printf("Erro ao ler o arquivo da matriz do nível.");
}

void reduceHealth(player_t *player)
{
    player->health--;
}

void updateEnergy(player_t *player, int offset)
{
    player->energy += offset;
    if (player->energy <= 20)
    {
        // Retirar vida e restaurar energia
        reduceHealth(player);
        player->energy = 100;
    }
}

void updateScore(player_t *player, int offset)
{
    player->score += offset;
}

void updateLastMined(player_t *player, ore_t lastMined)
{
    player->lastMined = lastMined;
}

int getFallSize(char (*level)[LVL_WIDTH], int x, int y)
{
    int fallSize = 0;

    // Aumentar tamanho da queda a cada bloco vazio abaixo do destino
    while (level[y + 1][x] == CHAR_EMPTY)
    {
        y++;
        fallSize++;
    }
    return fallSize;
}

void moveHorizontal(char (*level)[LVL_WIDTH], player_t *player, int offset)
{
    // Verificar se bloco destino livre
    char *target = &level[player->position.y][player->position.x + offset];
    if (*target == CHAR_EMPTY || *target == CHAR_PLAYER || *target == CHAR_LADDER || *target == CHAR_PLAYER_LADDER)
    {
        // Verificar tamanho da queda causada pelo movimento
        int fallSize;
        fallSize = getFallSize(level, (player->position.x + offset), player->position.y);

        // Alterar bloco atual na matriz
        if (level[player->position.y][player->position.x] == CHAR_PLAYER)
            level[player->position.y][player->position.x] = CHAR_EMPTY;
        else
            level[player->position.y][player->position.x] = CHAR_LADDER;

        // Alterar bloco alvo na matriz
        if (level[player->position.y + fallSize][player->position.x + offset] == CHAR_EMPTY)
            level[player->position.y + fallSize][player->position.x + offset] = CHAR_PLAYER;
        else
            level[player->position.y + fallSize][player->position.x + offset] = CHAR_PLAYER_LADDER;

        // Alterar valores posição do jogador
        player->position.x += offset;
        player->position.y += fallSize;

        // Retirar vida se queda maior que 3 blocos
        if (fallSize > 3)
            reduceHealth(player);
    }
}

void moveVertical(char (*level)[LVL_WIDTH], player_t *player, int offset)
{
    // Verificar se bloco atual e destino possuem escadas
    if (level[player->position.y][player->position.x] == CHAR_PLAYER_LADDER &&
        level[player->position.y + offset][player->position.x] == CHAR_LADDER)
    {
        // Alterar matriz
        level[player->position.y][player->position.x] = CHAR_LADDER;
        level[player->position.y + offset][player->position.x] = CHAR_PLAYER_LADDER;

        // Alterar valores posição do jogador
        player->position.y += offset;
    }
}

void mine(char (*level)[LVL_WIDTH], player_t *player, int direction)
{
    // Verificar bloco alvo
    char *block = NULL;
    switch (direction)
    {
    case (KEY_W):
        block = &level[player->position.y - 1][player->position.x];
        break;
    case (KEY_S):
        block = &level[player->position.y + 1][player->position.x];
        break;
    case (KEY_D):
        block = &level[player->position.y][player->position.x + 1];
        break;
    case (KEY_A):
        block = &level[player->position.y][player->position.x - 1];
        break;
    }

    // Verificar se bloco alvo é minerável
    if (*block == CHAR_DIRT || *block == CHAR_TITANIUM || *block == CHAR_GOLD || *block == CHAR_SILVER || *block == CHAR_CAESIUM ||
        *block == CHAR_URANIUM)
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
            updateLastMined(player, (ore_t) titaniumOre);
            break;
        case CHAR_GOLD:
            updateEnergy(player, 20);
            updateScore(player, 100);
            updateLastMined(player, (ore_t) goldOre);
            break;
        case CHAR_SILVER:
            updateEnergy(player, 10);
            updateScore(player, 50);
            updateLastMined(player, (ore_t) silverOre);
            break;
        case CHAR_CAESIUM:
            updateEnergy(player, -20);
            updateLastMined(player, (ore_t) caesiumOre);
            break;
        case CHAR_URANIUM:
            updateEnergy(player, -30);
            updateLastMined(player, (ore_t) uraniumOre);
            break;
        }

        // Remover bloco
        *block = CHAR_EMPTY;

        // Lidar com queda se houver
        moveHorizontal(level, player, 0);
    }
}

void placeLadder(char (*level)[LVL_WIDTH], player_t *player)
{
    if (player->ladders > 0)
    {
        int distance = 0;

        // Verificar escadas já posicionadas e definir alvo
        while (level[player->position.y - distance][player->position.x] == CHAR_PLAYER_LADDER ||
               level[player->position.y - distance][player->position.x] == CHAR_LADDER)
        {
            distance++;
        }
        char *target = &level[player->position.y - distance][player->position.x];

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

void drawHUD(player_t *player, int currentLevel)
{
    DrawText(TextFormat("%i", player->health), 66, 8, HUD_FONT_SIZE, RAYWHITE);
    DrawText(TextFormat("%i", player->energy), 177, 8, HUD_FONT_SIZE, RAYWHITE);
    DrawText(TextFormat("%i", player->ladders), 311, 8, HUD_FONT_SIZE, RAYWHITE);
    DrawTexture(player->lastMined.oreTexture, 400, 12, WHITE);
    DrawText(player->lastMined.oreName, 440, 8, HUD_FONT_SIZE, player->lastMined.oreNameColor);
    DrawText(TextFormat("%i", player->score),
             (956 - MeasureText(TextFormat("%i", player->score), 28)), 8, 28, RAYWHITE);
    DrawText("/1000", 962, 14, 20, DARKGRAY);
    DrawText(TextFormat("Nível %i", currentLevel), 1080, 8, HUD_FONT_SIZE, RAYWHITE);
}
