#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <vector>

// Represents a 2x2 grid of the planet
class Grid {
public:

  /**
   * Constructs a Grid object
   *
   * This object keeps track of what obstacles are currently on the grid.
   * This object also handles wrapping around the planet
   **/
  Grid(int numRows, int numCols) {
    this->obstacleMap.assign(numRows, std::vector<bool>(numCols, true));
  }

  /**
   * GETTERS
   **/
  int getNumRows() { return this->obstacleMap.size(); }
  int getNumCols() { return this->obstacleMap[0].size(); }

  /**
   * Places an obstacle at the given row and column
   **/
  void putObstacle(int row, int col) {
    if (isInGrid(row, col) ) {
      this->obstacleMap[row][col] = false;
    }
  }

  /**
   * Checks whether the given row and column location is within the grid
   * and has no obstacles
   **/
  bool isValidLocation(int row, int col) {
    if (isInGrid(row, col)) {
      return this->obstacleMap[row][col];
    } else {
      return false;
    }
  }

  /**
   * Checks if the given row and col is within the dimensions of the grid
   **/
  bool isInGrid(int row, int col) {
    if (row < 0 || col < 0) {
      return false;
    } else if (row >= getNumRows()) {
      return false;
    } else if (col >= getNumCols()) {
      return false;
    } else {
      return true;
    }
  }

  /**
   * Converts a row to a corresponding row on the grid (i.e. wrapping)
   * Example: row 4 on a 3 row grid would return row 0
   **/
  int convertToGridRow(int row) {
    return (row % this->getNumRows() + this->getNumRows()) % this->getNumRows();
  }

  /**
   * Converts a col to a corresponding col on the grid (i.e. wrapping)
   **/
  int convertToGridCol(int col) {
    return (col % this->getNumCols() + this->getNumCols()) % this->getNumCols();
  }

private:
  /**
   * 2D array, false value indicates spot is taken
   **/
  std::vector< std::vector<bool> > obstacleMap;
};

/**
 * Represents the four cardinal directions
 **/
enum Direction {
  NORTH = 0,
  EAST = 1,
  SOUTH = 2,
  WEST = 3
};

/**
 * Represents a Rover object
 * A Rover has a (row, col) position, a direction, and a grid upon which it sits
 **/
class Rover {
public:
  /**
   * Constructs a rover for a given (row, col) position, a direction, and a grid
   **/
  Rover(int row, int col, Direction dir, Grid grid) {
    // Validate that the given row and col is valid for the grid
    if (grid.isInGrid(row, col)) {
      throw std::runtime_error("Rover cannot be placed here");
    }

    // Set vars
    this->grid = grid;
    this->dir = dir;
    this->setRow(row);
    this->setCol(col);

    // Initialize available movements
    this->initMovementPatternMap();
  }

  /**
   * GETTERS
   **/
  int getRow() { return this->row; }
  int getCol() { return this->col; }
  int getDir() { return this->dir; }


  /**
   * MOVEMENT
   **/

  /**
   * Handles movement when input as a string
   * Characters allowed are 'F', 'B', 'L', 'R'
   **/
  void move(std::string movements) {
    for (char movement : movements) {
      moveHelper(movement);
    }
  }

  /**
   * Handles movement when input as a single character
   * Characters allowed are 'F', 'B', 'L', 'R'
   **/
  void move(char movement) {
    moveHelper(movement);
  }


private:
  /**
   * Current row and col of rover
   **/
  int row, col;

  /**
   * Direction rover is currently facing
   **/
  Direction dir;

  /**
   * Grid that rover is currently on
   **/
  Grid grid = Grid(0,0);

  /**
   * Maps from a cardinal direction to what the corresponding
   * forward movement (as a [row,col] pair) would look like
   **/
  std::vector< std::pair<int,int> > movementPatternMap;

  /**
   * Generates a mapping from cardinal direction to corresponding forward movement
   **/
  void initMovementPatternMap() {
    // Vector should hold the 4 cardinal directions
    movementPatternMap.resize(4);
    movementPatternMap[NORTH] = {1,0};
    movementPatternMap[EAST] = {0,1};
    movementPatternMap[SOUTH] = {-1,0};
    movementPatternMap[WEST] = {0,-1};
  }

  /**
   * Determines the type of move (movement/rotation)
   **/
  void moveHelper(char movement) {
    switch (movement) {
      // Forward
      case 'F': {
        bool isMoveForward = true;
        moveRover(isMoveForward);
        break;
      }
      // Backward
      case 'B': {
        bool isMoveForward = false;
        moveRover(isMoveForward);
        break;
      }
      // Left
      case 'L': {
        bool isRotateLeft = true;
        rotateRover(isRotateLeft);
        break;
      }
      // Right
      case 'R': {
        bool isRotateLeft = false;
        rotateRover(isRotateLeft);
        break;
      }
      default: {
        throw std::runtime_error("Invalid movement");
        break;
      }
    }
  }

