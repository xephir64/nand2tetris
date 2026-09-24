# A Mini Space Invaders Game

A small Space Invader-inspired game developed in Jack as part of the Nand2Tetris Project 9.

## How to Play

Don't forget to increase the execution speed.

### Controls
| Key | Action |
| ----------- | ----------- |
| <- | Move Left |
| -> | Move Right |
| Space | Fire a missile |
| Enter | Start the game |
| Esc   | Pause the game |

## Game Mechanic
- The player have 3 lives.
- Destroying an invader gives 10 points.
- Invaders can randomly fire missiles.
- The shields protect the playter from enemy missiles, and they are damaged when a missile hit a shield.
- The game ends when:

## Project Structure

The game is divided into several Jack classes, each with a specific responsibility:

- `Main` : Starts and dispose Game class.
- `Game` : Core class, manage the game loop, game states, input, collisions, and overall game logic.
- `Player` : Handles player position, movement and sprite.
- `Bullet` : Represent missiles.
- `Invader` : Represent an individual enemy.
- `InvaderFleet` : Handles invader fleet, manage invaders formation, movement and shooting.
- `Shield` : Manage Shield.  
- `Random` : Manage random numbers, simple implementation of the Linear Congruential Generator
- `Menu` : Contains drawing functions for "Space Invader" text.

## Some Technical Features

- A State-based Game Loop (Menu, Playing, Paused, Game Over, Win, Exit).
- Reusable bullets to avoid unnecessary object memory allocation.
- Random invader selection for shooting.
- Destructible shields.

## Running the Game 

Yon can use the [Nand2Tetris Web IDE](https://nand2tetris.github.io/web-ide/vm) and load the compiled files.
