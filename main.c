#include "raylib.h"
#include <stdio.h>

int main()
{
    // -------------------------------------------------------------------- //
    // Initialize                                                           //
    // -------------------------------------------------------------------- //
    const int screenWidth = 1200;
    const int screenHeight = 800;
    const int levelWidth = 30;
    const int levelHeight = 20;

    InitWindow(screenWidth, screenHeight, "TerraINF");

    while (!WindowShouldClose())
    {
        // -------------------------------------------------------------------- //
        // Update                                                               //
        // -------------------------------------------------------------------- //

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

        // -------------------------------------------------------------------- //
        // Draw                                                                 //
        // -------------------------------------------------------------------- //
        BeginDrawing();

        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
