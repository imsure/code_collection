/*
 * Interface for snake game.
 * ------------------------------
 * The rules of the game we are implementing:
 * 1. The snake moves by extending its head in the direction
 *    it's moving and pulling its tail in one space.
 * 2. The snake wins if it eats twenty pieces of food.
 * 3. The snake loses if it crashes into a wall or into itself.
 * 4. If the snake eats a piece of food, its length grows by one
 *    and a new piece of food is randomly placed.
 * 5. There are only one level, the starting level.
 *
 * Though traditionally Snake game is played by human, our snake
 * game will be computer-controlled with a simple AI algorithm.
 */

#include <iostream>
#include <vector>
#include <string>
#include <deque>
#include <fstream>
#include <cassert>
#include <ctime> // clock_t, clock(),...
#include <cstdlib> // system(),...

using namespace std;

struct pointT {
  int row, col;
};

/* Bundle all the game data into a single struct
   so we can pass all the game information to functions
   as a single parameter. */
struct gameT {
  /* World Representation.
     In order to represent the snake world, we need to keep
     track of the following information:
     1. The size and layout of the world.
     2. The location of the snake.
     3. How many pieces of food we've consumed. */

  /* We'll use a vector of string to represent the snake world.
     Each string encodes one row of the board.
     We'll use the following character to encode game board:
     # A space character (' ') represents an empty tile.
     # A pound sign ('#') represents a wall.
     # A dollor sign ('$') represents food.
     # An asterisk ('*') represents a tile occupied by a snake.

     For consistency, we'll treat the first index as row and
     the seconde index as column. Thus world[3][5] is row three,
     column five (zero-indexed). */
  
  vector<string> world; // The playing field

  /* Since we will need quick access to the dimensions of the
     playing field to be able to check whether the snake is out
     of bounds, we store this information explicitly.  */
  
  int numRows, numCols; // Size of the playing field

  /* To represent the snake in memory, we need to keep track of
     its velocity and an ordered list of the points comprising it.
     For efficiency issue, we will use a deque to encode the snake body. */
  
  deque<pointT> snake; // The snake body
  int dx, dy; // velocity : (delta x, delta y)

  /* Finally, we need to keep track of how many pieces of food
     we've eaten so far. */
  int numEaten;
};

const char kEmptyTile = ' ';
const char kWallTile = '#';
const char kFoodTile = '$';
const char kSnakeTile = '*';

const int kMaxFood = 20;

const string kClearCommand = "clear";

/* Pause 100ms between frames. */
const int kWaitTime = CLOCKS_PER_SEC / 10;

/* The percent chance to change snake's direction */
const double kTurnRate = 0.3;

void InitGame(gameT& game);
void RunSimulation(gameT& game);

