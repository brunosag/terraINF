<h1 align="center">
    <br>
    <img src="https://user-images.githubusercontent.com/97703272/231913688-c90284cf-8a46-471f-94b1-1f9574a49f61.png" alt="TerraINF" width="200">
    <br>
    TerraINF
    <br>
</h1>

<h4 align="center">A simplified version of Terraria built with <a href="https://en.wikipedia.org/wiki/C_(programming_language)" target="_blank">C</a> & <a href="https://www.raylib.com/" target="_blank">Raylib</a>.</h4>

<p align="center">
    <a href="#game-description">Game Description</a> •
    <a href="#how-to-use">How to Use</a> •
    <a href="#warnings">Warnings</a> •
    <a href="#license">License</a> •
    <a href="#contact">Contact</a>
</p>

## Game Description

TerraINF is a single-player mining game where the goal is to acquire the highest possible score by obtaining certain types of ores. With a specific score (which increases by 100% with each level), the player progresses through stages, increasing their challenge. Mining or breaking down blocks of earth increases or decreases the player's energy. The game has five ores: three of those (silver, gold, and titanium) increase the player's score and energy, and the others (cesium and uranium) decrease the player's energy. The player's horizontal movement is free, while vertical movement depends on ladders, which are available in twenty units at the beginning of each level. For the purposes of increasing challenge, the player has 3 lives at the beginning of the game. If there is a fall higher than 3 blocks, the player loses 1 life, and if his energy drops below 20 units, he also loses 1 life (and his energy is reset to the initial amount). Those lives are kept for the next level. When the health reaches zero, the player loses the game and can restart it from the first level or exit to the main menu.

The game menu presents the options to: start the game; create custom levels; play custom levels; access high score rankings; and exit the game.

The score ranking displays the name of the top 5 players sorted by highest score. If it is the first time opening the game, the leaderboard has 5 players with random names and zero score.

The level editor is an interactive screen where, using the mouse, the user can draw new levels and save them with a custom name. These levels can be accessed from the "custom levels" option on the main menu. The system stores up to 3 custom levels, automatically deleting the oldest one when creating a level that exceeds the maximum capacity. When selecting a level, the player is redirected to a custom game, where the score required to finish the game depends on the number of score-increasing ores that have been placed. If this score is reached, a victory screen is displayed, allowing the player to restart or exit the custom level. However, if there are no lives left, a game over screen shows up, with the same options as the victory screen. Scores from custom levels are not considered in the ranking.

## How to Use

When the executable file is started, the main menu of the game presents the five options mentioned earlier in the game description and illustrated in the screenshot below.

### Initial Menu

The user can navigate through the menu options using the keyboard: either with the W (up) and S (down) keys, or the up and down arrow keys, and the ENTER key to confirm the desired option. All menus can be navigated this way.

By selecting the "Start Game" option, a screen is displayed with the number of the level to be played.

### Level 1 Splash Screen

After this screen, the game itself starts. In it, it is possible to keep track, through the top bar, the player's health (marked in light green), energy (in red), stairs (in blue), score and the score required to advance to the next level (in pink), and the current level (in purple). The markings are demonstrated in the screenshot below.

### Level 1

Below the HUD is the playable space. The edges are unreachable to the player, while the lighter spaces indicate the areas accessible without mining. Movement can be done using either the W, A, S, D keys (up, left, down, and left respectively), or the arrow keys on the keyboard. To advance vertically, ladders must be positioned with the SHIFT key. The mining mode, activated with the "1" key, allows movement to previously blocked spaces; to perform the mining action, the user must indicate, using the keys, the target direction and then press SPACE. All ores start hidden, and after mining them, the game indicates the last item acquired in the center of the HUD (marked in red) and the new score after obtaining it (in orange), as shown in the following image.

### Mining at Level 1

Also observed in the image above are the stairs placed on the level (in blue). As already explained in the description of the game, when there are no lives remaining, the game ends, generating a game over screen.

### Game Over Screen

