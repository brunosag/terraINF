#include "raylib.h"
#include <stdio.h>
#define MAX_NAME 3

typedef struct player
{
    Vector2 location;
    char name[MAX_NAME + 1];
    int score;
    int lives;
    int energy;
    int ladders;
    char lastMined;
} t_player;

int main()
{
    // ---------------------------------------------------------------------------------------- //
    // Initialize                                                                               //
    // ---------------------------------------------------------------------------------------- //
    const int screenWidth = 1200;
    const int screenHeight = 800;
    const int levelWidth = 30;
    const int levelHeight = 20;
    const int elementSize = 40;
    const int HUDFontSize = 26;

    // Incializar janela do jogo
    InitWindow(screenWidth, screenHeight, "TerraINF");

    // Carregar sprites
    Texture2D backgroundTexture = LoadTexture("sprites/background.png");
    Texture2D borderTexture = LoadTexture("sprites/border.png");
    Texture2D dirtTexture = LoadTexture("sprites/dirt.png");
    Texture2D HUDTexture = LoadTexture("sprites/hud.png");
    Texture2D oreTexture = LoadTexture("sprites/ore.png");
    Texture2D playerTexture = LoadTexture("sprites/player.png");

    // Inicalizar jogador
    t_player player = {{2, 11}, {0}, 0, 3, 100, 20, 0};

    // Definir nível inicial
    int currentLevel = 1;

    while (!WindowShouldClose())
    {
        // ------------------------------------------------------------------------------------ //
        // Update                                                                               //
        // ------------------------------------------------------------------------------------ //

        // Abrir arquivo texto do nível
        FILE *levelFile = fopen("nivel1.txt", "r");
        if (levelFile == NULL)
        {
            printf("Erro ao ler o arquivo da matriz do nível.");
        }

        // Ler caracteres do arquivo e transferir para matriz
        char level[levelHeight][levelWidth];
        for (int i = 0; i < levelHeight; i++)
        {
            for (int j = 0; j < levelWidth; j++)
            {
                fread(&level[i][j], sizeof(char), 1, levelFile);
            }

            // Pular caractere de nova linha
            fseek(levelFile, 2, SEEK_CUR);
        }

        // Fechar arquivo texto do nível
        fclose(levelFile);

        // ------------------------------------------------------------------------------------ //
        // Draw                                                                                 //
        // ------------------------------------------------------------------------------------ //
        BeginDrawing();

        // Desenhar texturas com base na matriz
        for (int i = 0; i < levelHeight; i++)
        {
            for (int j = 0; j < levelWidth; j++)
            {
                // Verificar elemento atual na matriz
                Texture2D currentTexture;
                switch (level[i][j])
                {
                case 'G':
                case 'T':
                case 'S':
                case 'U':
                case 'C':
                    currentTexture = oreTexture;
                    break;
                case 'X':
                    currentTexture = dirtTexture;
                    break;
                case ' ':
                    currentTexture = backgroundTexture;
                    break;
                case 'B':
                    currentTexture = borderTexture;
                    break;
                case 'J':
                    currentTexture = playerTexture;
                    break;
                }

                // Desenhar elemento
                DrawTexture(currentTexture, (j * elementSize), (i * elementSize), WHITE);
            }
        }

        // Desenhar HUD
        DrawTexture(HUDTexture, 0, 0, WHITE);
        DrawText(TextFormat("%i", player.lives), 66, 8, HUDFontSize, RAYWHITE);
        DrawText(TextFormat("%i", player.energy), 177, 8, HUDFontSize, RAYWHITE);
        DrawText(TextFormat("%i", player.ladders), 311, 8, HUDFontSize, RAYWHITE);
        DrawText(TextFormat("%i", player.score),
                 (956 - MeasureText(TextFormat("%i", player.score), 28)), 8, 28, RAYWHITE);
        DrawText("/1000", 962, 14, 20, DARKGRAY);
        DrawText(TextFormat("Nível %i", currentLevel), 1080, 8, HUDFontSize, RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
