#include<iostream>

const float FuelTankCapacity = 20; //Full tank is 20 gallons
const float GasMileage = 30.0;   // gas mileage is 30 mpg
const float GasTax = 0.05;   // each passenger reduce gas milage by 5%

class Vehicle
{
 public:
  //Vehicle constructor
  //Post-condition: data members are initialized to 0
  Vehicle();
  //add comments, pre-condition and post condition
  void pickupCustomer(int number, int xo , int yo);


  // add comments, pre-condtion and post-condtion
  void dropCustomer(int xd, int yd);
private:
  // move the vehicle to the specified location
  // post-condition: cab is moved from the current location to (x, y)
  //      location and the activity is printed/reported
  //      if cab does not have enough gas, it will be filled up
  void moveTo( int x, int y);

  // load passengers into the vehicle
  // pre-condition: no passenger in the cab
  // post-condition: passengers are in the cab
  void loadPassenger( int number );

  // unload the passengers at the destination
  // pre-condition: passengers are in the cab
  // post-condition: no passenger in the cab
  void unloadPassenger( void );

  // display total miles travel so far and total gas consumed
  void displayInfo( ) const;

  // fill up the tank
  // post-condition: fuel is filled to the tank capacity
  void fillUpGas( void );

 private:
  int     m_x;                    //x-coordinate of the location
  int     m_y;                    //y-coordinate of the location
  int     m_passengerNum;         //number of passengers in the cab
  float   m_fuelInTank;           //fuel in the tank

  float   m_totalMilesTraveled;   //total miles traveled so far
  float   m_totalFuelConsumed;    //total fuel consumed so far
};
