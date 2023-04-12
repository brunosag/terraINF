/*
 * File:         interface.h
 * Authors:      Ana Rodrigues, Bruno Samuel e Pedro Lima
 * Date Created: 28/03/2023
 *
 * Description:  Módulo com as funções e definições
 *               para impressão de imagens e textos
 *               na tela do usuário.
 */

#ifndef INTERFACE_H
#define INTERFACE_H

#include "common.h"

#define EDITOR_FONT_SIZE 20
#define HUD_FONT_SIZE 26
#define HUD_ORE_SIZE 8
#define LAST_MINED_FADEIN_TIME 0.5f
#define LAST_MINED_FADEOFF_TIME 1.0f
#define LAST_MINED_FADEOUT_TIME 0.5f

#define SPLASH_FONT_SIZE 100
#define SPLASH_FADEIN_TIME 0.5f
#define SPLASH_FADEOFF_TIME 1.0f
#define SPLASH_FADEOUT_TIME 1.0f

#define ELEMENT_SIZE 40

Image createLevelMiniature(level_t *level, player_t *player);
void drawCustomLevelHUD(level_t *level, player_t *player);
void drawCustomLevelsMenu(Texture2D background, custom_levels_menu_t *menuData, int customLevelsAmount,
                          int selectedOption);
void drawCustomLevelsTextBox(const char *levelName, int nameSize, int maxNameSize, bool blinkUnderscore);
void drawEditorHUD(level_t *level, editor_option_t selected, editor_option_t hovered);
void drawEditorLevel(level_t *level);
void drawGameOverScreen(level_t *level, player_t *player, endgame_option_t selectedOption);
void drawHighScoreTextBox(player_t *player, int nameSize, int maxNameSize, bool blinkUnderscore);
void drawHUD(level_t *level, player_t *player);
void drawLevel(level_t *level, player_t *player);
void drawMenuScreen(Texture2D menuTexture, menu_option_t selectedOption);
void drawRankingScreen(Texture2D background, ranking_t *players, int rankingSize, ranking_option_t selectedOption);
void drawSplashScreen(player_t *player, Music *music);
void drawWinScreen(endgame_option_t selectedOption);

#endif