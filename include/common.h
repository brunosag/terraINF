/*
 * File:         common.h
 * Authors:      Ana Rodrigues, Bruno Samuel e Pedro Lima
 * Date Created: 28/03/2023
 *
 * Description:  Arquivo com definições e funções comuns a todo
 *               o projeto.
 */

#ifndef COMMON_H
#define COMMON_H

#include "raylib.h"
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define GAME_FRAMERATE 60

#define TIMING_SAMPLE 0.05f
#define FADE_OVER -1.0f
#define ALPHA_DISABLE 100.0f
#define ASCII_MIN 65
#define ASCII_MAX 90
#define BLINK_TIME 0.5f

#define MAX_FILE_NAME 100
#define FOLDER_NONE 0

#define MAX_PLAYER_NAME 3
#define MAX_LVL_NAME 30
#define LVL_WIDTH 30
#define LVL_HEIGHT 20
#define LAST_LVL 3

#define ELEMENT_COUNT 18
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
#define CAESIUM_COLOR                                                                                                  \
    (Color)                                                                                                            \
    {                                                                                                                  \
        226, 156, 100, 255                                                                                             \
    }
#define GOLD_COLOR                                                                                                     \
    (Color)                                                                                                            \
    {                                                                                                                  \
        226, 209, 126, 255                                                                                             \
    }
#define SILVER_COLOR                                                                                                   \
    (Color)                                                                                                            \
    {                                                                                                                  \
        190, 190, 190, 255                                                                                             \
    }
#define TITANIUM_COLOR                                                                                                 \
    (Color)                                                                                                            \
    {                                                                                                                  \
        192, 219, 196, 255                                                                                             \
    }
#define URANIUM_COLOR                                                                                                  \
    (Color)                                                                                                            \
    {                                                                                                                  \
        119, 193, 111, 255                                                                                             \
    }

#define MENU_FONT_SIZE 22
#define ENDGAME_TITLE_FONT_SIZE 100
#define GAMEOVER_NAME_DELAY 1.0f // em segundos

#define MAX_RANKING_SIZE 5

#define MAX_CUSTOM_LEVELS_AMOUNT 3
#define MAX_CUSTOM_LEVEL_NAME 20
#define MAX_CUSTOM_LEVEL_DATE 40

#define EXIT_CUSTOM_LEVELS_MENU -1

typedef enum fade
{
    FadeReset,
    FadeIn,
    FadeOff,
    FadeOut
} fade_t;

typedef enum element_number
{
    Background,
    CaesiumOre,
    Dirt,
    Edge,
    EditorHUD,
    GoldOre,
    HUD,
    Ladder,
    Ore,
    Player,
    PlayerLadder,
    PlayerLadderPickaxe,
    PlayerPickaxe,
    SilverOre,
    SlotSelected,
    SlotUnselected,
    TitaniumOre,
    UraniumOre
} element_number_t;

typedef enum ore_number
{
    Caesium,
    Gold,
    Silver,
    Titanium,
    Uranium
} ore_number_t;

typedef enum action_effects
{
    None,
    DirtMined,
    OreMined,
    PlayerMoved,
    PlayerFell,
    PlayerDamaged
} action_effects_t;

typedef enum menu_option
{
    StartGame = 1,
    LevelEditor,
    CustomLevels,
    Ranking,
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
} endgame_option_t;

typedef enum editor_option
{
    PlayerSlot,
    BackgroundSlot,
    DirtSlot,
    SilverSlot,
    GoldSlot,
    TitaniumSlot,
    CaesiumSlot,
    UraniumSlot,
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
    int maxScore;
} level_t;

typedef struct custom_level_metadata
{
    char miniatureFile[MAX_FILE_NAME + 1];
    char name[MAX_FILE_NAME + 1];
    time_t dateCreated;
} custom_level_metadata_t;

typedef struct custom_levels_menu
{
    char dateCreated[MAX_CUSTOM_LEVEL_DATE + 1];
    char name[MAX_CUSTOM_LEVEL_NAME + 1];
    Texture2D miniature;
} custom_levels_menu_t;

typedef struct ranking
{
    char name[MAX_PLAYER_NAME + 1];
    int position;
    int score;
} ranking_t;

bool uninterruptTimer(bool reset, float time);
float fadeTimer(bool reset, float fadeInTime, float fadeOffTime, float fadeOutTime);
int compareFileNames(const char *fileName1, const char *fileName2);
int findLowestIntervalValue(int *values, int valuesAmount, int firstValue, int lastValue);
void generateRandomName(char *name, int nameLength);
void removeFileExtension(char *fileName);
void updateDuplicateFileName(char *duplicateFileName, int lastDuplicateNumber);

#endif