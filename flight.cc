#include"flight.h"
#include<string>
#include<iostream>
#include<iomanip>

using namespace std;

// Default constructor
flight::flight()
{
  head = NULL;  // Initialize that there is no city in the list
  citiesInServed = 0; 
}

// Copy constructor
flight::flight(flightlistPtr headPosition, int num)
{
  head = headPosition;
  citiesInServed = num;
}

// Destructor
flight::~flight()
{
  head = NULL;
  citiesInServed = 0;
}

// Returns whether the list is empty
void flight::IsEmpty()
{
  if (head = NULL)
    {
      cout<<"The flight list is empty!"<<endl;
    }
  else
    {
      cout<<"There are "<<citiesInServed<<" in the flight list."<<endl;
    }
}

// Print the entire list
void flight::DisplayInfo() const
{
  flightlistPtr cur;
  cout<<"There are "<<citiesInServed<<" in the flight list."<<endl;
  cout<<"And the list are following:"<<endl;
  cur = head;
  while(cur != NULL)
    {
      cout<<cur->city<<endl;
      cur = cur->nextcity;
    }
}

// Returns the length of the list
void flight::ListLength() const
{
  cout<<"The length of the list is "<<citiesInServed<<endl;
}

// Find and returns city name
void flight::FindCity() const
{
  string cityName;
  flightlistPtr cur;
  int i;
  // Prompt the user input the city name
  cout<<"Please enter the whole city name that you want to find:"<<endl;
  cin>>cityName;
  cur  = head;
  i = -1;
  while (cur != NULL)
    {
      if (cityName == cur->city)
	{
	  cout<<"This city is found."<<endl;
	  cur = NULL;
	}
      i++;
    }
  if (i == citiesInServed)
    {
      cout<<"This city is not served."<<endl;
    }
}

// Delete city
void flight::DeleteCity()
{
  flightlistPtr cur, pre;
  int i;
  string DeleteCityName;
  cur  = head;
  cout<<"Please enter the city name that you want to delete"<<endl;
  cin>>DeleteCityName;
  cur = head;
  i = -1;
  // Deal with the list if the first item of flight list is the object
  while(cur != NULL)
    {
      if (cur->city == DeleteCityName)
	{
	  pre->nextcity = cur->nextcity;
	  cur->nextcity = NULL;
	  citiesInServed--;
	  cur = NULL;
	  cout<<"You have successfully deleted the city: "<<DeleteCityName<<endl;	 
	}
      else
	{
	  i++;
	  pre = cur;
	  cur = cur -> nextcity;
	}
    }
  // Deletion is not excuted
  if (i == citiesInServed)
    {
      cout<<"This city does not exit. Deletion can not be performed."<<endl;
    }
}

// Insert city
void flight::InsertCity()
{
  flightlistPtr pre, cur, newPtr;
  string InsertCityName;
  cur = head;
  cout<<"Please enter the city name that you want to insert:"<<endl;
  cin>>InsertCityName;
  if (head = NULL)
    {
      head = new flightlist;
      head->city = InsertCityName;
      head->nextcity = NULL;
    }
  else
    {
      for(pre = NULL, cur = head; cur != NULL; pre = cur, cur = cur->nextcity)
	{
	  if(InsertCityName < cur->city)
	    {
	      newPtr = new flightlist;
	      pre -> nextcity = newPtr;
	      newPtr->nextcity = cur;
	    }
	}
    }
  citiesInServed++;
}
	      
	      
	  
	  
