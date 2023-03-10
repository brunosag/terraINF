#include "raylib.h"
#include <stdio.h>

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

    // Incializar janela do jogo
    InitWindow(screenWidth, screenHeight, "TerraINF");

    // Carregar sprites
    Texture2D backgroundTexture = LoadTexture("sprites/background.png");
    Texture2D borderTexture = LoadTexture("sprites/border.png");
    Texture2D dirtTexture = LoadTexture("sprites/dirt.png");
    Texture2D oreTexture = LoadTexture("sprites/ore.png");
    Texture2D playerTexture = LoadTexture("sprites/player.png");

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

            // Descartar caractere de nova linha
            char charDisposal;
            fread(&charDisposal, sizeof(char), 1, levelFile);
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

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
