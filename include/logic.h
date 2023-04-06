/*
 * File:         logic.h
 * Authors:      Ana Rodrigues, Bruno Samuel e Pedro Lima
 * Date Created: 28/03/2023
 *
 * Description:  Arquivo com todas as implementações dos
 *               sistemas do jogo (mineração, vidas, escadas
 *               movimentação etc.).
 */

#ifndef LOGIC_H
#define LOGIC_H

#include <string.h>
#include "common.h"

int getFallSize(level_t *level, int x, int y);
bool mine(level_t *level, player_t *player, int direction);
void moveHorizontal(level_t *level, player_t *player, int offset);
void moveVertical(level_t *level, player_t *player, int offset);
void placeLadder(level_t *level, player_t *player);
void updateEnergy(player_t *player, int offset);
void updateRankingPositions(player_t *player, ranking_t *players, int rankingSize, int firstAlteredPosition);
void updateScore(player_t *player, int offset);

#endif