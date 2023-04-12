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

#include "common.h"

action_effects_t mine(level_t *level, player_t *player, int direction);
action_effects_t moveHorizontal(level_t *level, player_t *player, int offset);
bool isOrePlaced(level_t *level);
bool moveVertical(level_t *level, player_t *player, int offset);
bool placeLadder(level_t *level, player_t *player);
bool updateEnergy(player_t *player, int offset);
int customLevelMaxScore(level_t *level);
int getFallSize(level_t *level, int x, int y);
int updateCustomLevelsMetadata(custom_level_metadata_t *oldMetadata, custom_level_metadata_t *metadata,
                               custom_level_metadata_t *metadataStored, int *customLevelsStored,
                               int maxCustomLevelsAmount);
void createLevelFilePath(const char *folder, const char *fileName, int nameSize, char *filePath);
void getFileName(const char *filePath, char *fileName);
void placeBlock(level_t *level, player_t *player, position_t mousePosition, editor_option_t selected);
void updateRankingPositions(int totalScore, player_t *player, ranking_t *players, int rankingSize,
                            int firstAlteredPosition);
void updateScore(level_t *level, player_t *player, int offset);

#endif