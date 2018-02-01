#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

// Represents a 2x2 grid of the planet
class Grid {
public:
  Grid(int numRows, int numCols) {
    this->numRows = numRows;
    this->numCols = numCols;
  }
  int getNumRows() { return this->numRows; }
  int getNumCols() { return this->numCols; }

private:
  int numRows, numCols;
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
    if (row > grid.getNumRows() || row < 0) {
      throw std::runtime_error("failed to construct");
    }

    if (col > grid.getNumCols() || col < 0) {
      throw std::runtime_error("failed to construct");
    }

    // Set private vars
    this->grid = grid;
    this->dir = dir;
    this->setRow(row);
    this->setCol(col);
  }

  // GETTERS
  int getRow() { return this->row; }
  int getCol() { return this->col; }

  // SETTERS
  // TODO: Implement wrapping for next two functions
  int setRow(int row) {
    this->row = row;
  }
  int setCol(int col) {
    this->col = col;
  }

private:
  // Current row and col of rover
  int row, col;

  // Direction rover is currently facing
  Direction dir;

  // Grid that rover is currently on
  Grid grid = Grid(0,0);

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


// Testing Rover class
TEST_CASE( "Rover Construction Test", "[rover]" ) {
    Rover rov = Rover(1, 4, NORTH, Grid(5, 5));
    REQUIRE( rov.getRow() == 1 );
    REQUIRE( rov.getCol() == 4 );
}

TEST_CASE( "Rover Invalid Construction Test", "[rover]" ) {
    // Rover would not fit on this grid
    REQUIRE_THROWS_AS(Rover(1, 4, NORTH, Grid(1, 1)), std::runtime_error);
}

TEST_CASE( "Rover setters Test", "[rover]" ) {
    Rover rov = Rover(1, 4, NORTH, Grid(20, 20));
    rov.setRow(11);
    rov.setCol(15);
    REQUIRE( rov.getRow() == 11 );
    REQUIRE( rov.getCol() == 15 );
}

TEST_CASE( "Rover setters wrapping Test", "[rover]" ) {
    Rover rov = Rover(1, 4, NORTH, Grid(10, 10));

    // New co-ordinates are outside of grid!
    // TODO: Co-ordinates should be wrapped.
    rov.setRow(11);
    rov.setCol(15);
    REQUIRE( rov.getRow() == 11 );
    REQUIRE( rov.getCol() == 15 );
}
