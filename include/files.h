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

#include <string.h>
#include "auxiliar.h"

void loadLevel(level_t *level, player_t *player);
FILE *createRankingFile(int rankingSize);

#endif