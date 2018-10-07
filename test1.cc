/* Yiting Wang
   COMS7100
   Project1 
   Content: Use different method to fit the data
*/
   
#include<iostream>
#include<iomanip>
#include<string.h>
#include<cmath>
#include<vector>
#include<fstream>

using namespace std;

const float R = 8.31447;
// Choose from Table 8 unit: Pa m^3 K^-1 mol^-1
const int MaxSize = 1000; // For DataX and DataY
const float lamda = 0.001; // step size

struct FileInf
{
  int dataSize;
  float DataX[MaxSize];
  float DataY[MaxSize];
  float Initial[2];
  string Type;
  float temp;
  // float answer[];
  string title;
  string unit;
};
  
// Declare the function

void ReadList(string filename, FileInf& File);
// Read the data from the file

void vdw(FileInf File);
// The van der Waals equation

void virial(FileInf File);
// The virial equation

void rk(FileInf File);
// The Redlich-Kwong equation

void dieterici(FileInf File);
// The Dieterici equation

void berthelot(FileInf File);
// The Berthelot equation

void DisplayInformation();
// Display the fitting result and other information, like correlation coefficient, Rsuqare and so on.

int CountLines(string fileName);
// Count the lines of the file

int main()
{
  const int num = 5; // Number of the files
  int i;
  //string filename;
  ifstream infile; // The handler of the file
  FileInf File[num]; 
  // Declare the string vector
  std::string fileName[] = {"ar_130k_berthelot.txt","ar_130k_vdw.txt","ar_130k_dieterici.txt","ar_130k_rk.txt","ar_130k_virial.txt"};
  vector<std::string> vec;
  vec = vector<std::string> (fileName, fileName + (sizeof(fileName) / sizeof(std::string)));
  for(i = 0; i < num; i++)
    {
      cout<<"The No."<<i+1<<" file is "<<fileName[i]<<endl;
      //cout<<CountLines(fileName[i])<<endl;
      //filename = fileName[i];
      //infile.open(fileName[i].c_str());
      ReadList(fileName[i], File[i]);
      //cout<<i<<endl;
      //cout<<File[i].Type<<endl;
      if(File[i].Type == "berthelot")
	{
	  //cout<<"1"<<endl;
	  // berthelot(File[i]);
	  cout<<endl;
	}
      else if(File[i].Type == "vdw")
	{
	  //cout<<"2"<<endl;
	  // vdw(File[i]);
	  cout<<endl;
	}
      else if(File[i].Type == "dieterici")
	{
	  //cout<<"3"<<endl;
	  //dieterici(File[i]);
	  cout<<endl;
	}
      else if(File[i].Type == "rk")
	{
	  //cout<<"4"<<endl;
	  //rk(File[i]);
	  cout<<endl;
	}
      else if(File[i].Type == "virial")
	{
	  //cout<<"5"<<endl;
	  virial(File[i]);
	  cout<<endl;
	}
      else
	cout<<"Error!"<<endl;
      
      //infile.close();
    }
  return 0;
}

void ReadList(string filename, FileInf& File){
  int Num = 0;
  int j = 0;
  int FileLines; // The lines of the file
  ifstream infile;
  string type, line1, testline;
  FileLines = CountLines(filename);
  File.dataSize = FileLines - 5; // The data size is number of lines minus 5
  //cout<<File.dataSize<<endl;
  infile.open(filename.c_str());
    
  for (Num = 0;Num < FileLines; Num++)
    {
      /*cout<<Num<<endl;
      cout<<"The title is "<<File.title<<endl;
      //cout<<"The right answer will be: a = "<<File.answer[0]<<" b = "<<File.answer[1]<<endl;
      cout<<"The method is "<<File.Type<<endl;
      cout<<"The initial number will be a = "<<File.Initial[0]<<" b = "<<File.Initial[1]<<endl;
      cout<<"The temperature is "<<File.temp<<endl;
      cout<<"The unit is "<<File.unit<<endl;*/
      
      switch (Num)
      {
      case 0:
	getline(infile, File.title);
	break;
      case 1:
	getline(infile, File.Type,'\n');
	break;
      case 2:
	infile>>File.Initial[0];
	infile.ignore(1,'.');
	infile>>File.Initial[1];
	infile.ignore(256,'\n');
	break;
      case 3:
	infile.ignore(10,' ');
	infile>>File.temp;
	infile.ignore(256,'\n');
	break;
      case 4:
	getline(infile, File.unit);
	break;
      defalut:
	/*infile>>File.DataX[j]>>File.DataY[j];
	cout<<File.DataX[j]<<File.DataY[j]<<endl;
	infile.ignore(256,'\n');
	j++;*/   
	break;
      }
      if (Num > 4)
	{
	  infile>>File.DataX[j]>>File.DataY[j];
          infile.ignore(256,'\n');
          j++;
	}      
    }
  cout<<"After reading"<<endl;
  cout<<"The title is "<<File.title<<endl;
  //cout<<"The right answer will be: a = "<<File.answer[0]<<" b = "<<File.answer[1]<<endl;
  cout<<"The method is "<<File.Type<<endl;
  cout<<"The initial number will be a = "<<File.Initial[0]<<" b = "<<File.Initial[1]<<endl;
  cout<<"The temperature is "<<File.temp<<" K "<<endl;
  cout<<"The unit is "<<File.unit<<endl;
  //cout<<"The data is:"<<endl;
  //for(j = 0;j<File.dataSize;j++)
  // {
  //  cout<<" "<<File.DataX[j]<<"    "<<File.DataY[j]<<endl;
  // }
  
}
  
int CountLines(string fileName){
  int NumLines = 0;
  ifstream myIn;
  string tempString;
  myIn.open(fileName.c_str());
  while(myIn.peek() != EOF)
    {
      getline(myIn, tempString);
      NumLines++;
    }
  myIn.close();
  return NumLines;
}

