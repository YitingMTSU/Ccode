/*
  PROGRAMMER: Yiting Wang 
  CLASS: CSCI 6020 
  DUE DATE: 11/27/2016
  INSTRUCTOR: Rafet Al-Tobasei
  Purpose: Calculate a taxi that serves customers around town and generate a re           port that summarizes the operation
  Input: The customer requests in the same order as they are listed in the data         file. Each line of the data file has the following information in the          given order:
         customer_id    x-origin     y-origin    x-destination     y-destinatio         n      number-of-passengers
  Output: Print information for the following activities. All information shoul          d be clear and accurate.
         1.receiving a customer request,  print customer id, # of passengers, l          cation of customers, and destination of customers
         2. passenger loading: print the number of customer on board
         3.passenger unloading: print the number of customers unloaded
         4.whenever the cab moves one location to another location: print the f           ollowing information
           4.1 gas consumed during the trip
           4.2 miles of the trip
           4.3 gas remained after the trip
           4.4 total miles traveled so far after the trip
           4.5 total gas consumed so far after the trip
   Usage: Use command g++ ola4.cc Vehicle.cc -o aa ola4 and then ./aa request.d          at
*/

#include<iostream>
#include<fstream>
#include<string.h>
#include"Vehicle.h"

using namespace std;

int main( int argc, char* argv[])
{
  string customer;
  int xo,yo,xd,yd,np;
  Vehicle myCap;
  ifstream myIn;
  myIn.open(argv[1]);
  while(myIn.peek()!=EOF)
    {
      myIn>>customer>>xo>>yo>>xd>>yd>>np;
      myIn.ignore(256,'\n');
      cout<<"========================================================================"<<endl;
      cout<<customer<<" has "<<np<<" passengers "
	  <<"need service from ("<<xo<<", "<<yo<<")"<<" to ("<<xd<<", "
	  <<yd<<")"<<endl;
      myCap.pickupCustomer(np,xo,yo);
      myCap.dropCustomer(xd,yd);
    }
}
