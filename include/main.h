/*
 * File:         main.h
 * Authors:      Ana Rodrigues, Bruno Samuel e Pedro Lima
 * Date Created: 24/02/2023
 *
 * Description:  Arquivo principal do projeto com a implementação
 *               dos módulos de manipulação de arquivos, lógica do
 *               jogo e interface do(s) jogador(es).
 */

#ifndef MAIN_H
#define MAIN_H

#include "files.h"
#include "interface.h"
#include "logic.h"

bool highScore(level_t *level, player_t *player, endgame_option_t selected, Sound menuSelectionEffect);
bool saveCustomLevel(char *levelPath, level_t *level, player_t *player);
void startCustomLevelsMenu(void);
endgame_option_t gameOver(level_t *level, player_t *player, bool rankingOn);
endgame_option_t win(level_t *level, player_t *player);
menu_option_t startMenu(void);
ranking_option_t startRanking(void);
void getCustomLevelName(char *levelName, int *nameSize, int maxNameSize, level_t *level, editor_option_t selected,
                        editor_option_t hovered, Sound menuSelectionEffect, Music currentMusic);
void readCustomLevelsMenuData(custom_level_metadata_t *metadata, int *customLevelsAmount,
                              custom_levels_menu_t *menuData);
void startCustomGame(char *filename);
void startGame(void);
void startLevelEditor(void);

#endif