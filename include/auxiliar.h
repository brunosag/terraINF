/*
 * File:         auxiliar.h
 * Authors:      Ana Rodrigues, Bruno Samuel e Pedro Lima
 * Date Created: 28/03/2023
 *
 * Description:  Arquivo com as funções auxiliares para os
 *               demais módulos do projeto.
 */

#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stdlib.h>
#include <time.h>
#include "common.h"

#define TIMING_SAMPLE 0.05f
#define FADE_OVER -1.0f
#define ALPHA_DISABLE 100.0f
#define ASCII_MIN 65
#define ASCII_MAX 90

typedef enum fade
{
    FadeReset,
    FadeIn,
    FadeOff,
    FadeOut
} fade_t;

float fadeTimer(bool reset, float fadeInTime, float fadeOffTime, float fadeOutTime);
void generateRandomName(char *name, int nameLength);
bool uninterruptTimer(bool reset, float time);

#endif