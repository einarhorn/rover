#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include <vector>

// Represents a 2x2 grid of the planet
class Grid {
public:
  Grid(int numRows, int numCols) {
    this->numRows = numRows;
    this->numCols = numCols;
    this->obstacleMap.assign(numRows, std::vector<bool>(numCols, false));
  }
  int getNumRows() { return this->obstacleMap.size(); }
  int getNumCols() { return this->obstacleMap[0].size(); }

  void putObstacle(int row, int col) {
    if (validLocation(row, col) ) {
      this->obstacleMap[row][col] = true;
    }
  }

  bool obstacleExists(int row, int col) {
    if (validLocation(row, col)) {
      return this->obstacleMap[row][col];
    } else {
      return false;
    }
  }
private:
  int numRows, numCols;
  std::vector< std::vector<bool> > obstacleMap;

  bool validLocation(int row, int col) {
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
};

enum Direction {
  NORTH = 0,
  EAST = 1,
  SOUTH = 2,
  WEST = 3
};

class Rover {
public:
  // Rover Constructor
  Rover(int row, int col, Direction dir, Grid grid) {
    // We only do explicit error checking here
    if (row >= grid.getNumRows() || row < 0) {
      throw std::runtime_error("failed to construct");
    }

    if (col >= grid.getNumCols() || col < 0) {
      throw std::runtime_error("failed to construct");
    }

    // Set private vars
    this->grid = grid;
    this->dir = dir;
    this->setRow(row);
    this->setCol(col);

    // Initialize available movements
    this->initMovementPatternMap();
  }

  // GETTERS
  int getRow() { return this->row; }
  int getCol() { return this->col; }
  int getDir() { return this->dir; }

  // SETTERS
  // TODO: Should these be private??
  void setRow(int row) {
    // Wrap row
    this->row = (row % this->grid.getNumRows() + this->grid.getNumRows()) % this->grid.getNumRows();
  }

  void setCol(int col) {
    // Wrap column
    this->col = (col % this->grid.getNumCols() + this->grid.getNumCols()) % this->grid.getNumCols();
  }

  // MOVEMENT

  /**
   * Handles movement when input as a string
   **/
  void move(std::string movements) {
    for (char movement : movements) {
      // TODO: Implement some sort of error checking
      moveHelper(movement);
    }
  }

  /**
   * Handles movement when input as a single character
   **/
  void move(char movement) {
    // TODO: Implement some sort of error checking
    moveHelper(movement);
  }


private:
  // Current row and col of rover
  int row, col;

  // Direction rover is currently facing
  Direction dir;

  // Grid that rover is currently on
  Grid grid = Grid(0,0);

  // Maps from a cardinal direction to what the corresponding
  // forward movement (as a [row,col] pair) would look like
  std::vector< std::pair<int,int> > movementPatternMap;

  // Generates a mapping from cardinal direction to corresponding forward movement
  void initMovementPatternMap() {
    // Vector should hold the 4 cardinal directions
    movementPatternMap.resize(4);
    movementPatternMap[NORTH] = {1,0};
    movementPatternMap[EAST] = {0,1};
    movementPatternMap[SOUTH] = {-1,0};
    movementPatternMap[WEST] = {0,-1};
  }

  void moveHelper(char movement) {
    switch (movement) {
      case 'F': {
        bool isMoveForward = true;
        moveRover(isMoveForward);
        break;
      }
      case 'B': {
        bool isMoveForward = false;
        moveRover(isMoveForward);
        break;
      }
      case 'L': {
        bool isRotateLeft = true;
        rotateRover(isRotateLeft);
        break;
      }
      case 'R': {
        bool isRotateLeft = false;
        rotateRover(isRotateLeft);
        break;
      }
      default: {
        // TODO: Some sort of error here
        return;
      }
    }
  }

  void moveRover(bool isMoveForward) {
    // Get the available movement pattern for the rover's current direction
    std::pair<int, int> movementPattern = movementPatternMap[this->getDir()];

    // TODO: Rover needs to check if obstacle exists!
    if (isMoveForward) {
      this->setRow(this->getRow() + movementPattern.first);
      this->setCol(this->getCol() + movementPattern.second);
    } else {
      this->setRow(this->getRow() - movementPattern.first);
      this->setCol(this->getCol() - movementPattern.second);
    }
  }

  void rotateRover(bool isRotateLeft) {
    // TODO: Can we improve this?
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
        break;
      }
    }

  }

  void setDir(Direction dir) {
    this->dir = dir;
  }

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
    REQUIRE( grid.obstacleExists(1,1) == true );
    REQUIRE( grid.obstacleExists(2,2) == true );
    REQUIRE( grid.obstacleExists(0,2) == false );
    REQUIRE( grid.obstacleExists(10,10) == false );
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

TEST_CASE( "Rover setters Test", "[rover]" ) {
    Rover rov = Rover(1, 4, NORTH, Grid(20, 20));
    rov.setRow(11);
    rov.setCol(15);
    REQUIRE( rov.getRow() == 11 );
    REQUIRE( rov.getCol() == 15 );
}

TEST_CASE( "Rover setters wrapping Test", "[rover]" ) {
    Rover rov = Rover(1, 3, NORTH, Grid(4, 4));

    // New co-ordinates are outside of grid!
    // Should wrap back to zero
    rov.setRow(4);
    rov.setCol(4);
    REQUIRE( rov.getRow() == 0 );
    REQUIRE( rov.getCol() == 0 );

    // Wrap in the negative direction
    rov.setRow(-1);
    rov.setCol(-1);
    REQUIRE( rov.getRow() == 3 );
    REQUIRE( rov.getCol() == 3 );
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

    // TODO: Need error message
    rov.move('F');
    // Rover should still be at 1, 0
    REQUIRE( rov.getRow() == 1 );
    REQUIRE( rov.getCol() == 0 );

    // TODO: Error message
    rov.move('R');
    rov.move('F');
    // Rover should still be at 1, 0
    REQUIRE( rov.getRow() == 1 );
    REQUIRE( rov.getCol() == 0 );
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
