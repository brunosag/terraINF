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

gameover_option_t gameOver(level_t *level, player_t *player);
void startGame(player_t *player);
menu_option_t startMenu(void);
ranking_option_t startRanking(void);

#endif