void berthelot(FileInf File){
  // p = RT/(V-b) - a/(TV^2)
  float sa = 0; // Error function
  int N = File.dataSize; // Data size
  int i,j = 0;
  float T = File.temp; // Temperature
  float f; // value of the function
  float a = File.Initial[0];
  float b = File.Initial[1];
  float beta[2];
  float a_new,b_new;
  float df1,df2;
  float delta_y;
  float HesMa[4],HesMa_new[4]; // Hessian matrix
  float C[4]; // alpha^-1
  float delta_a[2];
  float ad_RR,ss_tot,y_mean,rho,sample_var,std_a,std_b,RR;
  float temp_err,temp_abs,R_factor,temp;
  float sa_new,sa_old;
  float lamda1 = lamda; // step size
  
  // Unify the unit
  // unit is dm^3/mol atm convert to m^3/mol pa
  for(i = 0; i<N; i++)
    {
      File.DataX[i] = File.DataX[i]/1000;
      File.DataY[i] = File.DataY[i]*101325;
    }
  
  // calculate error function
  for(i = 0; i<N; i++)
    {
      f = R*T/(File.DataX[i]-b) - a/(T*File.DataX[i]*File.DataX[i]);
      delta_y = File.DataY[i] - f;
      sa = sa + delta_y * delta_y;
    }
  
  // df/da = -1/T/V^2
  // df/db = R*T/(V-b)^2
  // calculate beta vector
  beta[0] = 0;
  beta[1] = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T/(File.DataX[i]-b) - a/(T*File.DataX[i]*File.DataX[i]);
      delta_y = File.DataY[i] - f;
      df1 = -1/T/File.DataX[i]/File.DataX[i];
      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b);
      beta[0] = beta[0] + delta_y*df1;
      beta[1] = beta[1] + delta_y*df2;
    }

  // calculate Hessian matrix
  HesMa[0] = 0;
  HesMa[1] = 0;
  HesMa[2] = 0;
  HesMa[3] = 0;
  for(i = 0; i<N; i++)
    {
      df1 = -1/T/File.DataX[i]/File.DataX[i];
      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b);
      HesMa[0] = HesMa[0] + df1*df1;
      HesMa[1] = HesMa[1] + df1*df2;
      HesMa[2] = HesMa[2] + df2*df1;
      HesMa[3] = HesMa[3] + df2*df2; 
    }

  // Modify the matrix
  HesMa_new[0] = HesMa[0] * (1 + lamda1);
  HesMa_new[1] = HesMa[1];
  HesMa_new[2] = HesMa[2];
  HesMa_new[3] = HesMa[3] * (1 + lamda1);

  // Solve linear equation
  delta_a[0] = (beta[0]*HesMa_new[3]-beta[1]*HesMa_new[1])/(HesMa_new[0]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
  delta_a[1] = (beta[1]*HesMa_new[0]-beta[0]*HesMa_new[2])/(HesMa_new[0]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);

  // Evaluate S(a+delta_a)
  a_new = a + delta_a[0];
  b_new = b + delta_a[1];
  sa_new = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T/(File.DataX[i]-b_new) - a_new/(T*File.DataX[i]*File.DataX[i]);
      delta_y = File.DataY[i] - f;
      sa_new = sa_new + delta_y * delta_y;
    }
  sa_old = sa;
  
  //cout<<sa_new<<endl;
  //cout<<sa_old<<endl;
  //cout<<abs(sa_new-sa_old)<<endl;
  // compare sa_new and sa_old
  while(abs(sa_new-sa_old)>=1e-5)
    {
      //cout<<"THE STEP  "<<j<<endl;
      //j++;
      //cout<<"error "<<sa_new<<endl;
      //cout<<"a = "<<a<<endl;
      //cout<<"b = "<<b<<endl;
      if(sa_new>sa_old)
	{
	  lamda1 = 10*lamda1;
	  // Modify the matrix
	  HesMa_new[0] = HesMa[0] * (1 + lamda1);
	  HesMa_new[3] = HesMa[3] * (1 + lamda1);
	  // Solve linear equation
	  delta_a[0] = (beta[0]*HesMa_new[3]-beta[1]*HesMa_new[1])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  delta_a[1] = (beta[1]*HesMa_new[0]-beta[0]*HesMa_new[2])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  // Evaluate S(a+delta_a)
	  a_new = a + delta_a[0];
	  b_new = b + delta_a[1];
	  sa_new = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T/(File.DataX[i]-b_new) - a_new/(T*File.DataX[i]*File.DataX              [i]);
	      delta_y = File.DataY[i] - f;
	      sa_new = sa_new + delta_y * delta_y;
	    }
	}
      else
	{
	  lamda1 = lamda1/10;
	  // use a+delta_a instead of a
	  a = a_new;
	  b = b_new;
	  sa_old = sa_new;
	  // calculate beta vector
	  beta[0] = 0;
	  beta[1] = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T/(File.DataX[i]-b) - a/(T*File.DataX[i]*File.DataX[i]);
	      delta_y = File.DataY[i] - f;
	      df1 = -1/T/File.DataX[i]/File.DataX[i];
	      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b);
	      beta[0] = beta[0] + delta_y*df1;
	      beta[1] = beta[1] + delta_y*df2;
	    }
	  // calculate Hessian matrix
	  HesMa[0] = 0;
	  HesMa[1] = 0;
	  HesMa[2] = 0;
	  HesMa[3] = 0;
	  for(i = 0; i<N; i++)
	    {
	      df1 = -1/T/File.DataX[i]/File.DataX[i];
	      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b);
	      HesMa[0] = HesMa[0] + df1*df1;
	      HesMa[1] = HesMa[1] + df1*df2;
	      HesMa[2] = HesMa[2] + df2*df1;
	      HesMa[3] = HesMa[3] + df2*df2;
	    }
	  
	  // Modify the matrix
	  HesMa_new[0] = HesMa[0] * (1 + lamda1);
	  HesMa_new[1] = HesMa[1];
	  HesMa_new[2] = HesMa[2];
	  HesMa_new[3] = HesMa[3] * (1 + lamda1);
	  // Solve linear equation
	  delta_a[0] = (beta[0]*HesMa_new[3]-beta[1]*HesMa_new[1])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  delta_a[1] = (beta[1]*HesMa_new[0]-beta[0]*HesMa_new[2])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  // Evaluate S(a+delta_a)
	  a_new = a + delta_a[0];
	  b_new = b + delta_a[1];
	  sa_new = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T/(File.DataX[i]-b_new) - a_new/(T*File.DataX[i]*File.DataX              [i]);
	      delta_y = File.DataY[i] - f;
	      sa_new = sa_new + delta_y * delta_y;
	    }
	  lamda1 = lamda;
	}
    }
  
  // print out the information of result
  cout<<"The final result is:"<<endl;
  cout<<"a = "<<a_new<<endl;
  cout<<"b = "<<b_new<<endl;
  cout<<"The chi square is "<<sa_new<<endl;
  // calculate sample variance
  sample_var = sa_new/(N-2);
  cout<<"The sample variance is "<<sample_var<<endl;
  // compute the matrix C
  temp = HesMa[0]*HesMa[3]-HesMa[1]*HesMa[2];
  C[0] = HesMa[3]/temp;
  C[1] = -HesMa[1]/temp;
  C[2] = -HesMa[2]/temp;
  C[3] = HesMa[0]/temp;
  cout<<"The matrix C is :"<<endl;
  cout<<"                 "<<C[0]<<" "<<C[1]<<endl;
  cout<<"                 "<<C[2]<<" "<<C[3]<<endl;
  // calculate standard deviation
  std_a = sqrt(C[0]*sample_var);
  std_b = sqrt(C[3]*sample_var);
  cout<<"The standard deviation for a is: "<<std_a<<endl;
  cout<<"The standard deviation for b is："<<std_b<<endl;
  // correlation coefficients
  rho = sample_var*C[1]/std_a/std_b;
  cout<<"The correlation coefficient is: "<<rho<<endl;
  // coefficient of determination R^2
  y_mean = 0;
  for(i = 0; i<N; i++)
    {
      y_mean = y_mean + File.DataY[i];
    }
  y_mean = y_mean/N;
  ss_tot = 0;
  for(i=0; i<N; i++)
    {
      ss_tot = ss_tot + (File.DataY[i] - y_mean)*(File.DataY[i] - y_mean);
    }
  RR = 1 - sa_new/ss_tot;
  cout<<"The coefficient of determination R^2 is: "<<RR<<endl;
  // adjust R^2
  ad_RR = 1 - sa_new/(N-2-1)/(ss_tot/(N-1));
  cout<<"The adjust R^2 is: "<<ad_RR<<endl;
  // R-factor
  temp_err = 0;
  temp_abs = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T/(File.DataX[i]-b_new) - a_new/(T*File.DataX[i]*File.DataX[i]);
      temp_err = temp_err + abs(File.DataY[i] - f);
      temp_abs = temp_abs + abs(File.DataY[i]);
    }
  
  R_factor = temp_err/temp_abs;
  cout<<"The R-factor is: "<<R_factor<<endl;      
}

