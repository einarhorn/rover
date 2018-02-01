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
  int getRow() { return this->row; }


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

TEST_CASE( "Rover Test", "[factorial]" ) {
    Rover rov = Rover();
    REQUIRE( rov.getRow() == 0 );
}
