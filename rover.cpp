#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"


enum Direction {
  NORTH = 0,
  EAST = 1,
  SOUTH = 2,
  WEST = 3
};

class Rover {
public:
  // Rover Constructor
  Rover(int row, int col) {
    this->row = row;
    this->col = col;
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


};


// Unit Testing Example
unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}


/**
 * TESTS GO HERE
 **/

// Unit Testing Example
TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}

TEST_CASE( "Rover Construction Test", "[factorial]" ) {
    Rover rov = Rover(1, 4);
    REQUIRE( rov.getRow() == 1 );
    REQUIRE( rov.getCol() == 4 );
}
