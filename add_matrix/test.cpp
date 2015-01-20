#include <iostream>
#include <cstdlib>

using namespace std;

int main(void) {

  const int n = 4;

  double psi[n][n] = { 0 };

  double right_plate = -10;
  double left_plate = 10;
  double mid_potential = 0;

  for (int i=0; i<n; i++) {
    psi[i][0] = left_plate;
    psi[i][n-1] = right_plate;
  }

  //Point in the middle
  int p = n/2;

  for (int j=1; j<n-1; j++) {
    for (int i=1; i<n-1; i++) {
      psi[i][j] = (1/4) * ( psi[i+1][j] + psi[i-1][j] + psi[i][j+1] + psi[i][j-1] );
      // psi[p][p] = mid_potential;
    }
  }

  for (int j=0; j<n; j++) {
    for (int i=0; i<n; i++) {
      cout << psi[i][j] << " ";
      //   if (i == (n-1))
      //cout << endl;
    }
  }

  cout << endl;

  return 0;

}
