#ifndef MAIN_C
#define MAIN_C

#include "../include/main.h"

int main()
{
    // Inicializar janela do jogo
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "TerraINF");
    SetTargetFPS(GAME_FRAMERATE);

    // Inicializar dispositivo de áudio
    InitAudioDevice();

    menu_option_t selected;
    while (!WindowShouldClose())
    {
        // Ler opção do usuário e direcionar à respectiva tela
        selected = startMenu();
        switch (selected)
        {
        case StartGame:
            startGame();
            break;
        case LevelEditor:
            startLevelEditor();
            break;
        case CustomLevels:
            startCustomLevelsMenu();
            break;
        case Ranking:
            startRanking();
            break;
        case Exit:
            CloseWindow();
            break;
        }
    }

    CloseAudioDevice();
    return 0;
}

gameover_option_t gameOver(level_t *level, player_t *player)
{
    gameover_option_t selected = ResetGame;

    // Carregar efeitos de Game Over
    Sound menuSelectionEffect = LoadSound("resources/sound_effects/menu_selection.wav");
    Sound gameOverEffect = LoadSound("resources/sound_effects/gameover.wav");
    PlaySound(gameOverEffect);

    // Tentar abrir o arquivo de ranking existente
    ranking_t players[MAX_RANKING_SIZE];
    if (!readRankingFile("ranking/ranking.bin", players))
    {
        // Verificar criação do arquivo com sucesso
        if (!createRankingFile("ranking/ranking.bin", MAX_RANKING_SIZE))
            readRankingFile("ranking/ranking.bin", players);
    }

    // Verificar se a pontuação do jogador é maior que alguma já existente
    int firstAlteredPosition = 0; // Nula para indicar nenhuma alteração
    for (int i = MAX_RANKING_SIZE - 1; i >= 0; i--)
        if (player->score > players[i].score)
            firstAlteredPosition = players[i].position;

    bool confirmed = false;
    bool nameConfirmed = false;
    // Reiniciar temporização
    uninterruptTimer(true, 0.0f);
    while (!(WindowShouldClose() || confirmed))
    {
        // Se passaram GAMEOVER_NAME_DELAY segundos e houve alguma posição alterada no ranking
        if (uninterruptTimer(false, GAMEOVER_NAME_DELAY) && firstAlteredPosition > 0 &&
            firstAlteredPosition <= MAX_RANKING_SIZE && !nameConfirmed)
        {
            nameConfirmed = highScore(level, player, selected, menuSelectionEffect);
        }
        else
        {
            // Verificar navegação de seleção
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            {
                if (selected < ExitGame)
                {
                    selected++;
                    PlaySound(menuSelectionEffect);
                }
            }
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            {
                if (selected > ResetGame)
                {
                    selected--;
                    PlaySound(menuSelectionEffect);
                }
            }

            // Verificar confirmação de seleção
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
            {
                // Confirmar seleção
                confirmed = true;
                PlaySound(menuSelectionEffect);
            }

            BeginDrawing();
            ClearBackground(BLACK);

            drawGameOverScreen(level, player, selected, ALPHA_DISABLE);

            EndDrawing();
        }
    }

    // Atualizar as posições do vetor local de ranking
    updateRankingPositions(player, players, MAX_RANKING_SIZE, firstAlteredPosition);

    // Caso haja alterações nas posições do ranking
    if (firstAlteredPosition > 0 && firstAlteredPosition <= MAX_RANKING_SIZE)
    {
        // Modificar posições no arquivo de ranking
        for (int i = (firstAlteredPosition - 1); i < MAX_RANKING_SIZE; i++)
            writeRankingPosition("ranking/ranking.bin", &players[i]);

        // Reiniciar o nome do jogador
        for (int i = 0; i < MAX_PLAYER_NAME; i++)
            player->name[i] = '\0';
    }

    UnloadSound(menuSelectionEffect);
    UnloadSound(gameOverEffect);

    // Retornar opção selecionada se confirmado
    return confirmed ? selected : ExitGame;
}

