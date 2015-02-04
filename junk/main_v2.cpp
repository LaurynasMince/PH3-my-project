#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

int main(void) {

  const int Nx = 230;
  const int Ny = 230;

  double V[Nx][Ny] = {0};

  double top_plate = 100.0;
  double bottom_plate = -100.0;

  for (int i=0; i<Nx; i++) {
    V[i][Ny-1] = top_plate;
    V[i][0] = bottom_plate;
  }

  double a = Nx/2, b = Ny/2;
  double r = 40;
  double distance_sqrd, r_sqrd = r*r;
  for (int j=1; j<(Ny-1); j++) {
    for (int i=1; i<(Nx-1); i++) {
      distance_sqrd = (i - a)*(i - a) + (j - b)*(j - b);
      if (distance_sqrd <= r_sqrd)
	V[i][j] = 0;
      else
	V[i][j] = (1/4.0)*(V[i-1][j] + V[i+1][j] + V[i][j-1] + V[i][j+1]);
    }
  }



  double s = 1.6;
  double temp, max_iterate = 50000, err_bound = pow(10, -6);
  for (int k=1; k<=max_iterate; k++) {
    for (int j=1; j<(Ny-1); j++) {
      for (int i=1; i<(Nx-1); i++) {
	distance_sqrd = (i - a)*(i - a) + (j - b)*(j - b);
	if (distance_sqrd <= r_sqrd)
	  V[i][j] = 0;
	else
	  V[i][j] = (1-s)*V[i][j] + (s/4.0)*(V[i-1][j] + V[i+1][j] + V[i][j-1] + V[i][j+1]);
      }
    }
  }

  ofstream output ("potential.dat");

  for (int j=Ny-1; j>=0; j--) {
    for (int i=0; i<Nx; i++) {
      output << setw(15) << V[i][j];
    }
    output << endl;
  }

  output.close();

  return 0;
}
