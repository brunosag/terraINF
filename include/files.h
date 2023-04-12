/*
 * File:         files.h
 * Authors:      Ana Rodrigues, Bruno Samuel e Pedro Lima
 * Date Created: 28/03/2023
 *
 * Description:  Módulo com as funções, variáveis e definições
 *               para manipulação de arquivos do projeto.
 */

#ifndef FILES_H
#define FILES_H

#include "common.h"
#ifdef _WIN32
    #include <direct.h>
#else
    #include <sys/stat.h>
    #include <sys/types.h>
#endif

int createCustomLevelsMetadataFile(const char *metadataFile, int maxCustomLevelsAmount);
int createRankingFile(const char *rankingFile, int rankingSize);
void loadEditorLevel(level_t *level);
void loadLevel(level_t *level, player_t *player);
int readCustomLevelsMetadataFile(const char *metadataFile, custom_level_metadata_t *metadata, int *customLevelsAmount, int *maxCustomLevelsAmount);
int readRankingFile(const char *rankingFile, ranking_t *players);
int saveCustomLevelFile(char *levelFile, level_t *level);
int writeCustomLevelsMetadata(const char *metadataFile, custom_level_metadata_t *metadata, int customLevelsAmount);
int writeRankingPosition(const char *rankingFile, ranking_t *player);

#endif