void vdw(FileInf File){
  // p = RT/(V-b) - a/(TV^2)
  float sa = 0; // Error function
  int N = File.dataSize; // Data size
  int i,j = 0;
  float T = File.temp; // Temperature
  float f; // value of the function
  float a = File.Initial[0];
  float b = File.Initial[1];
  float beta[2];
  float a_new,b_new;
  float df1,df2;
  float delta_y;
  float HesMa[4],HesMa_new[4]; // Hessian matrix
  float C[4]; // alpha^-1
  float delta_a[2];
  float ad_RR,ss_tot,y_mean,rho,sample_var,std_a,std_b,RR;
  float temp_err,temp_abs,R_factor,temp;
  float sa_new,sa_old;
  float lamda1 = lamda/10; // step size

  // Unify the unit
  // unit is m^3/mol bar convert to m^3/mol pa
  for(i = 0; i<N; i++)
    {
      File.DataX[i] = File.DataX[i];
      File.DataY[i] = File.DataY[i]*100000;
    }

  // calculate error function
  for(i = 0; i<N; i++)
    {
      f = R*T/(File.DataX[i]-b) - a/(File.DataX[i]*File.DataX[i]);
      delta_y = File.DataY[i] - f;
      sa = sa + delta_y * delta_y;
    }
  cout<<"sa:"<<sa<<endl;
  // df/da = -1/V^2
  // df/db = R*T/(V-b)^2
  // calculate beta vector
  beta[0] = 0;
  beta[1] = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T/(File.DataX[i]-b) - a/(File.DataX[i]*File.DataX[i]);
      delta_y = File.DataY[i] - f;
      df1 = -1/File.DataX[i]/File.DataX[i];
      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b);
      beta[0] = beta[0] + delta_y*df1;
      beta[1] = beta[1] + delta_y*df2;
    }
  cout<<"beta: "<<beta[0]<<"  "<<beta[1]<<endl;
  // calculate Hessian matrix
  HesMa[0] = 0;
  HesMa[1] = 0;
  HesMa[2] = 0;
  HesMa[3] = 0;
  for(i = 0; i<N; i++)
    {
      df1 = -1/File.DataX[i]/File.DataX[i];
      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b);
      HesMa[0] = HesMa[0] + df1*df1;
      HesMa[1] = HesMa[1] + df1*df2;
      HesMa[2] = HesMa[2] + df2*df1;
      HesMa[3] = HesMa[3] + df2*df2;
    }
  for(i=0;i<4;i++)
    {
      cout<<"Hesma: "<<HesMa[i]<<endl;
    }
  // Modify the matrix
  HesMa_new[0] = HesMa[0] * (1 + lamda1);
  HesMa_new[1] = HesMa[1];
  HesMa_new[2] = HesMa[2];
  HesMa_new[3] = HesMa[3] * (1 + lamda1);

  // Solve linear equation
  delta_a[0] = (beta[0]/(1e+14)*HesMa_new[3]-beta[1]/(1e+14)*HesMa_new[1])/(HesMa_new[0]/(1e+14)*HesMa_new[3]-HesMa_new[1]/(1e+14)*HesMa_new[2]);
  delta_a[1] = (beta[1]/(1e+14)*HesMa_new[0]-beta[0]/(1e+14)*HesMa_new[2])/(HesMa_new[0]/(1e+14)*HesMa_new[3]-HesMa_new[1]/(1e+14)*HesMa_new[2]);
  cout<<"delta: "<<delta_a[0]<<endl;
  cout<<"delta: "<<delta_a[1]<<endl;
  // Evaluate S(a+delta_a)
  a_new = a + delta_a[0];
  b_new = b + delta_a[1];
  sa_new = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T/(File.DataX[i]-b_new) - a_new/(File.DataX[i]*File.DataX[i]);
      delta_y = File.DataY[i] - f;
      sa_new = sa_new + delta_y * delta_y;
    }
  sa_old = sa;

  cout<<"sa_new: "<<sa_new<<endl;
  cout<<"sa_old: "<<sa_old<<endl;
  cout<<"err: "<<abs(sa_new-sa_old)<<endl;
  // compare sa_new and sa_old
  while(abs(sa_new-sa_old)>=1e-5)
    {
      cout<<"THE STEP  "<<j<<endl;
      j++;
      cout<<"error "<<sa_new<<endl;
      cout<<"a = "<<a<<endl;
      cout<<"b = "<<b<<endl;
      if(sa_new>sa_old)
	{
	  lamda1 = 10*lamda1;
	  // Modify the matrix
	  HesMa_new[0] = HesMa[0] * (1 + lamda1);
	  HesMa_new[3] = HesMa[3] * (1 + lamda1);
	  // Solve linear equation
	  delta_a[0] = (beta[0]*HesMa_new[3]-beta[1]*HesMa_new[1])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  delta_a[1] = (beta[1]*HesMa_new[0]-beta[0]*HesMa_new[2])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  // Evaluate S(a+delta_a)
	  a_new = a + delta_a[0];
	  b_new = b + delta_a[1];
	  sa_new = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T/(File.DataX[i]-b_new) - a_new/(File.DataX[i]*File.DataX[i]);
	      delta_y = File.DataY[i] - f;
	      sa_new = sa_new + delta_y * delta_y;
	    }
	}
      else
	{
	  lamda1 = lamda1/10;
	  // use a+delta_a instead of a
	  a = a_new;
	  b = b_new;
	  sa_old = sa_new;
	  // calculate beta vector
	  beta[0] = 0;
	  beta[1] = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T/(File.DataX[i]-b) - a/(File.DataX[i]*File.DataX[i]);
	      delta_y = File.DataY[i] - f;
	      df1 = -1/File.DataX[i]/File.DataX[i];
	      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b);
	      beta[0] = beta[0] + delta_y*df1;
	      beta[1] = beta[1] + delta_y*df2;
	    }
	  // calculate Hessian matrix
	  HesMa[0] = 0;
	  HesMa[1] = 0;
	  HesMa[2] = 0;
	  HesMa[3] = 0;
	  for(i = 0; i<N; i++)
	    {
	      df1 = -1/File.DataX[i]/File.DataX[i];
	      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b);
	      HesMa[0] = HesMa[0] + df1*df1;
	      HesMa[1] = HesMa[1] + df1*df2;
	      HesMa[2] = HesMa[2] + df2*df1;
	      HesMa[3] = HesMa[3] + df2*df2;
	    }

	  // Modify the matrix
	  HesMa_new[0] = HesMa[0] * (1 + lamda1);
	  HesMa_new[1] = HesMa[1];
	  HesMa_new[2] = HesMa[2];
	  HesMa_new[3] = HesMa[3] * (1 + lamda1);
	  // Solve linear equation
	  delta_a[0] = (beta[0]*HesMa_new[3]-beta[1]*HesMa_new[1])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  delta_a[1] = (beta[1]*HesMa_new[0]-beta[0]*HesMa_new[2])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  // Evaluate S(a+delta_a)
	  a_new = a + delta_a[0];
	  b_new = b + delta_a[1];
	  sa_new = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T/(File.DataX[i]-b_new) - a_new/(File.DataX[i]*File.DataX[i]);
	      delta_y = File.DataY[i] - f;
	      sa_new = sa_new + delta_y * delta_y;
	    }
	  lamda1 = lamda;
	}
    }

  // print out the information of result
  cout<<"The final result is:"<<endl;
  cout<<"a = "<<a_new<<endl;
  cout<<"b = "<<b_new<<endl;
  cout<<"The chi square is "<<sa_new<<endl;
  // calculate sample variance
  sample_var = sa_new/(N-2);
  cout<<"The sample variance is "<<sample_var<<endl;
  // compute the matrix C
  temp = HesMa[0]*HesMa[3]-HesMa[1]*HesMa[2];
  C[0] = HesMa[3]/temp;
  C[1] = -HesMa[1]/temp;
  C[2] = -HesMa[2]/temp;
  C[3] = HesMa[0]/temp;
  cout<<"The matrix C is :"<<endl;
  cout<<"                 "<<C[0]<<" "<<C[1]<<endl;
  cout<<"                 "<<C[2]<<" "<<C[3]<<endl;
  // calculate standard deviation
  std_a = sqrt(C[0]*sample_var);
  std_b = sqrt(C[3]*sample_var);
  cout<<"The standard deviation for a is: "<<std_a<<endl;
  cout<<"The standard deviation for b is："<<std_b<<endl;
  // correlation coefficients
  rho = sample_var*C[1]/std_a/std_b;
  cout<<"The correlation coefficient is: "<<rho<<endl;
  // coefficient of determination R^2
  y_mean = 0;
  for(i = 0; i<N; i++)
    {
      y_mean = y_mean + File.DataY[i];
    }
  y_mean = y_mean/N;
  ss_tot = 0;
  for(i=0; i<N; i++)
    {
      ss_tot = ss_tot + (File.DataY[i] - y_mean)*(File.DataY[i] - y_mean);
    }
  RR = 1 - sa_new/ss_tot;
  cout<<"The coefficient of determination R^2 is: "<<RR<<endl;
  // adjust R^2
  ad_RR = 1 - sa_new/(N-2-1)/(ss_tot/(N-1));
  cout<<"The adjust R^2 is: "<<ad_RR<<endl;
  // R-factor
  temp_err = 0;
  temp_abs = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T/(File.DataX[i]-b_new) - a_new/(File.DataX[i]*File.DataX[i]);
      temp_err = temp_err + abs(File.DataY[i] - f);
      temp_abs = temp_abs + abs(File.DataY[i]);
    }

  R_factor = temp_err/temp_abs;
  cout<<"The R-factor is: "<<R_factor<<endl;
}

