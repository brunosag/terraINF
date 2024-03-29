#ifndef FILES_C
#define FILES_C

#include "../include/files.h"

int createCustomLevelsMetadataFile(const char *metadataFile, int maxCustomLevelsAmount)
{
    int errorNumber = 0;

    // Ajeitar nome do diretório a ser criado
    int metadataFilePathLength = strlen(metadataFile);
    int lastSlash = metadataFilePathLength;
    char metadataDirectory[MAX_FILE_NAME + 1] = {0};
    for (int i = 0; i < metadataFilePathLength; i++)
    {
        metadataDirectory[i] = metadataFile[i];
        if (metadataDirectory[i] == '/')
            lastSlash = i;
    }

    // Preencher o resto do nome do diretório com caracteres nulos
    if (lastSlash != metadataFilePathLength)
    {
        for (int i = lastSlash; i < metadataFilePathLength; i++)
            metadataDirectory[i] = '\0';
// Tentar criar diretório para o arquivo (adaptado para a devida plataforma)
#ifdef _WIN32
        _mkdir(metadataDirectory);
#else
        mkdir(metadataDirectory, 0777);
#endif
    }

    // Verificar a abertura do arquivo
    FILE *file = fopen(metadataFile, "wb");
    if (file != NULL)
    {
        int customLevelsAmount = 0;

        // Guardar quantidade de níveis customizados nos primeiros bytes
        fwrite(&customLevelsAmount, sizeof(customLevelsAmount), 1, file);
        fwrite(&maxCustomLevelsAmount, sizeof(maxCustomLevelsAmount), 1, file);
    }
    else
        errorNumber = 1;

    fclose(file);
    return errorNumber;
}

int createRankingFile(const char *rankingFile, int rankingSize)
{
    int errorNumber = 0;

    // Ajeitar nome do diretório a ser criado
    int rankingFilePathLength = strlen(rankingFile);
    int lastSlash = rankingFilePathLength;
    char rankingDirectory[MAX_FILE_NAME + 1] = {0};
    for (int i = 0; i < rankingFilePathLength; i++)
    {
        rankingDirectory[i] = rankingFile[i];
        if (rankingDirectory[i] == '/')
            lastSlash = i;
    }

    // Preencher o resto do nome do diretório com caracteres nulos
    if (lastSlash != rankingFilePathLength)
    {
        for (int i = lastSlash; i < rankingFilePathLength; i++)
            rankingDirectory[i] = '\0';
// Tentar criar diretório para o arquivo (adaptado para a devida plataforma)
#ifdef _WIN32
        _mkdir(rankingDirectory);
#else
        mkdir(rankingDirectory, 0777);
#endif
    }

    // Verificar a abertura do arquivo
    FILE *file = fopen(rankingFile, "wb");
    if (file != NULL)
    {
        ranking_t rankingPlaceholder = {{0}, 0, 0};

        // Guardar no primeiro byte do arquivo a quantidade de jogadores no ranking
        if (fwrite(&rankingSize, sizeof(rankingSize), 1, file) != 1)
            errorNumber = 2;

        srand(time(NULL));
        for (int i = 0; i < rankingSize; i++)
        {
            rankingPlaceholder.position = i + 1;
            generateRandomName(rankingPlaceholder.name, MAX_PLAYER_NAME);
            if (fwrite(&rankingPlaceholder, sizeof(rankingPlaceholder), 1, file) != 1)
                errorNumber = 2;
        }
    }
    else
        errorNumber = 1;

    fclose(file);
    return errorNumber;
}

void loadEditorLevel(level_t *level)
{
    // Carregar sprites
    level->textures[Background] = LoadTexture("resources/sprites/background.png");
    level->textures[CaesiumOre] = LoadTexture("resources/sprites/caesium_ore.png");
    level->textures[Dirt] = LoadTexture("resources/sprites/dirt.png");
    level->textures[Edge] = LoadTexture("resources/sprites/edge.png");
    level->textures[EditorHUD] = LoadTexture("resources/sprites/editor_hud.png");
    level->textures[GoldOre] = LoadTexture("resources/sprites/gold_ore.png");
    level->textures[Ore] = LoadTexture("resources/sprites/ore.png");
    level->textures[Player] = LoadTexture("resources/sprites/player.png");
    level->textures[SilverOre] = LoadTexture("resources/sprites/silver_ore.png");
    level->textures[SlotHovered] = LoadTexture("resources/sprites/slot_hovered.png");
    level->textures[SlotSelected] = LoadTexture("resources/sprites/slot_selected.png");
    level->textures[TitaniumOre] = LoadTexture("resources/sprites/titanium_ore.png");
    level->textures[UraniumOre] = LoadTexture("resources/sprites/uranium_ore.png");

    // Ajustar nome do arquivo
    char filename[MAX_FILE_NAME + 1] = {'\0'};
    snprintf(filename, sizeof(filename), "levels/editor.txt");

    // Abrir arquivo texto do nível
    FILE *levelFile = fopen((const char *)filename, "r");
    if (levelFile != NULL)
    {
        // Ler caracteres do arquivo e transferir para matriz
        for (int i = 0; i < LVL_HEIGHT; i++)
        {
            for (int j = 0; j < LVL_WIDTH; j++)
                level->elements[i][j] = fgetc(levelFile);

            // Pular caractere de nova linha
            fseek(levelFile, 2, SEEK_CUR);
        }

        // Fechar arquivo texto do nível
        fclose(levelFile);
    }
    else
        printf("Could not read the level matrix file.");
}

