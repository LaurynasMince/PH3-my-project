/************************************************************************
 * This C++ source code adds two matrices together.
 *
 * Eventually this code will become a part of a class of arithmetic operations
 * of matrix.
 *
 **********
 * VERSION 1
 *
 **********
 *
 * Laurynas Mince, 18 January 2015
 ************************************************************************/

#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

int main(void) {
  
  // Size of an array -- for simplicity assuming square matrices
  const int n = 3;

  double array1[n][n] = 
    {
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9}
    };

  double array2[n][n] = 
    {
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9}
    };

  double answer[n][n] = {0};



  return 0;
}

double add_matrices(double *array1, double *array2, int n) {
   
 for (int i=0; i<n; i++) {
   for (int j=0; j<n; j++) {
     answer[i][j] = array1[i][j] + array2[i][j];
     cout << setw(5) << answer[i][j] << " ";
     if (j==n-1)
       cout << endl;
   }
 }

}