  /**
   * Moves the rover forwards/backwards
   **/
  void moveRover(bool isMoveForward) {
    // Get the available movement pattern for the rover's current direction
    std::pair<int, int> movementPattern = movementPatternMap[this->getDir()];

    int newRow, newCol;
    if (isMoveForward) {
      newRow = this->grid.convertToGridRow(this->getRow() + movementPattern.first);
      newCol = this->grid.convertToGridCol(this->getCol() + movementPattern.second);
    } else {
      newRow = this->grid.convertToGridRow(this->getRow() - movementPattern.first);
      newCol = this->grid.convertToGridCol(this->getCol() - movementPattern.second);
    }

    // Verifies that the new row and column have no obstacles placed
    if (this->grid.isValidLocation(newRow, newCol)) {
      this->setRow(newRow);
      this->setCol(newCol);
    } else {
      throw std::runtime_error("Obstacle encountered");
    }
  }

  /**
   * Rotates the rover left/right
   **/
  void rotateRover(bool isRotateLeft) {
    switch (this->getDir()) {
      case NORTH: {
        this->setDir(isRotateLeft ? WEST : EAST);
        break;
      }
      case EAST: {
        this->setDir(isRotateLeft ? NORTH : SOUTH);
        break;
      }
      case SOUTH: {
        this->setDir(isRotateLeft ? EAST : WEST);
        break;
      }
      case WEST: {
        this->setDir(isRotateLeft ? SOUTH : WEST);
        break;
      }
      default: {
        throw std::runtime_error("Cannot rotate");
        break;
      }
    }

  }

  /**
   * SETTERS
   **/
  void setRow(int row) { this->row = row; }
  void setCol(int col) { this->col = col; }
  void setDir(Direction dir) { this->dir = dir; }

};


/**
 * TESTS GO HERE
 **/

// Testing Grid class
TEST_CASE( "Grid Construction Test", "[grid]" ) {
    Grid grid = Grid(1, 4);
    REQUIRE( grid.getNumRows() == 1 );
    REQUIRE( grid.getNumCols() == 4 );
}

TEST_CASE( "Grid Obstacle Test", "[grid]" ) {
    Grid grid = Grid(4, 4);
    grid.putObstacle(1,1);
    grid.putObstacle(2,2);
    REQUIRE( grid.isValidLocation(1,1) == false );
    REQUIRE( grid.isValidLocation(2,2) == false );
    REQUIRE( grid.isValidLocation(0,2) == true );
    REQUIRE( grid.isValidLocation(10,10) == false );
}


// Testing Rover class
TEST_CASE( "Rover Construction Test", "[rover]" ) {
    Rover rov = Rover(1, 4, NORTH, Grid(5, 5));
    REQUIRE( rov.getRow() == 1 );
    REQUIRE( rov.getCol() == 4 );
}

TEST_CASE( "Rover Invalid Construction Test", "[rover]" ) {
    // Rover would not fit on this grid
    REQUIRE_THROWS_AS(Rover(1, 4, NORTH, Grid(1, 1)), std::runtime_error);

    // Nor this one
    REQUIRE_THROWS_AS(Rover(0, 4, NORTH, Grid(1, 4)), std::runtime_error);
}

// Rover Movement TESTS
TEST_CASE( "Rover basic movement Test", "[rover]" ) {
    Rover rov = Rover(0, 0, NORTH, Grid(4, 4));

    rov.move('F');
    // Rover should now be at 1, 0
    REQUIRE( rov.getRow() == 1 );
    REQUIRE( rov.getCol() == 0 );

    rov.move('B');
    // Rover should now be at 0, 0
    REQUIRE( rov.getRow() == 0 );
    REQUIRE( rov.getCol() == 0 );

    // Should wrap back to zero
    rov.move('F');
    rov.move('F');
    rov.move('F');
    rov.move('F');
    REQUIRE( rov.getRow() == 0 );
    REQUIRE( rov.getCol() == 0 );
}

TEST_CASE( "Rover basic obstacle movement Test", "[rover]" ) {
    Grid grid = Grid(4,4);
    // Place obstacles
    grid.putObstacle(1,1);
    grid.putObstacle(2,0);
    Rover rov = Rover(0, 0, NORTH, grid);

    rov.move('F');
    // Rover should now be at 1, 0
    REQUIRE( rov.getRow() == 1 );
    REQUIRE( rov.getCol() == 0 );

    // Error when attempting to move forward
    REQUIRE_THROWS_AS(rov.move('F'), std::runtime_error);

    // Error when attempting to move right
    rov.move('R');
    REQUIRE_THROWS_AS(rov.move('F'), std::runtime_error);
}

TEST_CASE( "Rover basic rotation Test", "[rover]" ) {
    Rover rov = Rover(0, 0, NORTH, Grid(4, 4));

    rov.move('R');
    REQUIRE( rov.getDir() == EAST );

    rov.move('L');
    REQUIRE( rov.getDir() == NORTH );

    rov.move('L');
    rov.move('L');
    REQUIRE( rov.getDir() == SOUTH );

    // Should wrap back to NORTH
    rov.move('L');
    rov.move('L');
    REQUIRE( rov.getDir() == NORTH );
}