bool highScore(level_t *level, player_t *player, gameover_option_t selected, Sound menuSelectionEffect)
{
    int letterCount = 0;
    float frameCounter = 0;
    bool blinkUnderscore = false;

    Sound highScoreEffect = LoadSound("resources/sound_effects/highscore.wav");
    PlaySound(highScoreEffect);

    bool nameConfirmed = false;
    while (!nameConfirmed)
    {
        // Ler caractere do teclado (Unicode)
        int key = GetCharPressed();

        // Checar se foram pressionadas mais de uma tecla
        while (key > 0)
        {
            // Ler apenas letras do alfabeto (maiúsculas e minúsculas)
            if (((key >= 65 && key <= 90) || (key >= 97 && key <= 122)) && (letterCount < MAX_PLAYER_NAME))
            {
                // Converte as letras para maiúsculas
                player->name[letterCount] = (char)toupper(key);
                player->name[letterCount + 1] = '\0';
                letterCount++;
            }

            // Checar próximo caractere do buffer
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            letterCount--;
            if (letterCount < 0)
                letterCount = 0;
            player->name[letterCount] = '\0';
        }

        if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) && letterCount == MAX_PLAYER_NAME)
        {
            nameConfirmed = true;
            PlaySound(menuSelectionEffect);
        }

        frameCounter++;
        if (frameCounter == GAME_FRAMERATE * BLINK_TIME)
        {
            blinkUnderscore = !blinkUnderscore;
            frameCounter = 0;
        }

        BeginDrawing();
        ClearBackground(BLACK);

        drawGameOverScreen(level, player, selected, 0.15f);
        drawHighScoreTextBox(player, letterCount, MAX_PLAYER_NAME, blinkUnderscore);

        EndDrawing();
    }

    UnloadSound(highScoreEffect);
    return nameConfirmed;
}

void startCustomLevelsMenu(void)
{
    // Carregar fundo
    Texture2D customLevelsBackground = LoadTexture("resources/backgrounds/custom_levels.png");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        drawCustomLevelsMenu(customLevelsBackground);

        EndDrawing();
    }
}

bool saveCustomLevel(char *levelFile, level_t *level, player_t *player)
{
    bool levelSaved = false;

    // Ajeitar nome do diretório do nível customizado
    int metadataFilePathLength = strlen(levelFile);
    int lastSlash = metadataFilePathLength;
    char metadataDirectory[MAX_FILE_NAME + 1] = {0};
    for (int i = 0; i < metadataFilePathLength; i++)
    {
        metadataDirectory[i] = levelFile[i];
        if (metadataDirectory[i] == '/')
            lastSlash = i;
    }

    // Criar nome do arquivo de metadados
    char metadataFile[MAX_FILE_NAME + 1];

    // Preencher o resto do nome do diretório com caracteres nulos
    if (lastSlash != metadataFilePathLength)
    {
        for (int i = lastSlash; i < metadataFilePathLength; i++)
            metadataDirectory[i] = '\0';

        // Colocar diretório no arquivo de metadados
        snprintf(metadataFile, sizeof(metadataFile) - 1, "%s/metadata.bin", metadataDirectory);
    }
    else
        snprintf(metadataFile, sizeof(metadataFile) - 1, "metadata.bin");

    // Tentar abrir arquivo de metadados dos níveis para leitura/escrita
    int customLevelsStored = 0;
    int maxCustomLevelsAmount = 0;
    custom_level_metadata_t metadataStored[MAX_CUSTOM_LEVELS_AMOUNT];
    if (!readCustomLevelsMetadataFile(metadataFile, metadataStored, &customLevelsStored, &maxCustomLevelsAmount))
    {
        // Verificar criação do arquivo com sucesso
        if (!createCustomLevelsMetadataFile(metadataFile, MAX_CUSTOM_LEVELS_AMOUNT))
            readCustomLevelsMetadataFile(metadataFile, metadataStored, &customLevelsStored, &maxCustomLevelsAmount);
    }

    // Guardar as meta-informações do nível atual
    int duplicateLevelName = 0;
    custom_level_metadata_t metadata;
    metadata.dateCreated = time(NULL);
    strncpy(metadata.name, levelFile, sizeof(metadata.name) - 1);
    metadata.miniature = createLevelMiniature(level, player);

    // Atualizar metadados (descobrir nomes duplicados)
    duplicateLevelName = updateCustomLevelsMetadata(&metadata, metadataStored, &customLevelsStored, maxCustomLevelsAmount);

    // Se o nível customizado e metadados foram salvos com sucesso, retornar sucesso
    if (!(saveCustomLevelFile(levelFile, level, duplicateLevelName) || writeCustomLevelsMetadata(metadataFile, metadataStored, customLevelsStored)))
        levelSaved = true;

    return levelSaved;
}

