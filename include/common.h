/*
 * File:         common.h
 * Authors:      Ana Rodrigues, Bruno Samuel e Pedro Lima
 * Date Created: 28/03/2023
 *
 * Description:  Arquivo com definições comuns a todo o 
 *               projeto.
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "raylib.h"

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define GAME_FRAMERATE 60

#define MAX_PLAYER_NAME 3
#define MAX_LVL_NAME 30
#define LVL_WIDTH 30
#define LVL_HEIGHT 20
#define LAST_LVL 3

#define ELEMENT_COUNT 13
#define CHAR_PLAYER 'J'
#define CHAR_GOLD 'G'
#define CHAR_TITANIUM 'T'
#define CHAR_SILVER 'S'
#define CHAR_URANIUM 'U'
#define CHAR_CAESIUM 'C'
#define CHAR_EMPTY ' '
#define CHAR_EDGE 'B'
#define CHAR_DIRT 'X'
#define CHAR_LADDER 'H'
#define CHAR_PLAYER_LADDER 'E'

#define MAX_ORE_NAME 20
#define ORE_COUNT 5
#define CAESIUM_COLOR (Color){226, 156, 100, 255}
#define GOLD_COLOR (Color){226, 209, 126, 255}
#define SILVER_COLOR (Color){190, 190, 190, 255}
#define TITANIUM_COLOR (Color){192, 219, 196, 255}
#define URANIUM_COLOR (Color){119, 193, 111, 255}

#define MENU_FONT_SIZE 22
#define GAMEOVER_TITLE_FONT_SIZE 100
#define GAMEOVER_NAME_DELAY 1.0f // em segundos

#define MAX_RANKING_SIZE 5

typedef enum element_number
{
    Background,
    Dirt,
    Edge,
    EditorHUD,
    HUD,
    Ladder,
    Ore,
    Player,
    PlayerLadder,
    PlayerLadderPickaxe,
    PlayerPickaxe,
    SlotSelected,
    SlotUnselected,
} element_number_t;

typedef enum ore_number
{
    Caesium,
    Gold,
    Silver,
    Titanium,
    Uranium
} ore_number_t;

typedef enum menu_option
{
    StartGame = 1,
    Ranking,
    LevelEditor,
    Exit
} menu_option_t;

typedef enum ranking_option
{
    ExitRanking = 1
} ranking_option_t;

typedef enum gameover_option
{
    ResetGame = 1,
    ExitGame
} gameover_option_t;

typedef enum editor_option
{
    PlayerSlot,
    OreSlot,
    DirtSlot,
    BackgroundSlot,
    Save
} editor_option_t;

typedef struct position
{
    int x;
    int y;
} position_t;

typedef struct ore
{
    char name[MAX_ORE_NAME + 1];
    Color nameColor;
    Texture2D texture;
} ore_t;

typedef struct player
{
    ore_t lastMined;
    position_t position;
    char name[MAX_PLAYER_NAME + 1];
    int score;
    int health;
    int energy;
    int ladders;
    int currentLevel;
    bool miningMode;
} player_t;

typedef struct level
{
    char elements[LVL_HEIGHT][LVL_WIDTH];
    Texture2D textures[ELEMENT_COUNT];
    ore_t ores[ORE_COUNT];
    int oreCount;
} level_t;

typedef struct ranking
{
    char name[MAX_PLAYER_NAME + 1];
    int position;
    int score;
} ranking_t;

#endif