#include<iostream>
using namespace std;

int main()
{
  int day, month, year, week;
  int D, C, remainder, f;
  cout << "Welcome!" << endl;
  cout << "Just tell me your birth date, and I will tell which day of the week you were born." << endl;
  cout << "Let's get started ..." << endl;
  cout << "Please tell me your birth date, in the form of: month day year" << endl;
  cin >> month >> day >> year;
  if ((year < 0) || (year > 2016))
    {
      cout << "Your input value is not correct" << endl;
      cout << "The computation is not carried out" << endl;
      return 0;
    }
  if ((month < 1) || (month > 12))
    {
      cout << "Your input value is not correct" << endl;
      cout << "The computation is not carried out" << endl;
      return 0;
    }
  if ((day < 1) || (day > 31))
    {
      cout << "Your input value is not correct" << endl;
      cout << "The computation is not carried out" << endl;
      return 0;
    }
  if (year % 100 == 0)
    {
      if (year % 400 == 0)
	{
	  cout << "This year is leap year" << endl;
	}
      else
	cout << "This year is not the leap year" << endl;
    }
  else
    { if (year % 4 == 0)
	{
	  cout << "This year is a leap year" << endl;
	}
      else
	{
	  cout << "This year is not a leap year" << endl;
	}
    }
  if ((month == 1) || (month == 2))
    {
      month = month + 10;
      year = year - 1;
    }
  else
    month = month -2;
  D = year % 100;
  C = year / 100;
  f =  day + (13 * month -1) / 5 + D + D/4 + C/4 - 2 * C;
  if (f % 7 < 0)
    remainder = f % 7 + 7;
  else
    remainder = f % 7;
  switch (remainder)
    {
    case(0):
      cout << "You were born on Sunday." << endl;
      break;
    case(1):
      cout << "You were born on Monday." << endl;
      break;
    case(2):
      cout << "You were born on Tuesday." << endl;
      break;
    case(3):
      cout << "You were born on Wednesday." << endl;
      break;
    case(4):
      cout << "You were born on Thursday." << endl;
      break;
    case(5):
      cout << "You were born on Friday." << endl;
      break;
    case(6):
      cout << "You were born on Saturday." << endl;
      break;
    }
  return 0;

}