void startGame(void)
{
    // Inicializar jogador e a tecla pressionada por esse
    player_t player = {{0}, {1, 2}, {'\0'}, 0, 3, 100, 20, 1, false};
    KeyboardKey direction = KEY_S;

    // Carregar todos os áudios do jogo
    Music firstLevelsMusic = LoadMusicStream("resources/music/first_levels.mp3");
    Music lastLevelMusic = LoadMusicStream("resources/music/last_level.mp3");
    Sound blockSteppedEffect = LoadSound("resources/sound_effects/block_stepped.ogg");
    Sound dirtMinedEffect = LoadSound("resources/sound_effects/dirt_mined.ogg");
    Sound fallEffect = LoadSound("resources/sound_effects/fall.ogg");
    Sound healthLostEffect = LoadSound("resources/sound_effects/health_lost.ogg");
    Sound ladderClimbedEffect = LoadSound("resources/sound_effects/ladder_climbed.ogg");
    Sound ladderPlacedEffect = LoadSound("resources/sound_effects/ladder_placed.ogg");
    Sound levelUpEffect = LoadSound("resources/sound_effects/level_up.ogg");
    Sound oreMinedEffect = LoadSound("resources/sound_effects/ore_mined.ogg");
    Sound pickaxeEquippedEffect = LoadSound("resources/sound_effects/pickaxe_equipped.ogg");

    // Carregar nível inicial
    level_t level;
    Music *currentMusic = &firstLevelsMusic;
    PlayMusicStream(*currentMusic);
    int currentLevel = player.currentLevel;
    loadLevel(&level, &player);
    drawSplashScreen(&player, currentMusic);
    gameover_option_t gameOverOption = ResetGame;
    while (!(WindowShouldClose() || gameOverOption == ExitGame))
    {
        UpdateMusicStream(*currentMusic);

        // Verificar se as condições de gameover ocorreram
        if (!player.health || !level.oreCount)
        {
            StopMusicStream(*currentMusic);
            gameOverOption = gameOver(&level, &player);
            if (gameOverOption == ResetGame)
            {
                player.health = 3;
                player.currentLevel = 1;
                PlayMusicStream(*currentMusic);
                currentLevel = player.currentLevel;
                loadLevel(&level, &player);
                drawSplashScreen(&player, currentMusic);
            }
        }

        // Verificar troca de nível
        if (currentLevel != player.currentLevel)
        {
            currentLevel = player.currentLevel;
            if (currentLevel == LAST_LVL)
            {
                StopMusicStream(*currentMusic);
                currentMusic = &lastLevelMusic;
                PlayMusicStream(*currentMusic);
            }
            loadLevel(&level, &player);
            PlaySound(levelUpEffect);
            drawSplashScreen(&player, currentMusic);
        }

        // Verificar movimentação
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
        {
            switch (moveHorizontal(&level, &player, 1))
            {
            case PlayerMoved:
                PlaySound(blockSteppedEffect);
                break;
            case PlayerFell:
                PlaySound(fallEffect);
                break;
            case PlayerDamaged:
                PlaySound(healthLostEffect);
                break;
            default:
                break;
            }
            direction = KEY_D;
        }
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
        {
            switch (moveHorizontal(&level, &player, -1))
            {
            case PlayerMoved:
                PlaySound(blockSteppedEffect);
                break;
            case PlayerFell:
                PlaySound(fallEffect);
                break;
            case PlayerDamaged:
                PlaySound(healthLostEffect);
                break;
            default:
                break;
            }
            direction = KEY_A;
        }
        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
        {
            if (moveVertical(&level, &player, -1))
                PlaySound(ladderClimbedEffect);
            direction = KEY_W;
        }
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
        {
            if (moveVertical(&level, &player, 1))
                PlaySound(ladderClimbedEffect);
            direction = KEY_S;
        }

        // Verificar modo mineração
        if (IsKeyPressed(KEY_ONE))
        {
            player.miningMode = !player.miningMode;
            PlaySound(pickaxeEquippedEffect);
        }

        // Verificar mineração
        if (IsKeyPressed(KEY_SPACE) && player.miningMode)
            // Se houver bloco minerado
            switch (mine(&level, &player, direction))
            {
            case DirtMined:
                PlaySound(dirtMinedEffect);
                break;
            case OreMined:
                PlaySound(oreMinedEffect);
                break;
            case PlayerFell:
                PlaySound(fallEffect);
                break;
            case PlayerDamaged:
                PlaySound(healthLostEffect);
                break;
            default:
                break;
            }

        // Verificar posicionamento de escada
        if (IsKeyPressed(KEY_LEFT_SHIFT))
            if (placeLadder(&level, &player))
                PlaySound(ladderPlacedEffect);

        BeginDrawing();
        ClearBackground(BLACK);

        // Desenhar texturas do nível
        drawLevel(&level, &player, ALPHA_DISABLE);

        // Desenhar HUD
        drawHUD(&level, &player, ALPHA_DISABLE);

        EndDrawing();
    }

    // Descarregar todos os áudios do jogo
    UnloadSound(blockSteppedEffect);
    UnloadSound(dirtMinedEffect);
    UnloadSound(fallEffect);
    UnloadSound(healthLostEffect);
    UnloadSound(ladderClimbedEffect);
    UnloadSound(ladderPlacedEffect);
    UnloadSound(levelUpEffect);
    UnloadSound(oreMinedEffect);
    UnloadSound(pickaxeEquippedEffect);
    UnloadMusicStream(firstLevelsMusic);
    UnloadMusicStream(lastLevelMusic);
}