void dieterici(FileInf File){
  // p = RTexp(-a/R/T/V)/(V-b)
  float sa = 0; // Error function
  int N = File.dataSize; // Data size
  int i,j = 0;
  float T = File.temp; // Temperature
  float f; // value of the function
  float a = File.Initial[0];
  float b = File.Initial[1];
  float beta[2];
  float a_new,b_new;
  float df1,df2;
  float delta_y;
  float HesMa[4],HesMa_new[4]; // Hessian matrix
  float C[4]; // alpha^-1
  float delta_a[2];
  float ad_RR,ss_tot,y_mean,rho,sample_var,std_a,std_b,RR;
  float temp_err,temp_abs,R_factor,temp;
  float sa_new,sa_old;
  float lamda1 = lamda; // step size

  // Unify the unit
  // unit is dm^3/mol atm convert to m^3/mol pa
  for(i = 0; i<N; i++)
    {
      File.DataX[i] = File.DataX[i]/1000;
      File.DataY[i] = File.DataY[i]*101325;
    }

  // calculate error function
  for(i = 0; i<N; i++)
    {
      f = R*T*exp(-a/R/T/File.DataX[i])/(File.DataX[i]-b);
      delta_y = File.DataY[i] - f;
      sa = sa + delta_y * delta_y;
    }

  // df/da = -exp(-a/R/T/V)/V/(V-b)
  // df/db = R*T*exp(-a/R/T/V)/(V-b)^2
  // calculate beta vector
  beta[0] = 0;
  beta[1] = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T*exp(-a/R/T/File.DataX[i])/(File.DataX[i]-b);
      delta_y = File.DataY[i] - f;
      df1 = -exp(-a/R/T/File.DataX[i])/File.DataX[i]/(File.DataX[i]-b);
      df2 = R*T*exp(-a/R/T/File.DataX[i])/(File.DataX[i]-b)/(File.DataX[i]-b);
      beta[0] = beta[0] + delta_y*df1;
      beta[1] = beta[1] + delta_y*df2;
    }

  // calculate Hessian matrix
  HesMa[0] = 0;
  HesMa[1] = 0;
  HesMa[2] = 0;
  HesMa[3] = 0;
  for(i = 0; i<N; i++)
    {
      df1 = -exp(-a/R/T/File.DataX[i])/File.DataX[i]/(File.DataX[i]-b);
      df2 = R*T*exp(-a/R/T/File.DataX[i])/(File.DataX[i]-b)/(File.DataX[i]-b);
      HesMa[0] = HesMa[0] + df1*df1;
      HesMa[1] = HesMa[1] + df1*df2;
      HesMa[2] = HesMa[2] + df2*df1;
      HesMa[3] = HesMa[3] + df2*df2;
    }

  // Modify the matrix
  HesMa_new[0] = HesMa[0] * (1 + lamda1);
  HesMa_new[1] = HesMa[1];
  HesMa_new[2] = HesMa[2];
  HesMa_new[3] = HesMa[3] * (1 + lamda1);

  // Solve linear equation
  delta_a[0] = (beta[0]*HesMa_new[3]-beta[1]*HesMa_new[1])/(HesMa_new[0]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
  delta_a[1] = (beta[1]*HesMa_new[0]-beta[0]*HesMa_new[2])/(HesMa_new[0]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);

  // Evaluate S(a+delta_a)
  a_new = a + delta_a[0];
  b_new = b + delta_a[1];
  sa_new = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T*exp(-a_new/R/T/File.DataX[i])/(File.DataX[i]-b_new);
      delta_y = File.DataY[i] - f;
      sa_new = sa_new + delta_y * delta_y;
    }
  sa_old = sa;

  //cout<<sa_new<<endl;
  //cout<<sa_old<<endl;
  //cout<<abs(sa_new-sa_old)<<endl;
  // compare sa_new and sa_old
  while(abs(sa_new-sa_old)>=1e-5)
    {
      //cout<<"THE STEP  "<<j<<endl;
      //j++;
      //cout<<"error "<<sa_new<<endl;
      //cout<<"a = "<<a<<endl;
      //cout<<"b = "<<b<<endl;
      if(sa_new>sa_old)
	{
	  lamda1 = 10*lamda1;
	  // Modify the matrix
	  HesMa_new[0] = HesMa[0] * (1 + lamda1);
	  HesMa_new[3] = HesMa[3] * (1 + lamda1);
	  // Solve linear equation
	  delta_a[0] = (beta[0]*HesMa_new[3]-beta[1]*HesMa_new[1])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  delta_a[1] = (beta[1]*HesMa_new[0]-beta[0]*HesMa_new[2])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  // Evaluate S(a+delta_a)
	  a_new = a + delta_a[0];
	  b_new = b + delta_a[1];
	  sa_new = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T*exp(-a_new/R/T/File.DataX[i])/(File.DataX[i]-b_new);
	      delta_y = File.DataY[i] - f;
	      sa_new = sa_new + delta_y * delta_y;
	    }
	}
      else
	{
	  lamda1 = lamda1/10;
	  // use a+delta_a instead of a
	  a = a_new;
	  b = b_new;
	  sa_old = sa_new;
	  // calculate beta vector
	  beta[0] = 0;
	  beta[1] = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T*exp(-a/R/T/File.DataX[i])/(File.DataX[i]-b);
	      delta_y = File.DataY[i] - f;
	      df1 = -exp(-a/R/T/File.DataX[i])/File.DataX[i]/(File.DataX[i]-b);
	      df2 = R*T*exp(-a/R/T/File.DataX[i])/(File.DataX[i]-b)/(File.DataX              [i]-b);   
	      beta[0] = beta[0] + delta_y*df1;
	      beta[1] = beta[1] + delta_y*df2;
	    }
	  // calculate Hessian matrix
	  HesMa[0] = 0;
	  HesMa[1] = 0;
	  HesMa[2] = 0;
	  HesMa[3] = 0;
	  for(i = 0; i<N; i++)
	    {
	      df1 = -exp(-a/R/T/File.DataX[i])/File.DataX[i]/(File.DataX[i]-b);
	      df2 = R*T*exp(-a/R/T/File.DataX[i])/(File.DataX[i]-b)/(File.DataX              [i]-b);
	      HesMa[0] = HesMa[0] + df1*df1;
	      HesMa[1] = HesMa[1] + df1*df2;
	      HesMa[2] = HesMa[2] + df2*df1;
	      HesMa[3] = HesMa[3] + df2*df2;
	    }

	  // Modify the matrix
	  HesMa_new[0] = HesMa[0] * (1 + lamda1);
	  HesMa_new[1] = HesMa[1];
	  HesMa_new[2] = HesMa[2];
	  HesMa_new[3] = HesMa[3] * (1 + lamda1);
	  // Solve linear equation
	  delta_a[0] = (beta[0]*HesMa_new[3]-beta[1]*HesMa_new[1])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  delta_a[1] = (beta[1]*HesMa_new[0]-beta[0]*HesMa_new[2])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  // Evaluate S(a+delta_a)
	  a_new = a + delta_a[0];
	  b_new = b + delta_a[1];
	  sa_new = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T*exp(-a_new/R/T/File.DataX[i])/(File.DataX[i]-b_new);
	      delta_y = File.DataY[i] - f;
	      sa_new = sa_new + delta_y * delta_y;
	    }
	  lamda1 = lamda;
	}
    }

  // print out the information of result
  cout<<"The final result is:"<<endl;
  cout<<"a = "<<a_new<<endl;
  cout<<"b = "<<b_new<<endl;
  cout<<"The chi square is "<<sa_new<<endl;
  // calculate sample variance
  sample_var = sa_new/(N-2);
  cout<<"The sample variance is "<<sample_var<<endl;
  // compute the matrix C
  temp = HesMa[0]*HesMa[3]-HesMa[1]*HesMa[2];
  C[0] = HesMa[3]/temp;
  C[1] = -HesMa[1]/temp;
  C[2] = -HesMa[2]/temp;
  C[3] = HesMa[0]/temp;
  cout<<"The matrix C is :"<<endl;
  cout<<"                 "<<C[0]<<" "<<C[1]<<endl;
  cout<<"                 "<<C[2]<<" "<<C[3]<<endl;
  // calculate standard deviation
  std_a = sqrt(C[0]*sample_var);
  std_b = sqrt(C[3]*sample_var);
  cout<<"The standard deviation for a is: "<<std_a<<endl;
  cout<<"The standard deviation for b is："<<std_b<<endl;
  // correlation coefficients
  rho = sample_var*C[1]/std_a/std_b;
  cout<<"The correlation coefficient is: "<<rho<<endl;
  // coefficient of determination R^2
  y_mean = 0;
  for(i = 0; i<N; i++)
    {
      y_mean = y_mean + File.DataY[i];
    }
  y_mean = y_mean/N;
  ss_tot = 0;
  for(i=0; i<N; i++)
    {
      ss_tot = ss_tot + (File.DataY[i] - y_mean)*(File.DataY[i] - y_mean);
    }
  RR = 1 - sa_new/ss_tot;
  cout<<"The coefficient of determination R^2 is: "<<RR<<endl;
  // adjust R^2
  ad_RR = 1 - sa_new/(N-2-1)/(ss_tot/(N-1));
  cout<<"The adjust R^2 is: "<<ad_RR<<endl;
  // R-factor
  temp_err = 0;
  temp_abs = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T*exp(-a_new/R/T/File.DataX[i])/(File.DataX[i]-b_new);
      temp_err = temp_err + abs(File.DataY[i] - f);
      temp_abs = temp_abs + abs(File.DataY[i]);
    }

  R_factor = temp_err/temp_abs;
  cout<<"The R-factor is: "<<R_factor<<endl;
}

