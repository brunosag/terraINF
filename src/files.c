#ifndef FILES_C
#define FILES_C

#include "../include/files.h"

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

FILE *createRankingFile(int rankingSize)
{
    FILE *file = fopen("ranking/ranking.bin", "wb");

    // Verificar a abertura do arquivo
    if(file != NULL)
    {
        ranking_t rankingPlaceholder = {{0}, 0, 0};

        // Guardar no primeiro byte do arquivo a quantidade de jogadores no ranking
        fwrite(&rankingSize, sizeof(rankingSize), 1, file);

        for (int i = 0; i < rankingSize; i++)
        {
            rankingPlaceholder.position = i + 1;
            generateRandomName(rankingPlaceholder.name, MAX_PLAYER_NAME);
            fwrite(&rankingPlaceholder, sizeof(rankingPlaceholder), 1, file);
        }

        rewind(file);
    }

    return file;
}

#endif