menu_option_t startMenu(void)
{
    // Carregar sprites
    Texture2D menuTexture = LoadTexture("resources/backgrounds/menu.png");

    // Selecionar primeira opção do menu
    static menu_option_t selected = StartGame;

    // Carregar e tocar música de menu
    Music menuMusic = LoadMusicStream("resources/music/menu.xm");
    PlayMusicStream(menuMusic);

    // Carregar efeito de seleção do menu
    Sound menuSelectionEffect = LoadSound("resources/sound_effects/menu_selection.wav");

    bool confirmed = false;
    while (!(WindowShouldClose() || confirmed))
    {
        // Verificar navegação de seleção
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        {
            if (selected < Exit)
            {
                selected++;
                PlaySound(menuSelectionEffect);
            }
        }
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        {
            if (selected > StartGame)
            {
                selected--;
                PlaySound(menuSelectionEffect);
            }
        }

        // Verificar confirmação de seleção
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
        {
            // Confirmar seleção
            confirmed = true;
            PlaySound(menuSelectionEffect);
        }

        UpdateMusicStream(menuMusic);

        BeginDrawing();
        ClearBackground(BLACK);

        drawMenuScreen(menuTexture, selected);

        EndDrawing();
    }

    // Descarregar todos áudios
    UnloadSound(menuSelectionEffect);
    UnloadMusicStream(menuMusic);

    // Retornar opção selecionada se confirmado
    return confirmed ? selected : Exit;
}

