/*
 * Implementation of the computer-played snake game
 * with a rudimentary AI.
 */
#include "snake.h"

static void OpenUserFile(ifstream& input);
static bool CheckFileFormat(ifstream& input);
static void LoadWorld(gameT& game, ifstream& input);
static pointT MakePoint(int row, int col);
static void ShowWorld(gameT& game);
static void Pause(void);
static void DisplayResult(gameT& game);
static void PerformAI(gameT& game);
static pointT GetNextPosition(gameT& game, int dx, int dy);
static bool InWorld(gameT& game, pointT nextPos);
static bool MoveSnake(gameT& game);
static void PlaceFood(gameT& game);
static bool Crashed(gameT& game, pointT nextPos);
static void DecideDirection(gameT& game);
static bool RandomChance(double probability);

/*
 * The main program.
 * Initialize the world, then run the simulation.
 */
int main()
{
  gameT game;
  InitGame(game);
  RunSimulation(game);
}

void InitGame(gameT& game)
{
  /* Seed the randomizer. static cast convert time_t to unsigned int.
     This line is idiomatic in C++. */
  srand(static_cast<unsigned int>(time(NULL)));

  ifstream input;
  OpenUserFile(input);
  
  LoadWorld(game, input);
}

static void OpenUserFile(ifstream& input)
{
  while (true) {
    string fileName;
    cout << "Enter filename:";
    getline(cin, fileName);
  
    input.open(fileName.c_str());
    if (input.is_open()) {
      if (CheckFileFormat(input)) return;
      else {
	cout << "Sorry, file format is not correct." << endl;
	input.close();
	continue;
      }
    }

    cout << "Sorry, I cannot find the file." << endl;
    /* Reset the stream's error state since opening a nonexistent
       file causes the stream to fail. */
    input.clear();
  }
}

/* Just return true temporarily. Implement it later. */
static bool CheckFileFormat(ifstream& input)
{
  return true;
}

static void LoadWorld(gameT& game, ifstream& input)
{
  input >> game.numRows >> game.numCols;
  game.world.resize(game.numRows);
  input >> game.dx >> game.dy;

  string dummy;
  /* Filter the remaining newline character in cin's buffer. */
  getline(input, dummy);
  for (int row = 0; row < game.numRows; ++row) {
    getline(input, game.world[row]);
    /* Set up the snake. */
    int col = game.world[row].find(kSnakeTile);
    if (col != string::npos) 
      game.snake.push_back(MakePoint(row, col));
  }

  game.numEaten = 0;
}

static pointT MakePoint(int row, int col)
{
  pointT point;
  point.row = row;
  point.col = col;
  return point;
}

void RunSimulation(gameT& game)
{
  while (game.numEaten < kMaxFood) {
    ShowWorld(game); // Display the board
    PerformAI(game); // Let the AI choose the direction

    /* Move the snake and stop if we crashed. */
    if (!MoveSnake(game))
      break;

    Pause();
  }

  DisplayResult(game);
}

static bool MoveSnake(gameT& game)
{
  pointT nextHead = GetNextPosition(game, game.dx, game.dy);
  if (Crashed(game, nextHead))
    return false; // Entering a dead end

  bool isFood = (game.world[nextHead.row][nextHead.col]
		 == kFoodTile);

  /* Update the world representation to reflect the move of the snake. */
  game.world[nextHead.row][nextHead.col] = kSnakeTile;
  game.snake.push_front(nextHead); // Update the snake representation to reflect change.
  if (!isFood) { // No food, need to cut the tail.
    game.world[game.snake.back().row][game.snake.back().col] = kEmptyTile;
    game.snake.pop_back();
  } else { // There is food at next position.
    ++game.numEaten;
    PlaceFood(game);
  }
  return true;
}

/* Randomly pick a location in the world and place a piece of food on it. */
static void PlaceFood(gameT& game)
{
  while (true) {
    int row = rand() % game.numRows;
    int col = rand() % game.numCols;
    if (game.world[row][col] == kEmptyTile)
      game.world[row][col] = kFoodTile;
    return;
  }
}

static void ShowWorld(gameT& game)
{
  /* Call system specific command to flush the display for the next frame. */
  system(kClearCommand.c_str());
  for (int i = 0; i < game.numRows; ++i) {
    cout << game.world[i] << endl;
  }
  cout << "Food eaten: " << game.numEaten << endl;
}

static void Pause(void)
{
  clock_t startTime = clock();

  /* Busy looping until specified wait time reached. */
  while ((clock()-startTime) < kWaitTime) ;
}

static void DisplayResult(gameT& game)
{
  if (game.numEaten == kMaxFood)
    cout << "The snake ate enough food and wins!" << endl;
  else
    cout << "Oh no! The snake crashed!" << endl;
}

/* Choose a direction(velocity) for the snake that
   determines snake's next move.

   Our rudimentary AI works as follows:
   If the snake if about to crash in its current direction,
   the AI will turn the snake out of danger. Otherwise
   the snake will continue on its current direction, but
   has a percent chance to randomly change direction. */
static void PerformAI(gameT& game)
{
  /* Figure out where we will be after we move this turn. */
  pointT nextHead = GetNextPosition(game, game.dx, game.dy);

  /* If the snake hits a wall or we randomly decide to, turn the snake. */
  if (Crashed(game, nextHead) || RandomChance(kTurnRate)) {
    DecideDirection(game); // Modify the velocity of the snake if necessary.
  } 
}

static void DecideDirection(gameT& game)
{
  /* Compute the velocity for turing left or right by
     multipling vector (game.dx, game.dy) with a 90 degree
     rotate matrix R(angle): [cos(pi/2), -sin(pi/2)]
     [sin(pi/2), cos(pi/2)] */
  /* Turn left which is couterclockwise(positive angle).
     result of R(pi/2) * velocity-vector. */
  int leftDx = -game.dy;
  int leftDy = game.dx;

  /* Turn right which is clockwise(negative angle).
     result of R(-pi/2) * velocity-vector. */
  int rightDx = game.dy;
  int rightDy = -game.dx;

  bool leftOK = !Crashed(game, GetNextPosition(game, leftDx, leftDy));
  bool rightOK = !Crashed(game, GetNextPosition(game, rightDx, rightDy));

  bool willTurnLeft;
  if (!leftOK && !rightOK)
    return; // If we can't turn, don't turn.
  else if (!leftOK && rightOK)
    willTurnLeft = false; // If we must turn right, do so.
  else if(leftOK && !rightOK)
    willTurnLeft = true; // If we must turn left, do so.
  else
    willTurnLeft = RandomChance(0.5); // If we can turn both way, pick randomly.

  game.dx = willTurnLeft ? leftDx : rightDx;
  game.dy = willTurnLeft ? leftDy : rightDy;
}

static pointT GetNextPosition(gameT& game, int dx, int dy)
{
  /* Get the head position. */
  pointT result = game.snake.front();

  /* Increment the head position by the specified direction. */ 
  result.row += dx;
  result.col += dy;
  return result;
}

/* Check if the snake is going to crash at 'nextPos'. */
static bool Crashed(gameT& game, pointT nextPos)
{
  return !InWorld(game, nextPos) ||
    (game.world[nextPos.row][nextPos.col] == kWallTile) ||
    (game.world[nextPos.row][nextPos.col] == kSnakeTile);
}

static bool InWorld(gameT& game, pointT pt)
{
  return (pt.row < game.numRows) &&
    (pt.col < game.numCols) &&
    (pt.row >= 0) && (pt.col >= 0);
}

static bool RandomChance(double probability)
{
  return rand()/(RAND_MAX + 1.0) < probability;
}