void rk(FileInf File){
  // p = RT/(V-b) - a/(T^1/2*V*(V+b))
  float sa = 0; // Error function
  int N = File.dataSize; // Data size
  int i,j = 0;
  float T = File.temp; // Temperature
  float f; // value of the function
  float a = File.Initial[0];
  float b = File.Initial[1];
  float beta[2];
  float a_new,b_new;
  float df1,df2;
  float delta_y;
  float HesMa[4],HesMa_new[4]; // Hessian matrix
  float C[4]; // alpha^-1
  float delta_a[2];
  float ad_RR,ss_tot,y_mean,rho,sample_var,std_a,std_b,RR;
  float temp_err,temp_abs,R_factor,temp;
  float sa_new,sa_old;
  float lamda1 = lamda; // step size

  // Unify the unit
  // unit is dm^3/mol megapa convert to m^3/mol pa
  for(i = 0; i<N; i++)
    {
      File.DataX[i] = File.DataX[i]/1000;
      File.DataY[i] = File.DataY[i]*1000000;
    }

  // calculate error function
  for(i = 0; i<N; i++)
    {
      f = R*T/(File.DataX[i]-b) - a/(sqrt(T)*File.DataX[i]*(File.DataX[i]+b));
      delta_y = File.DataY[i] - f;
      sa = sa + delta_y * delta_y;
    }

  // df/da = -1/T^1/2/V/(V+b)
  // df/db = R*T/(V-b)^2 + a/T^1/2/V/(V+b)^2
  // calculate beta vector
  beta[0] = 0;
  beta[1] = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T/(File.DataX[i]-b) - a/(sqrt(T)*File.DataX[i]*(File.DataX[i]+b));
      delta_y = File.DataY[i] - f;
      df1 = -1/sqrt(T)/File.DataX[i]/(File.DataX[i]+b);
      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b) + a/sqrt(T)/File.DataX[i]/(      File.DataX[i]+b)/(File.DataX[i]+b);
      beta[0] = beta[0] + delta_y*df1;
      beta[1] = beta[1] + delta_y*df2;
    }

  // calculate Hessian matrix
  HesMa[0] = 0;
  HesMa[1] = 0;
  HesMa[2] = 0;
  HesMa[3] = 0;
  for(i = 0; i<N; i++)
    {
      df1 = -1/sqrt(T)/File.DataX[i]/(File.DataX[i]+b);
      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b) + a/sqrt(T)/File.DataX[i]/(      File.DataX[i]+b)/(File.DataX[i]+b);
      HesMa[0] = HesMa[0] + df1*df1;
      HesMa[1] = HesMa[1] + df1*df2;
      HesMa[2] = HesMa[2] + df2*df1;
      HesMa[3] = HesMa[3] + df2*df2;
    }

  // Modify the matrix
  HesMa_new[0] = HesMa[0] * (1 + lamda1);
  HesMa_new[1] = HesMa[1];
  HesMa_new[2] = HesMa[2];
  HesMa_new[3] = HesMa[3] * (1 + lamda1);

  // Solve linear equation
  delta_a[0] = (beta[0]*HesMa_new[3]-beta[1]*HesMa_new[1])/(HesMa_new[0]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
  delta_a[1] = (beta[1]*HesMa_new[0]-beta[0]*HesMa_new[2])/(HesMa_new[0]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);

  // Evaluate S(a+delta_a)
  a_new = a + delta_a[0];
  b_new = b + delta_a[1];
  sa_new = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T/(File.DataX[i]-b_new) - a_new/(sqrt(T)*File.DataX[i]*(File.DataX[      i]+b_new));
      delta_y = File.DataY[i] - f;
      sa_new = sa_new + delta_y * delta_y;
    }
  sa_old = sa;

  //cout<<sa_new<<endl;
  //cout<<sa_old<<endl;
  //cout<<abs(sa_new-sa_old)<<endl;
  // compare sa_new and sa_old
  while(abs(sa_new-sa_old)>=1e-5)
    {
      //cout<<"THE STEP  "<<j<<endl;
      //j++;
      //cout<<"error "<<sa_new<<endl;
      //cout<<"a = "<<a<<endl;
      //cout<<"b = "<<b<<endl;
      if(sa_new>sa_old)
	{
	  lamda1 = 10*lamda1;
	  // Modify the matrix
	  HesMa_new[0] = HesMa[0] * (1 + lamda1);
	  HesMa_new[3] = HesMa[3] * (1 + lamda1);
	  // Solve linear equation
	  delta_a[0] = (beta[0]*HesMa_new[3]-beta[1]*HesMa_new[1])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  delta_a[1] = (beta[1]*HesMa_new[0]-beta[0]*HesMa_new[2])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  // Evaluate S(a+delta_a)
	  a_new = a + delta_a[0];
	  b_new = b + delta_a[1];
	  sa_new = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T/(File.DataX[i]-b_new) - a_new/(sqrt(T)*File.DataX[i]*(File.DataX[i]+b_new));
	      delta_y = File.DataY[i] - f;
	      sa_new = sa_new + delta_y * delta_y;
	    }
	}
      else
	{
	  lamda1 = lamda1/10;
	  // use a+delta_a instead of a
	  a = a_new;
	  b = b_new;
	  sa_old = sa_new;
	  // calculate beta vector
	  beta[0] = 0;
	  beta[1] = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T/(File.DataX[i]-b) - a/(sqrt(T)*File.DataX[i]*(File.DataX[              i]+b));
	      delta_y = File.DataY[i] - f;
	      df1 = -1/sqrt(T)/File.DataX[i]/(File.DataX[i]+b);
	      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b) + a/sqrt(T)/File.DataX[i]/(File.DataX[i]+b)/(File.DataX[i]+b);     
	      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b);
	      beta[0] = beta[0] + delta_y*df1;
	      beta[1] = beta[1] + delta_y*df2;
	    }
	  // calculate Hessian matrix
	  HesMa[0] = 0;
	  HesMa[1] = 0;
	  HesMa[2] = 0;
	  HesMa[3] = 0;
	  for(i = 0; i<N; i++)
	    {
	      df1 = -1/sqrt(T)/File.DataX[i]/(File.DataX[i]+b);
	      df2 = R*T/(File.DataX[i]-b)/(File.DataX[i]-b) + a/sqrt(T)/File.DataX[i]/(File.DataX[i]+b)/(File.DataX[i]+b);
	      HesMa[0] = HesMa[0] + df1*df1;
	      HesMa[1] = HesMa[1] + df1*df2;
	      HesMa[2] = HesMa[2] + df2*df1;
	      HesMa[3] = HesMa[3] + df2*df2;
	    }

	  // Modify the matrix
	  HesMa_new[0] = HesMa[0] * (1 + lamda1);
	  HesMa_new[1] = HesMa[1];
	  HesMa_new[2] = HesMa[2];
	  HesMa_new[3] = HesMa[3] * (1 + lamda1);
	  // Solve linear equation
	  delta_a[0] = (beta[0]*HesMa_new[3]-beta[1]*HesMa_new[1])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  delta_a[1] = (beta[1]*HesMa_new[0]-beta[0]*HesMa_new[2])/(HesMa_new[0          ]*HesMa_new[3]-HesMa_new[1]*HesMa_new[2]);
	  // Evaluate S(a+delta_a)
	  a_new = a + delta_a[0];
	  b_new = b + delta_a[1];
	  sa_new = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T/(File.DataX[i]-b_new) - a_new/(sqrt(T)*File.DataX[i]*(File.DataX[i]+b_new));    	      
	      delta_y = File.DataY[i] - f;
	      sa_new = sa_new + delta_y * delta_y;
	    }
	  lamda1 = lamda;
	}
    }

  // print out the information of result
  cout<<"The final result is:"<<endl;
  cout<<"a = "<<a_new<<endl;
  cout<<"b = "<<b_new<<endl;
  cout<<"The chi square is "<<sa_new<<endl;
  // calculate sample variance
  sample_var = sa_new/(N-2);
  cout<<"The sample variance is "<<sample_var<<endl;
  // compute the matrix C
  temp = HesMa[0]*HesMa[3]-HesMa[1]*HesMa[2];
  C[0] = HesMa[3]/temp;
  C[1] = -HesMa[1]/temp;
  C[2] = -HesMa[2]/temp;
  C[3] = HesMa[0]/temp;
  cout<<"The matrix C is :"<<endl;
  cout<<"                 "<<C[0]<<" "<<C[1]<<endl;
  cout<<"                 "<<C[2]<<" "<<C[3]<<endl;
  // calculate standard deviation
  std_a = sqrt(C[0]*sample_var);
  std_b = sqrt(C[3]*sample_var);
  cout<<"The standard deviation for a is: "<<std_a<<endl;
  cout<<"The standard deviation for b is："<<std_b<<endl;
  // correlation coefficients
  rho = sample_var*C[1]/std_a/std_b;
  cout<<"The correlation coefficient is: "<<rho<<endl;
  // coefficient of determination R^2
  y_mean = 0;
  for(i = 0; i<N; i++)
    {
      y_mean = y_mean + File.DataY[i];
    }
  y_mean = y_mean/N;
  ss_tot = 0;
  for(i=0; i<N; i++)
    {
      ss_tot = ss_tot + (File.DataY[i] - y_mean)*(File.DataY[i] - y_mean);
    }
  RR = 1 - sa_new/ss_tot;
  cout<<"The coefficient of determination R^2 is: "<<RR<<endl;
  // adjust R^2
  ad_RR = 1 - sa_new/(N-2-1)/(ss_tot/(N-1));
  cout<<"The adjust R^2 is: "<<ad_RR<<endl;
  // R-factor
  temp_err = 0;
  temp_abs = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T/(File.DataX[i]-b_new) - a_new/(sqrt(T)*File.DataX[i]*(File.DataX[      i]+b_new));
      temp_err = temp_err + abs(File.DataY[i] - f);
      temp_abs = temp_abs + abs(File.DataY[i]);
    }

  R_factor = temp_err/temp_abs;
  cout<<"The R-factor is: "<<R_factor<<endl;
}