ranking_option_t startRanking(void)
{
    ranking_option_t selected = ExitRanking;

    Music rankingMusic = LoadMusicStream("resources/music/ranking.mp3");
    PlayMusicStream(rankingMusic);

    // Carregar efeitos sonoros do ranking
    Sound menuSelectionEffect = LoadSound("resources/sound_effects/menu_selection.wav");

    // Tentar abrir o arquivo de ranking existente
    ranking_t players[MAX_RANKING_SIZE];
    if (!readRankingFile("ranking/ranking.bin", players))
    {
        // Verificar criação do arquivo com sucesso
        if (!createRankingFile("ranking/ranking.bin", MAX_RANKING_SIZE))
            readRankingFile("ranking/ranking.bin", players);
    }

    bool confirmed = false;
    while (!(WindowShouldClose() || confirmed))
    {
        // Verificar confirmação de seleção
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
        {
            // Confirmar seleção
            confirmed = true;
            PlaySound(menuSelectionEffect);
        }

        UpdateMusicStream(rankingMusic);

        BeginDrawing();
        ClearBackground(BLACK);

        drawRankingScreen(players, MAX_RANKING_SIZE, selected);

        EndDrawing();
    }

    UnloadSound(menuSelectionEffect);
    UnloadMusicStream(rankingMusic);
    return confirmed ? selected : ExitRanking;
}

void startLevelEditor(void)
{
    // Carregar áudios
    Music levelEditorMusic = LoadMusicStream("resources/music/level_editor.mp3");
    PlayMusicStream(levelEditorMusic);

    // Carregar template de nível
    level_t level;
    player_t player;
    editor_option_t selected = PlayerSlot;
    bool levelSaved = false;
    player.miningMode = false;
    player.position.x = 11;
    player.position.y = 2;
    loadEditorLevel(&level);

    while (!(WindowShouldClose() || levelSaved))
    {
        // Verificar navegação de seleção
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        {
            if (selected < Save)
                selected++;
        }
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        {
            if (selected > PlayerSlot)
                selected--;
        }

        // Verificar salvamento do nível
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
        {
            if (isPlayerPlaced(&level))
            {
                // Salvar nível
                char levelFile[MAX_FILE_NAME + 1] = "custom_levels/nivel1.txt";
                levelSaved = saveCustomLevel(levelFile, &level, &player);

//NÃO ESCLUIR, FALTA TESTAR ESTA PARTE
/*                int customLevelsAmount = 0;
                int maxCustomLevelsAmount = 0;
                custom_level_metadata_t metadata[MAX_CUSTOM_LEVELS_AMOUNT];
                readCustomLevelsMetadataFile();
                while(!WindowShouldClose())
                {
                    
                }*/
            }
        }

        // Verificar posicionamento de bloco
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            // Obter posição do mouse
            position_t mousePosition = {(GetMouseX() / ELEMENT_SIZE), (GetMouseY() / ELEMENT_SIZE)};

            // Verificar borda
            if (mousePosition.x > 0 && mousePosition.x < LVL_WIDTH - 1 && mousePosition.y > 0 &&
                mousePosition.y < LVL_HEIGHT - 1)
            {
                // Posicionar bloco
                placeBlock(&level, &player, mousePosition, selected);
            }
        }

        UpdateMusicStream(levelEditorMusic);

        BeginDrawing();
        ClearBackground(BLACK);

        drawEditorLevel(&level);
        drawEditorHUD(&level, selected);

        EndDrawing();
    }

    UnloadMusicStream(levelEditorMusic);
}

#endif