Two options are presented: restart the game from level 1 or exit to the main menu, they can be selected with the keys described above. Below the message "Game Over" is the reason why the game ended, which can be related to exhausted lives or the inability of the level to be completed (lack of ores). If the player has a better score than any ranking member, the player's name is requested to be placed in the high score ranking screen, as exemplified below.

### High Score Screen

Inspired by the old Pac-Man games, the name stored is exactly 3 characters long (if typed in lowercase, it will be converted to uppercase). By entering the 3 characters, the confirm option is unlocked, allowing the user to return to the game over screen.

### Confirmation on High Score Screen

The main game does not present a victory screen, since it has 3 levels and the last level does not contain enough ores to reach the established goal, maintaining a healthy ranking system.

Returning to the main menu and selecting the level editor option, the player is greeted with an interactive screen which can be navigated with the mouse or keyboard. With the keyboard, A and D or the side arrows can be used to select an ore or save the level. The mouse also works and makes it possible to click on selectable items on the HUD. If the ESC key is pressed, the game will exit without saving and return to the initial menu. Selectable items are marked in orange, and the save option is only selected by hovering over them and left clicking, or by using the aforementioned keys and pressing ENTER.

### Level Editor

The level save screen shown below prompts the user for a name for the level. This name accepts several types of characters, but the letters saved will be uppercase only. The acceptable length is from 1 to 20 characters. After entering a valid name, the confirmation option is unlocked and, if selected with the ENTER key, redirects the player to the start menu. Up to 3 custom levels can be stored, automatically discarding the oldest.

### Custom Level Saving

With the new level saved, it can be accessed through the "Custom Levels" option on the start menu. This screen displays each custom level created with its name, thumbnail, and creation date. When none have been created yet, this screen is empty, allowing the user to return to the menu either by selecting the "Exit" option (same selection keys as in the initial menu) or by pressing the ESC key. Below is shown the screen with the newly created example level for this manual.

### Custom Levels Menu

By selecting the custom level and pressing the ENTER key, the player is redirected to the desired level. The custom game is very similar to the regular one, however it does not feature a Splash Screen nor a level name on the HUD. Aditionally, the score is calculated based on the amount of positive-scoring ores that were placed. The picture below elucidates these differences well.

### Custom Game

This game mode features a game over and all other mechanics similar to the original mode. The only differences being that there is a victory screen if the player manages to collect all positive-scoring ores in the stage, and the ranking system does not apply. To return to the start menu, simply press the ESC key at any point in the custom game.

### Victory Screen

The last option of the start menu shows the top player ranking for the main game. It only has an "Exit" option to return to the menu. The picture below shows the score obtained in the high score demo of the main game:

## Warnings

The game was developed on a Windows operating system using VS Code. Conditions have been added to make it easier to compile it on any operating system. However, there is no guarantee that the code will compile on any platform. Also, as the implementations were made in VS Code, the proper functioning of the files in other IDEs is not guaranteed.

For the proper functioning of the game, it is necessary to have the game's executable file (.exe), the "levels" folder containing levels 1, 2 and 3, and the editor level in txt format and the "resources" folder containing all sound effects, music, sprites and backgrounds. These folders must be located in the same directory as the executable file. The "ranking" and "custom_levels" folders are created as the game runs, so they are not needed when it runs for the first time. However, when deleted after the first time, the "ranking" and "custom_levels" data is reset.

## License

Distributed under the MIT License. See `LICENSE` for more information.

## Contact

Bruno Samuel - [LinkedIn](https://www.linkedin.com/in/brunosag/) - brunosag@outlook.com.br
<br>
Pedro Lima - [LinkedIn](https://www.linkedin.com/in/pedro-lubaszewski/) - pedro.llima@inf.ufrgs.br
<br>
Ana Cláudia Rodrigues - [LinkedIn](https://www.linkedin.com/in/ana-cl%C3%A1udia-rodrigues-1b3524221/) - rodrigues.ac2001@gmail.com
