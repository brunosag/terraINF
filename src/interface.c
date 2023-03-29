#ifndef INTERFACE_C
#define INTERFACE_C

#include "../include/interface.h"

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

#endif