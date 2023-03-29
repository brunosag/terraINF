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

#define MENU_FONT_SIZE 22

#define MAX_RANKING_SIZE 5

typedef enum menu_option
{
    StartGame = 1,
    Ranking,
    LevelEditor,
    Exit
} menu_option_t;

typedef enum gameover_option
{
    ResetGame = 1,
    ExitGame
} gameover_option_t;

menu_option_t startMenu(void);
void startGame(player_t *player);
gameover_option_t gameOver(level_t *level, player_t *player);

#endif