/***************************************************************
 * attempt to solve the problem of setting the error boundary and 
 * stopping iterating if that boundary is achieved.
 *
 * So far it looks that the accuracy does not converge as fast as
 * expected. Only a few elements are found to have a small error.
 * One of them is at the position (grid_cols - 2, grid_rows - 1) --
 * this element stops changing quite fast and stays the same. Other
 * elements were found to be on the upper and lower boundaries in the
 * middle column of the grid -- makes sense to some extent as the 
 * potential in the middle of the grid should be approximately equal
 * to zero. Apparently 1000 iterations is not enough. Up side is that
 * the solution does not diverge dramatically at this point -- need
 * need to check if this holds for even larger number of iterations.
 ******************************************************************
 *
 * Modified on 4 February 2015
 ******************************
 * I arived at the conclusion that using the relaxation factor can be very
 * inefficient and may cause bugs, errors and divergences. Therefore, I think
 * the following code can become the basis for the numerical solution of the
 * problem. I will investigate the use of the relaxation factor, and will use
 * it if it will not cause the residual to accelerate to infinity (as it used
 * to do until now).
 *****
 * TO DO:
 **** Rewrite the main code using this algorithm
 **** Apply the prewritten functions to check for errors
 **** Check if previously produced codes gave the same graphs (solutions)
 **** Investigate the use of the relaxation factor
 **** Separate the remaining code ito functions for a better use
 *
 * CODE NEEDS FURTHER INVESTIGATION...
 *
 ****************************************************************/

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>

using namespace std;

float error_check(float old_v[], float new_v[], int grid_rows, int grid_cols);
void equate_matrix(float old_v[], float new_v[], int grid_rows, int grid_cols);

int main(void) {
  
  const int grid_cols = 100;
  const int grid_rows = 100;
  const int grid_spacing = 1;

  float v[grid_rows * grid_cols] = {0};
  float new_v[grid_rows * grid_cols]= {0};

  float left_plate = 2.5;
  float right_plate = -2.5;

  // Boundary conditions
  for (int row=0; row < grid_rows; row++) {
    v[row * grid_cols] = left_plate;
    v[row * grid_cols + (grid_cols-1)] = right_plate;
    new_v[row * grid_cols] = left_plate;
    new_v[row * grid_cols + (grid_cols-1)] = right_plate;
  }

  // Solution of the problem using the Finite Difference Method

  /**********************************************************************
   * THE FIRST ITERATION OF THE SOLUTION THAT IS GOING TO BE IMPROVED   *
   **********************************************************************/
  for (int row=1; row<(grid_rows-1); row++) {
    for (int col=1; col < (grid_cols-1); col++) {
      v[row * grid_cols + col] = (1/4.0)*(v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)] + v[(row-1) * grid_cols + col] + v[(row+1) * grid_cols + col]);
    }
  }
  
  float relax = 1.9;
  float err_tol = pow(10, -3);
  int max_iterate = 10000;
  for (int k=1; k<max_iterate; k++) {
    if (k%100==0) {
      cout << "Currently in the " << k << "th iteration." << endl;
    }
    
    for (int row=0; row < grid_rows; row++) {
      for (int col=1; col < (grid_cols-1); col++) {
	if (row==0) {
	  new_v[col] = v[grid_cols + col];
	}
	else if (row == (grid_rows-1)) {
	  new_v[row*grid_cols + col] = v[(row-1)*grid_cols + col];
	}
	else {
	  new_v[row * grid_cols + col] = (1 - relax) * v[row*grid_cols + col] + (relax/4.0) * (v[row * grid_cols + (col+1)] + new_v[row * grid_cols + (col-1)] + v[(row+1) * grid_cols + col] + new_v[(row-1) * grid_cols + col]);
	}
      }
    }

    float err = error_check(v, new_v, grid_rows, grid_cols);
    if (err <= err_tol) {
      cout << "The accuracy achieved in the " << k << "th iteration" << endl;
      break;
    }
    else {
      cout << err << endl;
      equate_matrix(v, new_v, grid_rows, grid_cols);
    }
  }

  ofstream output ("potential.dat");
  for (int row=0; row < grid_rows; row++) {
    for (int col=0; col < grid_cols; col++) {
      output << new_v[row*grid_cols + col] << " ";
    }
    output << endl;
  }

  output.close();


  return 0;
}

float error_check(float old_v[], float new_v[], int grid_rows, int grid_cols) {
  
  float test_max=0, difference, relative_err;
  for (int row=0; row < grid_rows; row++) {
    for(int col=1; col < (grid_cols-1); col++) {
      difference = fabs(old_v[row * grid_cols + col] - new_v[row * grid_cols + col]);
	if (difference > test_max) {
	  test_max = difference;
	}
    }
  }
  
  return test_max;
}

void equate_matrix(float old_v[], float new_v[], int grid_rows, int grid_cols) {
  
  for (int row=0; row<grid_rows; row++) {
    for (int col=0; col<grid_cols; col++) {
      old_v[row * grid_cols + col] = new_v[row * grid_cols + col];
    }
  }
}