void loadLevel(level_t *level, player_t *player, char filename[])
{
    // Reiniciar status do jogador e nivel
    player->lastMined = (ore_t){0};
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

    snprintf(level->ores[Caesium].name, MAX_ORE_NAME, "Caesium");
    snprintf(level->ores[Gold].name, MAX_ORE_NAME, "Gold");
    snprintf(level->ores[Silver].name, MAX_ORE_NAME, "Silver");
    snprintf(level->ores[Titanium].name, MAX_ORE_NAME, "Titanium");
    snprintf(level->ores[Uranium].name, MAX_ORE_NAME, "Uranium");

    // Carregar sprites
    level->textures[Background] = LoadTexture("resources/sprites/background.png");
    level->textures[Dirt] = LoadTexture("resources/sprites/dirt.png");
    level->textures[Edge] = LoadTexture("resources/sprites/edge.png");
    level->textures[HUD] = LoadTexture("resources/sprites/hud.png");
    level->textures[Ladder] = LoadTexture("resources/sprites/ladder.png");
    level->textures[Ore] = LoadTexture("resources/sprites/ore.png");
    level->textures[Player] = LoadTexture("resources/sprites/player.png");
    level->textures[PlayerLadder] = LoadTexture("resources/sprites/player_ladder.png");
    level->textures[PlayerLadderPickaxe] = LoadTexture("resources/sprites/player_ladder_pickaxe.png");
    level->textures[PlayerPickaxe] = LoadTexture("resources/sprites/player_pickaxe.png");
    level->ores[Caesium].texture = LoadTexture("resources/sprites/caesium.png");
    level->ores[Gold].texture = LoadTexture("resources/sprites/gold.png");
    level->ores[Silver].texture = LoadTexture("resources/sprites/silver.png");
    level->ores[Titanium].texture = LoadTexture("resources/sprites/titanium.png");
    level->ores[Uranium].texture = LoadTexture("resources/sprites/uranium.png");

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
                switch (level->elements[i][j])
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
        printf("Could not read the level matrix file.");
}

int readCustomLevelsMetadataFile(const char *metadataFile, custom_level_metadata_t *metadata, int *customLevelsAmount,
                                 int *maxCustomLevelsAmount)
{
    int entriesRead = 0;

    // Verificar abertura de arquivo
    FILE *file = fopen(metadataFile, "rb");
    if (file != NULL)
        if (fread(customLevelsAmount, sizeof(*customLevelsAmount), 1, file) == 1)
            // Pular número máximo de níveis customizados
            if (fread(maxCustomLevelsAmount, sizeof(*maxCustomLevelsAmount), 1, file) == 1)
                for (int i = 0; i < *customLevelsAmount; i++)
                    // Verificar leitura de cada item do arquivo e contabilizar
                    if (fread(&metadata[i], sizeof(metadata[i]), 1, file) == 1)
                        entriesRead++;
    fclose(file);
    return entriesRead;
}

int readRankingFile(const char *rankingFile, ranking_t *players)
{
    int entriesRead = 0;

    // Verificar abertura de arquivo
    FILE *file = fopen(rankingFile, "rb");
    if (file != NULL)
    {
        int rankingSize = 0;
        if (fread(&rankingSize, sizeof(rankingSize), 1, file) == 1)
            // Verificar se sabe o tamanho de entradas no arquivo
            for (int i = 0; i < rankingSize; i++)
                // Verificar leitura de cada item do arquivo e contabilizar
                if (fread(&players[i], sizeof(players[i]), 1, file) == 1)
                    entriesRead++;
    }

    fclose(file);
    return entriesRead;
}

int saveCustomLevelFile(char *levelFile, level_t *level)
{
    int errorNumber = 0;

    // Verificar se arquivo foi criado
    FILE *file = fopen(levelFile, "w");
    if (file != NULL)
    {
        // Ler caracteres da matriz e transferir para arquivo
        for (int i = 0; i < LVL_HEIGHT; i++)
        {
            for (int j = 0; j < LVL_WIDTH; j++)
                fprintf(file, "%c", level->elements[i][j]);
            fprintf(file, "\n");
        }
    }
    else
        errorNumber = 1;

    fclose(file);
    return errorNumber;
}

int writeCustomLevelsMetadata(const char *metadataFile, custom_level_metadata_t *metadata, int customLevelsAmount)
{
    int errorNumber = 0;

    // Verificar abertura de arquivo para leitura/escrita
    FILE *file = fopen(metadataFile, "rb+");
    if (file != NULL)
    {
        // Atualizar número de níveis customizados
        fwrite(&customLevelsAmount, sizeof(customLevelsAmount), 1, file);
        // Pular o número máximo de níveis customizados
        fseek(file, sizeof(customLevelsAmount), SEEK_CUR);

        // Verificar a efetiva escrita dos novos metadados
        if (fwrite(metadata, sizeof(*metadata), customLevelsAmount, file) != customLevelsAmount)
            errorNumber = 2;
    }
    else
        errorNumber = 1;

    fclose(file);
    return errorNumber;
}

int writeRankingPosition(const char *rankingFile, ranking_t *player)
{
    int errorNumber = 0;

    // Verificar abertura de arquivo para leitura/escrita
    FILE *file = fopen(rankingFile, "rb+");
    if (file != NULL)
    {
        // Pular para a posição de escrita do jogador a ser alterado
        fseek(file, sizeof(int) + (player->position - 1) * sizeof(*player), SEEK_SET);

        // Verificar a efetiva escrita do novo jogador no ranking
        if (fwrite(player, sizeof(*player), 1, file) != 1)
            errorNumber = 2;
    }
    else
        errorNumber = 1;

    fclose(file);
    return errorNumber;
}

#endif