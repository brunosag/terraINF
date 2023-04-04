#ifndef AUXILIAR_C
#define AUXILIAR_C

#include "../include/auxiliar.h"

float fadeTimer(bool reset, float fadeInTime, float fadeOffTime, float fadeOutTime)
{
    static float alpha = FADE_OVER;
    static float fadeTimer = 0.0f;
    static float fadeSample = 0.0f;
    static fade_t fadeState = FadeReset;

    // Caso não queira-se reiniciar a contagem do timer
    if(!reset)
    {
        // Caso a função já tenha sido completa, reiniciar ciclo de fade
        if (fadeState == FadeReset)
        {
            alpha = 0.0f;
            fadeState = FadeIn;
        }

        // Cronometrar o tempo de fade in, fade off e fade out
        fadeTimer += GetFrameTime();
        if (fadeTimer >= TIMING_SAMPLE)
        {
            fadeTimer = 0.0f;
            fadeSample += TIMING_SAMPLE;
            switch (fadeState)
            {
            case FadeReset:
                alpha = 0.0f;
                fadeTimer = 0.0f;
                fadeSample = 0.0f;
                fadeState = FadeIn;
                break;
            case FadeIn:
                alpha = fadeSample / (fadeInTime);
                if(alpha >= 1.0f)
                {
                    fadeSample = 0.0f;
                    fadeState++;
                }
                break;
            case FadeOff:
                alpha = 1.0f;
                if(fadeSample >= fadeOffTime)
                {
                    fadeSample = 0.0f;
                    fadeState++;
                }
                break;
            case FadeOut:
                alpha = 1.0f - fadeSample / (fadeOutTime);
                if(alpha <= 0.0f)
                {
                    alpha = FADE_OVER;
                    fadeTimer = 0.0f;
                    fadeSample = 0.0f;
                    fadeState = FadeReset;
                }
                break;
            }
        }
    }
    else
    {
        alpha = 0.0f;
        fadeTimer = 0.0f;
        fadeSample = 0.0f;
        fadeState = FadeReset;
    }
    
    // Retornar a transparência atualizada a cada chamada
    return alpha;
}

void generateRandomName(char *name, int nameLength)
{
    srand(time(NULL));
    for (int i = 0; i < nameLength; i++)
        name[i] = ASCII_MIN + (rand() % (ASCII_MAX - ASCII_MIN + 1));
}

bool UninterruptTimer(bool reset, float time)
{
    static bool timeOver = false;
    static float timeCounter = 0.0f;
    static float timeSample = 0.0f;

    // Caso não queira-se reiniciar a contagem do timer
    if(!reset)
    {
        // Cronometrar o tempo passado a cada frame desenhado
        timeCounter += GetFrameTime();
        if (timeCounter >= TIMING_SAMPLE)
        {
            timeCounter = 0.0f;
            timeSample += TIMING_SAMPLE;

            // Após passarem 'time' segundos, a flag de tempo decorrido se torna verdadeira
            if(timeSample == time)
                timeOver = true;
        }
    }
    else
    {
        timeOver = false;
        timeCounter = 0.0f;
        timeSample = 0.0f;
    }
    
    // Retornar uma flag de tempo finalizado
    return timeOver;
}

#endif