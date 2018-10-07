/*
PROGRAMMER:        Yiting
CLASS:             CSCI 6020
DUE DATA:          11/16/2016
INSTRUCTOR:        Rafet Al-Tobasei
Purpose:           To read the data from the file and create a menu to select opinions in different usages.
INPUT:             The file is "booklist.txt"
Expected results:  When the visitors input the character, the computer can do the corresponding tasks for the visitors. The options are following:
OPTIONS                USAGES
i                      Insert new item "book record"
d                      Delete an existing item "book record" 
a                      Search a book by authoe name
t                      Search a book by title
s                      Sorting by Book title
p                      Display the whole list
q                      Quit the program
How to use:        Use command "g++ booklist.cpp -o booklist". And then use command "./booklist" to excute the file. Then there will have some tips to tell you what should be input.
 */
#include<iostream> //Header file for input/output
#include<fstream>
#include<string>
#include<cassert>
#include<iomanip>

using namespace std;

//Declare the global variable
const int MAX_BOOKS = 500; //Maximum number of books to be stored
const int TitleWidth = 80; //Length of the title
const int AuthorWidth = 30; //Length of the author
const int DataWidth = 10;  //Length of the data
const int lenOption = 6;
const int lenDescrib = 35;

struct BookType
{
  string title;
  string author;
  int data;
};

//Declare all the functions here
void EnterBooksInLibrary(ifstream& infile, BookType [], int &number);
//Sort all books by title
void SortByTitle(BookType [], int number);
//Search books by this author
void ListBooksByThisAuthor(BookType [], int number, string thisAuthor);
//Search books by this title
void ListBooksByThisTitle(BookType [], int number, string thisTitle);
//Display all the information about each book in the library Fields
void DisplayBooks(BookType [], const int number);
//Insert new item to the booklist
void InsertNewItem(BookType [], int &number);
//Delete item
void DeleteItem(BookType [],  int &number);


int main(int argc, char* argv[])
{
  int numberBooksInLibrary = 0; 
  BookType library[MAX_BOOKS];
  string thisAuthor, dataFile, thisTitle;
  ifstream  myIn;
  char OPTION;
  bool keep = true;    
  
  myIn.open(argv[1]);
  assert(myIn);
  EnterBooksInLibrary(myIn, library, numberBooksInLibrary);
  myIn.close();
  while(keep){
    //Print the options
    cout<<"The following choices are available"<<endl;
    cout<<right<<setw(lenOption)<<"i"<<"      "<<left<<setw(lenDescrib)<<"Insert new item \"book record\""<<endl;
    cout<<right<<setw(lenOption)<<"d"<<"      "<<left<<setw(lenDescrib)<<"Delete an existing item \"book record\""<<endl;
    cout<<right<<setw(lenOption)<<"a"<<"      "<<left<<setw(lenDescrib)<<"Search a book by author name"<<endl;
    cout<<right<<setw(lenOption)<<"t"<<"      "<<left<<setw(lenDescrib)<<"Search a book by title"<<endl;
    cout<<right<<setw(lenOption)<<"s"<<"      "<<left<<setw(lenDescrib)<<"Sorting by Book title"<<endl;
    cout<<right<<setw(lenOption)<<"p"<<"      "<<left<<setw(lenDescrib)<<"Display the whole list"<<endl;
    cout<<right<<setw(lenOption)<<"q"<<"      "<<left<<setw(lenDescrib)<<"Quit the program"<<endl;
    //Choose options
    cout<<"Please choose one option what you want:"<<endl;
    cin>>OPTION;
    //Make a choose
    switch(OPTION){
      case('i'):
	InsertNewItem(library, numberBooksInLibrary);
	break;
      case('d'):
        DeleteItem(library,  numberBooksInLibrary);
        break;
      case('a'):
	cout<<"Please enter the author who you want to search:"<<endl;
	cin>>thisAuthor;
        ListBooksByThisAuthor(library, numberBooksInLibrary, thisAuthor);
        break;
      case('t'):
	cout<<"Please enter the title that you want to search:"<<endl;
	cin>>thisTitle;
        ListBooksByThisTitle(library, numberBooksInLibrary, thisTitle);
        break;
      case('s'):
        SortByTitle(library, numberBooksInLibrary);
        break;
      case('p'):
        DisplayBooks(library, numberBooksInLibrary);
        break;
      case('q'):
        keep = false;
        break;
    }
  }
  return 0;
}

