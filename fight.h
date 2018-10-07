// flight.h file
// Declare the class that used in the fianl exam

// Declare a struct contains city name and the pointers
struct flightlist
{
  string city; // The city that served by this company
  flightlist *nextcity;
}; // end struct
typedef flightlist* flightlistPtr; // Define the pointer type

const int max_cityInServed = 100; // The maximum city that this company can serve

class flight
{
 public:
  // Default constructor
  flight();

  // Copy the constructor

  // To test whether the list is empty or not 
  void IsEmpty();

  // Display the information of the entire list
  void DisplayInfo() const;

  // Returns the length of the list
  void ListLength() const;

  // Find and returns a city name
  void FindCity() const;

  // Delete a city from the list
  void DeleteCity();

  // Insert a city into the list but remains to be the sorted order
  void InsertCity();

  // Destrutor
  ~flight();

  
 private:
  int citiesInServed;  // The total number of cities served by the company
  bool found;   // The variable that determine the city 
  string CityList[max_cityInServed];
  flightlistPtr head;
  flightlistPtr ptr;

}
