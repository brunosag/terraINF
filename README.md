<h1 align="center">
    <br>
    <img src="https://user-images.githubusercontent.com/97703272/231913688-c90284cf-8a46-471f-94b1-1f9574a49f61.png" alt="TerraINF" width="200">
    <br>
    TerraINF
    <br>
</h1>

<h4 align="center">A plataform mining game built with <a href="https://en.wikipedia.org/wiki/C_(programming_language)" target="_blank">C</a> & <a href="https://www.raylib.com/" target="_blank">Raylib</a>.</h4>

<p align="center">
    <a href="#about">About</a> •
    <a href="#instructions">Instructions</a> •
    <a href="#warnings">Warnings</a> •
    <a href="#license">License</a> •
    <a href="#contact">Contact</a>
</p>

## About

TerraINF is a single-player plataform mining game that challenges you to obtain the highest possible score by collecting specific types of ores. With each level, the required score increases by 100%, making the game progressively more challenging. To achieve your goal, you must break down blocks of earth or mine ores, which can increase or decrease your energy.

There are five types of ores in the game, including silver, gold, and titanium, which increase both your score and energy. In contrast, cesium and uranium decrease your energy. You have free horizontal movement in the game, but vertical movement is limited to ladders, which are available in sets of twenty at the start of each level.

To add to the challenge, you begin the game with three lives. If you fall from a height greater than three blocks, you will lose one life. Similarly, if your energy drops below 20 units, you will lose one life, and your energy will reset to the initial amount. Your remaining lives carry over to the next level. If you lose all your lives and your health reaches zero, the game ends, and you can choose to restart from the first level or return to the main menu.

## Instructions

### Main Menu

<img src="https://user-images.githubusercontent.com/97703272/232647527-afcdf7b8-3395-4b1a-b10e-6011df708ce5.png" align="left" width="450">

When the game executable file is launched, the main menu appears, offering the five options described in the game description. The user can navigate through the menu options using the keyboard, either by pressing the W (up) and S (down) keys or the up and down arrow keys. Pressing the ENTER key confirms the desired option. All menus in the game can be navigated using these keys.

<br clear="left">

### Splash Screen

<img src="https://user-images.githubusercontent.com/97703272/232647711-a6a438f0-9415-48c1-9033-53b0c1888090.png" align="left" width="450">

By selecting the "Start Game" option, a splash screen appears with the number of the level to be played.

<br clear="left">

### HUD & Movement

<img src="https://user-images.githubusercontent.com/97703272/232666859-ae91cb16-fdc5-429e-a470-75e46bfe0fa3.png" align="left" width="450">

The game begins, and the top bar displays the player's health (in green), energy (in yellow), stairs (in red), score and the score required to advance to the next level (in purple), and the current level (in blue). The playable space appears below the HUD. The edges of the screen are unreachable, while the lighter areas indicate the accessible areas without mining. The player can move around using the W, A, S, D keys (up, left, down, and right, respectively) or the arrow keys. To move up and down, ladders must be positioned using the SHIFT key.

<br clear="left">

### Mining

<img src="https://user-images.githubusercontent.com/97703272/232666939-5c8bbeb1-36d8-40e1-8055-46570ffa2731.png" align="left" width="450">

The "1" key activates the mining mode, allowing the player to move into previously blocked spaces. To mine an ore, the user must indicate the target direction using the keys and then press SPACE. All ores start hidden, and after mining, the game indicates the last item acquired in the center of the HUD (in red) and the new score after obtaining it (in blue), as shown in the image. Also visible in the image above are the stairs placed on the level (in yellow).

<br clear="left">

### Game Over

<img src="https://user-images.githubusercontent.com/97703272/232647779-24470534-b916-42a7-bab5-f7d2b9ea415a.png" align="left" width="450">

If the player runs out of lives, the game ends and displays a game over screen with two options: restart the game from level 1 or exit to the main menu. These options can be selected using the same keys described earlier. The game over screen also displays the reason why the game ended, which could be related to exhausted lives or the inability of the level to be completed due to a lack of ores.

<br clear="left">

### High Score

<img src="https://user-images.githubusercontent.com/97703272/232647809-b80972c8-eb8e-42b0-b18f-42a9001a41f6.png" align="left" width="450">

If the player achieves a high score, the game prompts them to enter their name (a maximum of three characters). The name is then added to the high score ranking screen. The main game does not present a victory screen, as it has only 3 levels, and the last level does not contain enough ores to reach the established goal, maintaining a healthy ranking system.

