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

bool isPlayerPlaced(level_t *level)
{
    bool playerPlaced = false;

    // Ler toda a matriz do nível para ver se há um jogador
    for (int i = 0; i < LVL_HEIGHT; i++)
        for (int j = 0; j < LVL_WIDTH; j++)
            if(level->elements[i][j] == CHAR_PLAYER)
                playerPlaced = true;

    return playerPlaced;
}

action_effects_t mine(level_t *level, player_t *player, int direction)
{
    action_effects_t actionEffect = None;

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
    if (*block == CHAR_DIRT || *block == CHAR_TITANIUM || *block == CHAR_GOLD || *block == CHAR_SILVER ||
        *block == CHAR_CAESIUM || *block == CHAR_URANIUM)
    {
        // Atualizar energia, score e último minério do jogador
        switch (*block)
        {
        case CHAR_DIRT:
            actionEffect = DirtMined;
            if (updateEnergy(player, -3))
                actionEffect = PlayerDamaged;
            break;
        case CHAR_TITANIUM:
            actionEffect = OreMined;
            if (updateEnergy(player, 30))
                actionEffect = PlayerDamaged;
            updateScore(player, 150);
            player->lastMined = level->ores[Titanium];
            level->oreCount--;
            break;
        case CHAR_GOLD:
            actionEffect = OreMined;
            if (updateEnergy(player, 20))
                actionEffect = PlayerDamaged;
            updateScore(player, 100);
            player->lastMined = level->ores[Gold];
            level->oreCount--;
            break;
        case CHAR_SILVER:
            actionEffect = OreMined;
            if (updateEnergy(player, 10))
                actionEffect = PlayerDamaged;
            updateScore(player, 50);
            player->lastMined = level->ores[Silver];
            level->oreCount--;
            break;
        case CHAR_CAESIUM:
            actionEffect = OreMined;
            if (updateEnergy(player, -20))
                actionEffect = PlayerDamaged;
            player->lastMined = level->ores[Caesium];
            level->oreCount--;
            break;
        case CHAR_URANIUM:
            actionEffect = OreMined;
            if (updateEnergy(player, -30))
                actionEffect = PlayerDamaged;
            player->lastMined = level->ores[Uranium];
            level->oreCount--;
            break;
        }

        // Remover bloco
        *block = CHAR_EMPTY;

        // Lidar com queda se houver
        switch (moveHorizontal(level, player, 0))
        {
        case PlayerFell:
            if (actionEffect != PlayerDamaged)
                actionEffect = PlayerFell;
            break;
        case PlayerDamaged:
            actionEffect = PlayerDamaged;
            break;
        default:
            break;
        }
    }

    return actionEffect;
}

action_effects_t moveHorizontal(level_t *level, player_t *player, int offset)
{
    action_effects_t actionEffect = None;

    // Verificar se bloco destino livre
    char *target = &level->elements[player->position.y][player->position.x + offset];
    if (*target == CHAR_EMPTY || *target == CHAR_PLAYER || *target == CHAR_LADDER || *target == CHAR_PLAYER_LADDER)
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

        // Se não houver queda
        if(fallSize <= 1)
            actionEffect = PlayerMoved;
        else
            actionEffect = PlayerFell;

        // Retirar vida se queda maior que 3 blocos
        if (fallSize > 3)
        {
            player->health--;
            actionEffect = PlayerDamaged;
        }
    }

    return actionEffect;
}

bool moveVertical(level_t *level, player_t *player, int offset)
{
    bool movedVertical = false;

    // Verificar se bloco atual e destino possuem escadas
    if (level->elements[player->position.y][player->position.x] == CHAR_PLAYER_LADDER &&
        level->elements[player->position.y + offset][player->position.x] == CHAR_LADDER)
    {
        // Alterar matriz
        level->elements[player->position.y][player->position.x] = CHAR_LADDER;
        level->elements[player->position.y + offset][player->position.x] = CHAR_PLAYER_LADDER;

        // Alterar valores posição do jogador
        player->position.y += offset;

        // Atualizar valor de retorno
        movedVertical = true;
    }

    return movedVertical;
}

void placeBlock(level_t *level, player_t *player, position_t mousePosition, editor_option_t selected)
{
    switch (selected)
    {
    case PlayerSlot:
        if (mousePosition.x != player->position.x || mousePosition.y != player->position.y)
        {
            level->elements[player->position.y][player->position.x] = CHAR_EMPTY;
            level->elements[mousePosition.y][mousePosition.x] = CHAR_PLAYER;
            player->position.x = mousePosition.x;
            player->position.y = mousePosition.y;
        }
        break;
    case BackgroundSlot:
        level->elements[mousePosition.y][mousePosition.x] = CHAR_EMPTY;
        break;
    case DirtSlot:
        level->elements[mousePosition.y][mousePosition.x] = CHAR_DIRT;
        break;
    case SilverSlot:
        level->elements[mousePosition.y][mousePosition.x] = CHAR_SILVER;
        break;
    case GoldSlot:
        level->elements[mousePosition.y][mousePosition.x] = CHAR_GOLD;
        break;
    case TitaniumSlot:
        level->elements[mousePosition.y][mousePosition.x] = CHAR_TITANIUM;
        break;
    case CaesiumSlot:
        level->elements[mousePosition.y][mousePosition.x] = CHAR_CAESIUM;
        break;
    case UraniumSlot:
        level->elements[mousePosition.y][mousePosition.x] = CHAR_URANIUM;
        break;
    case Save:
        break;
    }
}

bool placeLadder(level_t *level, player_t *player)
{
    bool ladderPlaced = false;

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

        // Verificar se alvo é vazio
        if (*target == CHAR_EMPTY)
        {
            *target = CHAR_LADDER;
            player->ladders--;
            ladderPlaced = true;
        }

        // Verificar se alvo é jogador
        if (*target == CHAR_PLAYER)
        {
            *target = CHAR_PLAYER_LADDER;
            player->ladders--;
            ladderPlaced = true;
        }
    }

    return ladderPlaced;
}

bool updateEnergy(player_t *player, int offset)
{
    bool energyDrained = false;

    player->energy += offset;
    if (player->energy <= 20)
    {
        // Retirar vida e restaurar energia
        player->health--;
        player->energy = 100;
        energyDrained = true;
    }

    return energyDrained;
}

void updateRankingPositions(player_t *player, ranking_t *players, int rankingSize, int firstAlteredPosition)
{
    // Se o argumento de primeira posição alterada for uma posição válida (não nula)
    if (firstAlteredPosition > 0 && firstAlteredPosition <= rankingSize)
    {
        // Alterar de forma decrescente as posições dos jogadores já existentes
        int i;
        for (i = rankingSize - 1; i > (firstAlteredPosition - 1); i--)
        {
            strcpy(players[i].name, players[i - 1].name);
            players[i].score = players[i - 1].score;
        }

        // Colocar o novo jogador na sua posição do ranking
        strcpy(players[i].name, player->name);
        players[i].score = player->score;
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