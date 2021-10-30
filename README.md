# TowerDefense
In this project, a game engine is created using C++ and SDL2. The engine uses actors and components to structure its game. It supports only 2D graphics using SDL_image. The engine is then used to create a Tower Defense game. <br>
The game itself is simple: it allows the player to create towers by left clicking on the map, and initiate the game by pressing the right mouse button. The computer then spawns enemies in a constant rate that try to reach the other side of the map. <br>
The game features an A* path finding algorithm that allows the player to build a tower even on the path tiles, as long as there is still a valid path for the enemies to go from the beginning to the end of the map. This allows the customization of the route taken by the enemies directly by blocking the original one with towers.

# Controls
Left mouse button: Places a tower on the tile. Apart from the beginning and end tiles, any tile can be chosen, even the ones forming the path for the enemies, as long as there is a valid route connecting the beginning and the end.<br>
Right mouse button: Initializes the game, making enemies come at a constant rate. After it is pressed, no new towers can be placed in the path.<br>
ESC button: exits game

# Compilation and Use
The repository already contains TowerDefense.exe file containing the game corresponding to the code in the repository. <br>
If you want to change it, by substituting the values of the enemies' HP or spawn rate, Tower's reload time, among others, this can be easily done by changing the const variables on the beginning of the Tower.cpp, Enemy.cpp, Projectile.cpp and Grid.cpp.<br>
To recompile it using mingw32, go to the root folder and run "$mingw32-make.exe". No external library is needed.
