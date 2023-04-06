#ifndef FILES_C
#define FILES_C

#include "../include/files.h"

int createRankingFile(const char* rankingFile, int rankingSize)
{
    int errorNumber = 0;

    // Verificar a abertura do arquivo
    FILE *file = fopen(rankingFile, "wb");
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
    }
    else
        errorNumber = 1;

    fclose(file);

    return errorNumber;
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
    level->textures[Background] = LoadTexture("resources/sprites/background.png");
    level->textures[Dirt] = LoadTexture("resources/sprites/dirt.png");
    level->textures[Edge] = LoadTexture("resources/sprites/edge.png");
    level->textures[HUD] = LoadTexture("resources/sprites/hud.png");
    level->textures[Ladder] = LoadTexture("resources/sprites/ladder.png");
    level->textures[Ore] = LoadTexture("resources/sprites/ore.png");
    level->textures[PlayerLadderPickaxe] = LoadTexture("resources/sprites/player_ladder_pickaxe.png");
    level->textures[PlayerLadder] = LoadTexture("resources/sprites/player_ladder.png");
    level->textures[PlayerPickaxe] = LoadTexture("resources/sprites/player_pickaxe.png");
    level->textures[Player] = LoadTexture("resources/sprites/player.png");
    level->ores[Caesium].texture = LoadTexture("resources/sprites/caesium_ore.png");
    level->ores[Gold].texture = LoadTexture("resources/sprites/gold_ore.png");
    level->ores[Silver].texture = LoadTexture("resources/sprites/silver_ore.png");
    level->ores[Titanium].texture = LoadTexture("resources/sprites/titanium_ore.png");
    level->ores[Uranium].texture = LoadTexture("resources/sprites/uranium_ore.png");

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

int readRankingFile(const char *rankingFile, ranking_t *players)
{
    int entriesRead = 0;

    // Verificar abertura de arquivo
    FILE *file = fopen(rankingFile, "rb");
    if(file != NULL)
    {
        int rankingSize = 0;
        if (fread(&rankingSize, sizeof(rankingSize), 1, file) == 1)
            // Verificar se sabe o tamanho de entradas no arquivo
            for(int i = 0; i < rankingSize; i++)
                // Verificar leitura de cada item do arquivo e contabilizar
                if(fread(&players[i], sizeof(players[i]), 1, file) == 1)
                    entriesRead++;
    }

    fclose(file);
    return entriesRead;
}

int writeRankingPosition(const char *rankingFile, ranking_t *player)
{
    int errorNumber = 0;

    // Verificar abertura de arquivo para leitura/escrita
    FILE *file = fopen(rankingFile, "rb+");
    if(file != NULL)
    {
        // Pular para a posição de escrita do jogador a ser alterado
        fseek(file, sizeof(int) + (player->position - 1) * sizeof(*player), SEEK_SET);
        
        // Verificar a efetiva escrita do novo jogador no ranking
        if(fwrite(player, sizeof(*player), 1, file) != 1)
            errorNumber = 2;
    }
    else
        errorNumber = 1;

    fclose(file);
    return errorNumber;
}

#endif