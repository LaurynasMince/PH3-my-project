#include <iostream>
#include <cstdlib>
#include <cmath>
#include <iomanip>

using namespace std;

double relaxation_fac(int ,int);

int main(void) {
 
  //Initialize and define grid size
  const int Nx = 4;
  const int Ny = 4;

  //Find the relaxation factor and check if it is between 0 and 2
  double omega = relaxation_fac(Nx,Ny);

  if (omega < 0) {
    cout << "ERROR: the relaxation factor is less than 0. Terminating..." << endl;
    exit(8);
  }
  else if (omega > 2) {
    cout << "ERROR: the relaxation factor is greater than 2. Terminating..." << endl;
    exit (8);
  }

  //Define an array to store the values of potential
  double V[Nx][Ny] = {0};

  //Set boundary conditions and use them to initialize array V
  double left_plate = 1.0;
  double right_plate = -1.0;

  for (int j=0; j<Ny; j++) {
    V[0][j] = left_plate;
    V[Nx-1][j] = right_plate;
  }

  // Loop to fill in rest of the array V 
  for (int j=1; j<(Ny-1); j++) {
    for (int i=1; i<(Nx-1); i++) {
      V[i][j] = (1/4.0)*(V[i-1][j] + V[i+1][j] + V[i][j-1] + V[i][j+1]);
    }
  }
  /*
  int max_iterate = 2;
  for (int k=1; k<=max_iterate; k++) {
    // Loop to find an array of residuals R(i,j)
    double R[Nx-2][Ny-2] = { 0 };
    for (int j=1; j<(Ny-1); j++) {
      for (int i=1; i<(Nx-1); i++) {
	R[i-1][j-1] = V[i-1][j] + V[i+1][j] + V[i][j-1] + V[i][j+1] - 4*V[i][j];
      }
    }
    
    for (int j=1; j<(Ny-1); j++) {
      for (int i=1; i<(Nx-1); i++) {
	V[i][j] = V[i][j] + omega * R[i-1][j-1];
      }
    }
  }
  */
  for (int j=Ny-1; j>=0; j--) {
    for (int i=0; i<Nx; i++) {
      cout << setw(15) << V[i][j];
    }
    cout << endl;
  }
  



  return 0;
}

// Definition of the function called relaxation_fac which finds the relaxation factor for the use in the successive over-relaxation method

double relaxation_fac(int Nx, int Ny) {
  double omega, t;
  const double pi = 3.14159;
 
  t = cos(pi/Nx) + cos(pi/Ny);
  omega = (8 - sqrt(64 - 16*t*t))/(t*t);
  return omega;
}
