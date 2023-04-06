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

#include "auxiliar.h"
#include <string.h>

int createRankingFile(const char *rankingFile, int rankingSize);
int readRankingFile(const char *rankingFile, ranking_t *players);
int writeRankingPosition(const char *rankingFile, ranking_t *player);
void loadEditorLevel(level_t *level);
void loadLevel(level_t *level, player_t *player);

#endif