#ifndef COMMON_C
#define COMMON_C

#include "../include/common.h"

int compareFileNames(const char* fileName1, const char* fileName2)
{
    // Zero se não há duplicatas
    int duplicateNumber = 0;

    // Encontrar tamanho de cada nome de arquivo
    int fileName1Length = strlen(fileName1);
    int fileName2Length = strlen(fileName2);
    int *comparisonLength = NULL;
    const char *largestFileName = NULL;
    if (fileName1Length > fileName2Length)
    {
        largestFileName = fileName1;
        comparisonLength = &fileName2Length;
    }
    else
    {
        largestFileName = fileName2;
        comparisonLength = &fileName1Length;
    }

    // Realizar comparação caractere a caractere para obter diferença entre cada elemento
    int i = 0;
    int nameComparison;
    do
    {
        nameComparison = abs(fileName1[i] - fileName2[i]);
        i++;
    }
    while (!nameComparison && i <= *comparisonLength);
    
    // Se os nomes tiverem o mesmo tamanho, realizar comparação caractere a caractere
    if (fileName1Length == fileName2Length && !nameComparison)
        duplicateNumber = 1;
    // Caso contrário, se a diferença entre os nomes ocorrer na extensão de um dos arquivos
    else if (nameComparison == ('_' - '.'))
        duplicateNumber = atoi(&largestFileName[i]);

    return duplicateNumber;
}

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

// Procurar menor valor que não está na lista de valores e que está dentro de um intervalo
int findLowestIntervalValue(int *values, int valuesAmount, int firstValue, int lastValue)
{
    int lowestValue = firstValue;

    // Excluir valores do intervalo que já estão na lista
    int interval = lastValue - firstValue + 1;
    int actualValuesAmount = 0;
    int listOfActualValues[interval];
    bool valueInList;
    for (int i = 0; i < interval; i++)
    {
        valueInList = false;
        for (int j = 0; j < valuesAmount; j++)
            if (values[j] == firstValue + i)
                valueInList = true;

        if (!valueInList)
        {
            listOfActualValues[actualValuesAmount] = firstValue + i;
            actualValuesAmount++;
        }
    }

    // Descobrir o menor valor que está na lista
    if (actualValuesAmount > 1)
        for (int i = 0; i < (actualValuesAmount - 1); i++)
        {
            if (listOfActualValues[i] < listOfActualValues[i + 1])
                lowestValue = listOfActualValues[i];
            else
                lowestValue = listOfActualValues[i + 1];
        }
    else
        lowestValue = listOfActualValues[0];

    return lowestValue;
}

void generateRandomName(char *name, int nameLength)
{
    srand(time(NULL));
    for (int i = 0; i < nameLength; i++)
        name[i] = ASCII_MIN + (rand() % (ASCII_MAX - ASCII_MIN + 1));
}

void removeFileExtension(char *fileName)
{
    // Procurar início da extensão do arquivo
    int fileNameSize = strlen(fileName);
    int fileExtStart = fileNameSize;
    for (int i = 0; i < fileNameSize; i++)
        if (fileName[i] == '.')
            fileExtStart = i;

    // Guardar apenas nome do arquivo, sem extensão
    for (int i = fileExtStart; i < fileNameSize; i++)
        fileName[i] = '\0';
}

bool uninterruptTimer(bool reset, float time)
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
            if(timeSample >= time)
            {
                timeOver = true;
                timeSample = time;
            }
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

void updateDuplicateFileName(char *duplicateFileName, int duplicateNumber)
{
    // Remover extensão do arquivo
    char fileName[MAX_FILE_NAME + 1] = {0};
    strncpy(fileName, duplicateFileName, sizeof(fileName));
    removeFileExtension(fileName);

    // Remontar nome do arquivo corretamente
    snprintf(duplicateFileName, MAX_FILE_NAME, "%s_%d.txt", fileName, duplicateNumber);
}

#endif