<br clear="left">

### Level Editor

<img src="https://user-images.githubusercontent.com/97703272/232666987-42b92b99-b188-417d-9877-da3e2d7473a6.png" align="left" width="450">

The level editor option on the main menu allows the player to create their custom levels. It features an interactive screen that can be navigated with either the mouse or keyboard. The keyboard shortcuts include using the A and D keys or the side arrow keys to select an ore or save the level. The mouse can also be used to click on the selectable items on the HUD. Pressing the ESC key exits the level editor without saving and returns to the initial menu. The selectable items are marked in yellow, and the save option is only selected by hovering over them and left-clicking or using the aforementioned keys and pressing ENTER.

<br clear="left">

### Saving Custom Level

<img src="https://user-images.githubusercontent.com/97703272/232647877-e682db1c-7a84-48cb-8c2c-95bcbbd30ade.png" align="left" width="450">

The level save screen prompts the user for a name for the level. The name can contain several types of characters, but the letters are always uppercase. The acceptable length is from 1 to 20 characters. After entering a valid name, the confirmation option is unlocked, and if selected with the ENTER key, it redirects the player to the start menu. The game can store up to three custom levels, discarding the oldest one automatically.

<br clear="left">

### Custom Levels Menu

<img src="https://user-images.githubusercontent.com/97703272/232647887-741e6195-034f-4868-9f3e-a18131866993.png" align="left" width="450">

Once a new level is saved, it can be accessed via the "Custom Levels" option on the start menu. This screen displays each custom level created, along with its name, thumbnail, and creation date. If no custom levels have been created yet, this screen will be empty, and the player can either return to the menu by selecting the "Exit" option (using the same selection keys as in the main menu) or by pressing the ESC key. To play a custom level, select it and press ENTER, and the player will be redirected to the desired level.

<br clear="left">

### Custom Game

<img src="https://user-images.githubusercontent.com/97703272/232647908-2d5faa23-2e8f-4bfe-a6c3-e65e8cf66f10.png" align="left" width="450">

The custom game mode is very similar to the regular game mode, except that it does not feature a splash screen or a level name on the HUD. Additionally, the score is calculated based on the number of positive-scoring ores that were placed. The screenshot included in this manual illustrates these differences well. This game mode features a game over and all other mechanics similar to the original mode, with the only differences being that there is a victory screen if the player manages to collect all positive-scoring ores in the stage, and the ranking system does not apply. To return to the start menu, simply press the ESC key at any point during the custom game.

<br clear="left">

### Victory

<img src="https://user-images.githubusercontent.com/97703272/232647925-3dc022fa-e898-4508-a6b6-29e23baaba29.png" align="left" width="450">

<br clear="left">

### Ranking Screen

<img src="https://user-images.githubusercontent.com/97703272/232647982-09434d75-2057-4153-866e-12e540665184.png" align="left" width="450">

The last option on the start menu shows the top player rankings for the main game. This option only has an "Exit" button to return to the menu. The screenshot shows the score obtained in the high score demo of the main game.

<br clear="left">

## Warnings

The game was initially developed on a Windows operating system using VS Code, but conditions have been added to make it easier to compile the code on any operating system. However, while efforts have been made to ensure compatibility across platforms, there is no guarantee that the code will compile without issue on every system. Additionally, as the implementation was done using VS Code, the proper functioning of the code in other IDEs cannot be guaranteed.

To run the game with no issues, you must have the game's executable file (.exe), the "levels" folder containing levels 1, 2, and 3, and the editor level in .txt format, as well as the "resources" folder containing all the sound effects, music, sprites, and backgrounds. These folders must be located in the same directory as the executable file. Note that the "ranking" and "custom_levels" folders are created as the game runs and are not necessary for the first-time launch. However, if they are deleted after the first run, all ranking and custom level data will be reset.

## License

Distributed under the MIT License. See `LICENSE` for more information.

## Contact

Ana Cláudia Rodrigues - [LinkedIn](https://www.linkedin.com/in/ana-cl%C3%A1udia-rodrigues-1b3524221/) - rodrigues.ac2001@gmail.com
<br>
Bruno Samuel - [LinkedIn](https://www.linkedin.com/in/brunosag/) - brunosag@outlook.com.br
<br>
Pedro Lima - [LinkedIn](https://www.linkedin.com/in/pedro-lubaszewski/) - pedro.llima@inf.ufrgs.br
