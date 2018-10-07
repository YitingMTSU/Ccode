#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<iomanip>
#include"flight.h"

using namespace std;

// Read the data from the file and create the pointer based list
void ReadData(ifstream& infile, int& number, flightlistPtr& head);

// Sort the city array
void SortCity(flightlistPtr head, int number);

int main(int argc, char* argv[])
{
  
  ifstream myIn; // File handle
  int cityNum, i; // i is number of the deletion opertors 
  flightlistPtr head;
  head = NULL;
  myIn.open(argv[1]);
  ReadData(myIn, cityNum, head); // read the data
  SortCity(head, cityNum); // sort the city list in ascending order
  flight FlyUs(head, cityNum);//copy the class
  for(i = 0; i < 3; i++) // Do three deletions
    {
      FlyUs.DeleteCity();
    }
  FlyUs.DisplayInfo();// Display the information after deletions
}

// Read the data from a file
void ReadData(ifstream& infile, int& number, flightlistPtr& head)
{
  flightlistPtr cur;
  int j = 1;
  head = new flightlist;
  cur = head;
  while(infile.peek()!=EOF)
    {
      if (j == 1)
	{
	  infile>>number;
	  infile.ignore(256,'\n');
	}
      j++;
      getline(infile, cur->city);
      cur->nextcity = new flightlist;
      cur = cur->nextcity;
    }
  cur = NULL;
}
       
// Sort the city array
void SortCity(flightlistPtr head, int number)
{
  flightlistPtr pre, cur;
  string tmp_string; // temperature string to swap 
  for(pre = head; pre != NULL; pre = pre->nextcity)
    {
      for (cur = pre->nextcity; cur != NULL; cur = cur->nextcity)
	{
	  if (pre->city > cur->city)
	    {
	      tmp_string = pre->city; // swap the items
	      pre->city = cur->city;
	      cur->city = tmp_string;
	    }
	}
    }
}



