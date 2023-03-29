#ifndef LOGIC_C
#define LOGIC_C

#include "../include/logic.h"

int getFallSize(level_t *level, int x, int y)
{
    int fallSize = 0;

    // Aumentar tamanho da queda a cada bloco vazio abaixo do destino
    while (level->elements[y + 1][x] == CHAR_EMPTY)
    {
        y++;
        fallSize++;
    }
    return fallSize;
}

void mine(level_t *level, player_t *player, int direction)
{
    // Verificar bloco alvo
    char *block = NULL;
    switch (direction)
    {
    case (KEY_W):
    case (KEY_UP):
        block = &level->elements[player->position.y - 1][player->position.x];
        break;
    case (KEY_S):
    case (KEY_DOWN):
        block = &level->elements[player->position.y + 1][player->position.x];
        break;
    case (KEY_D):
    case (KEY_RIGHT):
        block = &level->elements[player->position.y][player->position.x + 1];
        break;
    case (KEY_A):
    case (KEY_LEFT):
        block = &level->elements[player->position.y][player->position.x - 1];
        break;
    }

    // Verificar se bloco alvo é minerável
    if (*block == CHAR_DIRT || *block == CHAR_TITANIUM || *block == CHAR_GOLD ||
        *block == CHAR_SILVER || *block == CHAR_CAESIUM || *block == CHAR_URANIUM)
    {
        // Atualizar energia, score e último minério do jogador
        switch (*block)
        {
        case CHAR_DIRT:
            updateEnergy(player, -3);
            break;
        case CHAR_TITANIUM:
            updateEnergy(player, 30);
            updateScore(player, 150);
            player->lastMined = level->ores[Titanium];
            level->oreCount--;
            break;
        case CHAR_GOLD:
            updateEnergy(player, 20);
            updateScore(player, 100);
            player->lastMined = level->ores[Gold];
            level->oreCount--;
            break;
        case CHAR_SILVER:
            updateEnergy(player, 10);
            updateScore(player, 50);
            player->lastMined = level->ores[Silver];
            level->oreCount--;
            break;
        case CHAR_CAESIUM:
            updateEnergy(player, -20);
            player->lastMined = level->ores[Caesium];
            level->oreCount--;
            break;
        case CHAR_URANIUM:
            updateEnergy(player, -30);
            player->lastMined = level->ores[Uranium];
            level->oreCount--;
            break;
        }

        // Remover bloco
        *block = CHAR_EMPTY;

        // Lidar com queda se houver
        moveHorizontal(level, player, 0);
    }
}

void moveHorizontal(level_t *level, player_t *player, int offset)
{
    // Verificar se bloco destino livre
    char *target = &level->elements[player->position.y][player->position.x + offset];
    if (*target == CHAR_EMPTY || *target == CHAR_PLAYER || *target == CHAR_LADDER ||
        *target == CHAR_PLAYER_LADDER)
    {
        // Verificar tamanho da queda causada pelo movimento
        int fallSize = getFallSize(level, (player->position.x + offset), player->position.y);

        // Alterar bloco atual na matriz
        if (level->elements[player->position.y][player->position.x] == CHAR_PLAYER)
            level->elements[player->position.y][player->position.x] = CHAR_EMPTY;
        else
            level->elements[player->position.y][player->position.x] = CHAR_LADDER;

        // Alterar bloco alvo na matriz
        if (level->elements[player->position.y + fallSize][player->position.x + offset] == CHAR_EMPTY)
            level->elements[player->position.y + fallSize][player->position.x + offset] = CHAR_PLAYER;
        else
            level->elements[player->position.y + fallSize][player->position.x + offset] = CHAR_PLAYER_LADDER;

        // Alterar valores posição do jogador
        player->position.x += offset;
        player->position.y += fallSize;

        // Retirar vida se queda maior que 3 blocos
        if (fallSize > 3)
            player->health--;
    }
}

void moveVertical(level_t *level, player_t *player, int offset)
{
    // Verificar se bloco atual e destino possuem escadas
    if (level->elements[player->position.y][player->position.x] == CHAR_PLAYER_LADDER &&
        level->elements[player->position.y + offset][player->position.x] == CHAR_LADDER)
    {
        // Alterar matriz
        level->elements[player->position.y][player->position.x] = CHAR_LADDER;
        level->elements[player->position.y + offset][player->position.x] = CHAR_PLAYER_LADDER;

        // Alterar valores posição do jogador
        player->position.y += offset;
    }
}

void placeLadder(level_t *level, player_t *player)
{
    if (player->ladders > 0)
    {
        int distance = 0;

        // Verificar escadas já posicionadas e definir alvo
        while (level->elements[player->position.y - distance][player->position.x] == CHAR_PLAYER_LADDER ||
               level->elements[player->position.y - distance][player->position.x] == CHAR_LADDER)
        {
            distance++;
        }
        char *target = &level->elements[player->position.y - distance][player->position.x];

        // Verificar se alvo é jogador
        if (*target == CHAR_EMPTY)
        {
            *target = CHAR_LADDER;
            player->ladders--;
        }

        // Verificar se alvo é vazio
        if (*target == CHAR_PLAYER)
        {
            *target = CHAR_PLAYER_LADDER;
            player->ladders--;
        }
    }
}

void updateEnergy(player_t *player, int offset)
{
    player->energy += offset;
    if (player->energy <= 20)
    {
        // Retirar vida e restaurar energia
        player->health--;
        player->energy = 100;
    }
}

void updateScore(player_t *player, int offset)
{
    player->score += offset;

    // Verificar se jogador passou de fase
    if (player->score >= (int)(1000 * pow(2, player->currentLevel - 1)))
        player->currentLevel++;
}

#endif