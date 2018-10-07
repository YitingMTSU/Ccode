#include"Vehicle.h"
#include<iostream>
#include<iomanip>
#include<math.h>

using namespace std;

const int moveInfo = 8; //the format to print the infomation
const int otherInfo =15;
const int customerInfo = 12;
const int totalInfo = 10;

Vehicle::Vehicle()
{
  m_x = 0; //Initially all cabs are parked at the location (0, 0)
  m_y = 0; 
  m_passengerNum = 0; //with no passenger on board
  m_fuelInTank = 20; //It has full tank (20 gallons) of fuel.

  m_totalMilesTraveled = 0; //Initially the distance at 0 and consumed fuel 0
  m_totalFuelConsumed = 0;
}

void Vehicle::pickupCustomer(int number, int xo, int yo)
{
  moveTo(xo, yo);
  loadPassenger(number);
  displayInfo();
}

void Vehicle::dropCustomer(int xd, int yd)
{
  moveTo(xd, yd);
  unloadPassenger();
  displayInfo();
}

void Vehicle::moveTo(int x, int y)
{
  cout<<setw(moveInfo)<<"-- "<<"taxi is moving from ("<<m_x<<", "<<m_y<<") to ("<<x<<", "<<y<<")"<<endl; //print the moving information
  float distance; //the distance from xo,yo to x,y
  distance = sqrt(pow(x - m_x, 2) + pow(y - m_y, 2));
  cout<<setw(otherInfo)<<"-- "<<distance<<" miles to travel"<<endl;//print the distance to travel
  m_x = x;
  m_y = y;
  cout<<setw(otherInfo)<<"-- "<<"Remaining gas: "<<m_fuelInTank<<endl;
  float fuelNeed; //the fuel that needed in this travelling
  fuelNeed = distance /(GasMileage * (1 - GasTax * m_passengerNum));
  //if the gas is not enough, fill up the gas
  if (fuelNeed > m_fuelInTank)
    {
      fillUpGas();
    }
  m_fuelInTank -= fuelNeed;
  cout<<setw(otherInfo)<<"-- "<<"Gas used: "<<fuelNeed<<endl;
  m_totalMilesTraveled += distance;
  m_totalFuelConsumed += fuelNeed;
}

//load the passenger
void Vehicle::loadPassenger(int number)
{
  m_passengerNum = number;
  cout<<setw(customerInfo)<<"-- "<<"Loading "<<m_passengerNum<<" passengers"<<endl;
}

//make the passenger = 0
void Vehicle::unloadPassenger(void)
{
  cout<<setw(customerInfo)<<"-- "<<"Unloading "<<m_passengerNum<<" passengers"<<endl;
  m_passengerNum = 0;
}

//print the infomation about the total distance and gas
void Vehicle::displayInfo() const
{
  cout<<setw(totalInfo)<<"-- "<<"TotalMiles:    "<<m_totalMilesTraveled<<endl;
  cout<<setw(totalInfo)<<"-- "<<"TotalGas:    "<<m_totalFuelConsumed<<endl;
  cout<<endl;
  cout<<endl;
}

void Vehicle::fillUpGas(void)
{
  m_fuelInTank = 20;
  cout<<setw(otherInfo+12)<<"Fill up gas!"<<endl;//print the fill up the gas
}