void virial(FileInf File){
  // p = RT/V*(1+B/V+C/V^2)
  float sa = 0; // Error function
  int N = File.dataSize; // Data size
  int i,j = 0;
  float T = File.temp; // Temperature
  float f; // value of the function
  float a = File.Initial[0];
  float b = File.Initial[1];
  float beta[2];
  float a_new,b_new;
  float df1,df2;
  float delta_y;
  float HesMa[4],HesMa_new[4]; // Hessian matrix
  float C[4]; // alpha^-1
  float delta_a[2];
  float ad_RR,ss_tot,y_mean,rho,sample_var,std_a,std_b,RR;
  float temp_err,temp_abs,R_factor,temp;
  float sa_new,sa_old;
  float lamda1 = lamda; // step size

  // Unify the unit
  // unit is m^3/mol bar convert to m^3/mol pa
  for(i = 0; i<N; i++)
    {
      File.DataX[i] = File.DataX[i];
      File.DataY[i] = File.DataY[i]*100000;
    }

  // calculate error function
  for(i = 0; i<N; i++)
    {
      f = R*T/File.DataX[i]*(1+a/File.DataX[i]+b/File.DataX[i]/File.DataX[i]);
      delta_y = File.DataY[i] - f;
      sa = sa + delta_y * delta_y;
    }
  cout<<"sa:"<<sa<<endl;
  // df/da = RT/V^2
  // df/db = R*T/(V)^3
  // calculate beta vector
  beta[0] = 0;
  beta[1] = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T/File.DataX[i]*(1+a/File.DataX[i]+b/File.DataX[i]/File.DataX[i]);
      delta_y = File.DataY[i] - f;
      df1 = R*T/File.DataX[i]/File.DataX[i];
      df2 = R*T/File.DataX[i]/File.DataX[i]/File.DataX[i];
      beta[0] = beta[0] + delta_y*df1;
      beta[1] = beta[1] + delta_y*df2;
    }
  cout<<"beta"<<beta[0]<<endl;
  cout<<"beta"<<beta[1]<<endl;
  // calculate Hessian matrix
  HesMa[0] = 0;
  HesMa[1] = 0;
  HesMa[2] = 0;
  HesMa[3] = 0;
  for(i = 0; i<N; i++)
    {
      df1 = R*T/File.DataX[i]/File.DataX[i];
      df2 = R*T/File.DataX[i]/File.DataX[i]/File.DataX[i];
      HesMa[0] = HesMa[0] + df1*df1;
      HesMa[1] = HesMa[1] + df1*df2;
      HesMa[2] = HesMa[2] + df2*df1;
      HesMa[3] = HesMa[3] + df2*df2;
    }
  for(i=0;i<4;i++)
    {
      cout<<"HESMA: "<<HesMa[i]<<endl;
    }
  // Modify the matrix
  HesMa_new[0] = HesMa[0] * (1 + lamda1);
  HesMa_new[1] = HesMa[1];
  HesMa_new[2] = HesMa[2];
  HesMa_new[3] = HesMa[3] * (1 + lamda1);

  // Solve linear equation
  delta_a[0] = (beta[0]/(1e+19)*HesMa_new[3]-beta[1]/(1e+19)*HesMa_new[1])/(HesMa_new[0]/(1e+19)*HesMa_new[3]-HesMa_new[1]/(1e+19)*HesMa_new[2]);
  delta_a[1] = (beta[1]/(1e+19)*HesMa_new[0]-beta[0]/(1e+19)*HesMa_new[2])/(HesMa_new[0]/(1e+19)*HesMa_new[3]-HesMa_new[1]/(1e+19)*HesMa_new[2]);
  cout<<"delta"<<delta_a[0]<<"   "<<delta_a[1]<<endl;
  // Evaluate S(a+delta_a)
  a_new = a + delta_a[0];
  b_new = b + delta_a[1];
  sa_new = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T/File.DataX[i]*(1+a_new/File.DataX[i]+b_new/File.DataX[i]/File.DataX[i]);
      delta_y = File.DataY[i] - f;
      sa_new = sa_new + delta_y * delta_y;
    }
  sa_old = sa;

  cout<<"sa_new: "<<sa_new<<endl;
  cout<<"sa_old: "<<sa_old<<endl;
  cout<<"err:"<<abs(sa_new-sa_old)<<endl;
  // compare sa_new and sa_old
  while(abs(sa_new-sa_old)>=1e-5)
    {
      cout<<"THE STEP  "<<j<<endl;
      j++;
      cout<<"error "<<sa_new<<endl;
      cout<<"a = "<<a<<endl;
      cout<<"b = "<<b<<endl;
      if(sa_new>sa_old)
	{
	  lamda1 = 10*lamda1;
	  // Modify the matrix
	  HesMa_new[0] = HesMa[0] * (1 + lamda1);
	  HesMa_new[3] = HesMa[3] * (1 + lamda1);
	  // Solve linear equation
	  delta_a[0] = (beta[0]/(1e+19)*HesMa_new[3]-beta[1]/(1e+19)*HesMa_new[1])/(HesMa_new[0]/(1e+19)*HesMa_new[3]-HesMa_new[1]/(1e+19)*HesMa_new[2]);
	  delta_a[1] = (beta[1]/(1e+19)*HesMa_new[0]-beta[0]/(1e+19)*HesMa_new[2])/(HesMa_new[0          ]/(1e+19)*HesMa_new[3]-HesMa_new[1]/(1e+19)*HesMa_new[2]);
	  // Evaluate S(a+delta_a)
	  a_new = a + delta_a[0];
	  b_new = b + delta_a[1];
	  sa_new = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T/File.DataX[i]*(1+a_new/File.DataX[i]+b_new/File.DataX[i]/              File.DataX[i]);
	      delta_y = File.DataY[i] - f;
	      sa_new = sa_new + delta_y * delta_y;
	    }
	}
      else
	{
	  lamda1 = lamda1/10;
	  // use a+delta_a instead of a
	  a = a_new;
	  b = b_new;
	  sa_old = sa_new;
	  // calculate beta vector
	  beta[0] = 0;
	  beta[1] = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T/File.DataX[i]*(1+a/File.DataX[i]+b/File.DataX[i]/File.DataX[i]);
	      delta_y = File.DataY[i] - f;
	      df1 = R*T/File.DataX[i]/File.DataX[i];
	      df2 = R*T/File.DataX[i]/File.DataX[i]/File.DataX[i];
	      beta[0] = beta[0] + delta_y*df1;
	      beta[1] = beta[1] + delta_y*df2;
	    }
	  cout<<"beata"<<beta[0]<<"  "<<beta[1]<<endl;
	  // calculate Hessian matrix
	  HesMa[0] = 0;
	  HesMa[1] = 0;
	  HesMa[2] = 0;
	  HesMa[3] = 0;
	  for(i = 0; i<N; i++)
	    {
	      df1 = R*T/File.DataX[i]/File.DataX[i];
	      df2 = R*T/File.DataX[i]/File.DataX[i]/File.DataX[i];
	      HesMa[0] = HesMa[0] + df1*df1;
	      HesMa[1] = HesMa[1] + df1*df2;
	      HesMa[2] = HesMa[2] + df2*df1;
	      HesMa[3] = HesMa[3] + df2*df2;
	    }

	  // Modify the matrix
	  HesMa_new[0] = HesMa[0] * (1 + lamda1);
	  HesMa_new[1] = HesMa[1];
	  HesMa_new[2] = HesMa[2];
	  HesMa_new[3] = HesMa[3] * (1 + lamda1);
	  for(i=1;i<4;i++)
	    {
	      cout<<"HesMa"<<HesMa[i]<<endl;
	    }
	  // Solve linear equation
	  delta_a[0] = (beta[0]/(1e+19)*HesMa_new[3]-beta[1]/(1e+19)*HesMa_new[1])/(HesMa_new[0]/(1e+19)*HesMa_new[3]-HesMa_new[1]/(1e+19)*HesMa_new[2]);
	  delta_a[1] = (beta[1]/(1e+19)*HesMa_new[0]-beta[0]/(1e+19)*HesMa_new[2])/(HesMa_new[0]/(1e+19)*HesMa_new[3]-HesMa_new[1]/(1e+19)*HesMa_new[2]);
	  cout<<"delta"<<delta_a[0]<<"  "<<delta_a[1]<<endl;
  	  // Evaluate S(a+delta_a)
	  a_new = a + delta_a[0];
	  b_new = b + delta_a[1];
	  sa_new = 0;
	  for(i = 0; i<N; i++)
	    {
	      f = R*T/File.DataX[i]*(1+a_new/File.DataX[i]+b_new/File.DataX[i]/              File.DataX[i]);
	      delta_y = File.DataY[i] - f;
	      sa_new = sa_new + delta_y * delta_y;
	    }
	  cout<<"sa_old"<<sa_old<<endl;
	  cout<<"sa_new"<<sa_new<<endl;
	  lamda1 = lamda;
	}
    }

  // print out the information of result
  cout<<"The final result is:"<<endl;
  cout<<"a = "<<a_new<<endl;
  cout<<"b = "<<b_new<<endl;
  cout<<"The chi square is "<<sa_new<<endl;
  // calculate sample variance
  sample_var = sa_new/(N-2);
  cout<<"The sample variance is "<<sample_var<<endl;
  // compute the matrix C
  temp = HesMa[0]*HesMa[3]-HesMa[1]*HesMa[2];
  C[0] = HesMa[3]/temp;
  C[1] = -HesMa[1]/temp;
  C[2] = -HesMa[2]/temp;
  C[3] = HesMa[0]/temp;
  cout<<"The matrix C is :"<<endl;
  cout<<"                 "<<C[0]<<" "<<C[1]<<endl;
  cout<<"                 "<<C[2]<<" "<<C[3]<<endl;
  // calculate standard deviation
  std_a = sqrt(C[0]*sample_var);
  std_b = sqrt(C[3]*sample_var);
  cout<<"The standard deviation for a is: "<<std_a<<endl;
  cout<<"The standard deviation for b is："<<std_b<<endl;
  // correlation coefficients
  rho = sample_var*C[1]/std_a/std_b;
  cout<<"The correlation coefficient is: "<<rho<<endl;
  // coefficient of determination R^2
  y_mean = 0;
  for(i = 0; i<N; i++)
    {
      y_mean = y_mean + File.DataY[i];
    }
  y_mean = y_mean/N;
  ss_tot = 0;
  for(i=0; i<N; i++)
    {
      ss_tot = ss_tot + (File.DataY[i] - y_mean)*(File.DataY[i] - y_mean);
    }
  RR = 1 - sa_new/ss_tot;
  cout<<"The coefficient of determination R^2 is: "<<RR<<endl;
  // adjust R^2
  ad_RR = 1 - sa_new/(N-2-1)/(ss_tot/(N-1));
  cout<<"The adjust R^2 is: "<<ad_RR<<endl;
  // R-factor
  temp_err = 0;
  temp_abs = 0;
  for(i = 0; i<N; i++)
    {
      f = R*T/File.DataX[i]*(1+a_new/File.DataX[i]+b_new/File.DataX[i]/\
      File.DataX[i]);
      temp_err = temp_err + abs(File.DataY[i] - f);
      temp_abs = temp_abs + abs(File.DataY[i]);
    }

  R_factor = temp_err/temp_abs;
  cout<<"The R-factor is: "<<R_factor<<endl;
}

	  
	  
   
    
  
  
  
  
  

  
  

  

  
      
	
      
      