void EnterBooksInLibrary(ifstream& infile, BookType library[], int &number){
  //Read the file
  int index = 0;
  string str;
  while(infile.peek() != EOF)
    {
      //getline(infile, library[index].title);
      getline(infile, str);
      //str.erase( std::remove(str.begin(), str.end(), '\r'), str.end());
      //str.erase(str.end()-4);
      library[index].title = str;
      //getline(infile, library[index].author);
      getline(infile, str);
      //str.erase(str.end()-4);
      //str.erase( std::remove(str.begin(), str.end(), '\r'), str.end());
      library[index].author = str;
      infile>>library[index].data;
      infile.ignore(100,'\n');
      index++;
      number++;
    }
  int i;
  /* for(i = 0; i < number; i++)
    {
      cout<<library[i].title<<library[i].author<<endl;
      cout<<library[i].data<<endl;
      }*/
}

void InsertNewItem(BookType library[], int &number){
  cout<<"Please enter the title of the book that you want to insert"<<endl;
  cin>>library[number].title; //Insert the title of new book
  cout<<"Please enter the author of this book"<<endl;
  cin>>library[number].author;//Insert the author of new book
  cout<<"Please enter the data of this book"<<endl;
  cin>>library[number].data;//Insert the data of new book
  number++;
}

void DeleteItem(BookType library[],  int &number){
  string TargetTitle; //The title that want to delete
  cout<<"Please enter the title of the book that you want to Delete"<<endl;
  cin>>TargetTitle;
  int i;
  //Start to find the target
  for(i = 0; i < number; i++)
    {
      if (library[i].title == TargetTitle)
	{
	  library[i].title = library[number - 1].title;
	  library[i].author = library[number - 1].author;
	  library[i].data = library[number - 1].data;
	}
    }
  number--;//Decrease the number of books in the library
}

void ListBooksByThisAuthor(BookType library[], int number, string thisAuthor){
  int i;
  int count;
  cout<<"The result is following:"<<endl;
  cout<<left<<setw(TitleWidth)<<"Title"<<left<<setw(AuthorWidth)<<"Author"<<left<<setw(DataWidth)<<"Data"<<endl;
  cout<<"=================================================================================================================================================="<<endl;
  for (i=0; i<number; i++)
    {
      if (library[i].author == thisAuthor)
	{
	  cout<<left<<setw(TitleWidth)<<library[i].title<<"|"<<left<<setw(AuthorWidth)<<library[i].author<<"|"<<left<<setw(DataWidth)<<library[i].data<<endl;
	  count++;
	}
    }
  cout<<"There are "<<count<<" solutions!"<<endl;
}

void ListBooksByThisTitle(BookType library[], int number, string thisTitle){
  int i;
  cout<<"The result is following:"<<endl;
  cout<<left<<setw(TitleWidth)<<"Title"<<left<<setw(AuthorWidth)<<"Author"<<left<<setw(DataWidth)<<"Data"<<endl;
  cout<<"=================================================================================================================================================="<<endl;
  for (i=0; i<number; i++)
    {
      if (library[i].title == thisTitle)
	{
	  cout<<left<<setw(TitleWidth)<<library[i].title<<"|"<<left<<setw(AuthorWidth)<<library[i].author<<"|"<<left<<setw(DataWidth)<<library[i].data<<endl;
	}
    }
}

void DisplayBooks(BookType library[], const int number){
  int i;
  cout<<left<<setw(TitleWidth)<<"Title"<<left<<setw(AuthorWidth)<<"Author"<<left<<setw(DataWidth)<<"Data"<<endl;
  cout<<"=================================================================================================================================================="<<endl;
  for(i=0; i<number; i++)
    {
      cout<<left<<setw(TitleWidth)<<library[i].title<<"|"<<left<<setw(AuthorWidth)<<library[i].author<<"|"<<left<<setw(DataWidth)<<library[i].data<<endl;
    }
  cout<<"There are "<<number<<" books in the library!"<<endl;
}

void SortByTitle(BookType library[], int number){
  int i,j;
  string temp_title, temp_author;
  int temp_data;
  for(i=0; i<number-1; i++)
    {
      for(j=i+1; j<number; j++)
	{
	  if(library[i].title > library[j].title)
	    {
	      //swap the title
	      temp_title = library[i].title;
	      library[i].title = library[j].title;
	      library[j].title = temp_title;
	      //swap the author
	      temp_author = library[i].author;
	      library[i].author = library[j].author;
	      library[j].author = temp_author;
	      //swap the data
	      temp_data = library[i].data;
	      library[i].data= library[j].data;
	      library[j].data= temp_data;
	    }
	}
    }
}
	      
	      

      
	  
