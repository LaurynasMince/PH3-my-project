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
  for (int row=0; row < grid_rows; row++) {
    for (int col=1; col < grid_cols-1; col++) {
      if (row == 0) // assuming v[(row-1)*grid_cols + col] is 0
	v[row * grid_cols + col] = (1/4.0)*(v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
      else if (row == (grid_rows - 1))
	v[row * grid_cols + col] = (1/4.0)*(v[(row-1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
      else
	v[row * grid_cols + col] = (1/4.0)*(v[(row-1) * grid_cols + col] + v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
    }
  }

  // HERE I AM USING A SIMPLE SOR METHOD WITHOUT RELAXATION -- SEEMS TO PRODUCE MORE SENSIBLE RESULTS WITHOUT LARGE DIVERGENCES. IT IS VERY POSSIBLE THAT THIS WAY TAKES MORE ITERATIONS TO ARRIVE AT A GOOD APPROXIMATION, BUT IF THAT IS THE COST OF GETTING MORE ACCURATE SOLUTION, WHEN WHY NOT???
  // !!!!!!!!!!!!!!! NOTE TO MYSELF: INVESTIGATE THE EFFECTS OF DIFFERENT RELAXATION FACTORS!!!!!!!!!!
  int max_iterate = 100000;
  float err_bound = pow(10, -6);
  for (int k=1; k <= max_iterate; k++) {
    for (int row=0; row < grid_rows; row++) {
      for (int col=1; col < grid_cols-1; col++) {
	if (row == 0) // assuming v[(row-1)*grid_cols + col] is 0
	  new_v[row * grid_cols + col] = (1/4.0)*(v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
	else if (row == (grid_rows - 1))
	  new_v[row * grid_cols + col] = (1/4.0)*(v[(row-1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
	else
	  new_v[row * grid_cols + col] = (1/4.0)*(v[(row-1) * grid_cols + col] + v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
      }
    }


    // Comparing the previous and new arrays to find the largest error (difference between the two corresonding elements in two arrays)
    
    float old_element, new_element, difference, relative_err;
    float test_max, x, y;
    for (int row=0; row < grid_rows; row++) {
      for (int col=0; col < grid_cols; col++) {
	old_element = v[row * grid_rows + col];
	new_element = new_v[row * grid_rows + col];
	
	difference = abs(old_element - new_element);
	//	cout << difference << " " << row << " " << col << endl;
	if (col == 0 && row == 0) {
	  test_max = difference;
	  x = col;
	  y = row;
	}
	else {
	  if (difference > test_max) {
	    test_max = difference;
	    x = col;
	    y = row;
	  }
	}
      }
    }
    if (test_max <= err_bound) {
      cout << "Accuracy achieved in the " << k << " iteration." << endl;
      cout << test_max << endl;

      /**********************************************************/
      // Printing out the array to see if the code works
      /*
      for (int nrow=0; nrow < grid_rows; nrow++) {
	for (int ncol=0; ncol < grid_cols; ncol++) {
	  cout << new_v[nrow * grid_cols + ncol] << " ";
	}
	cout << endl;
      }
      
      cout << endl;
      
      for (int nrow=0; nrow < grid_rows; nrow++) {
	for (int ncol=0; ncol < grid_cols; ncol++) {
	  cout << new_v[nrow * grid_cols + ncol] << " ";
	}
	cout << endl;
      }
      */
      /************************************************************/
      break;
    }
  
    
  /*   
    if (k==max_iterate) {
      
      // Printing out the array to see if the code works
      for (int row=0; row < grid_rows; row++) {
	for (int col=0; col < grid_cols; col++) {
	  cout << new_v[row * grid_cols + col] << " ";
	}
	cout << endl;
      }
      
      cout << endl;
      
      for (int row=0; row < grid_rows; row++) {
	for (int col=0; col < grid_cols; col++) {
	  cout << new_v[row * grid_cols + col] << " ";
	}
	cout << endl;
      }
    } 
  */  

    // Renew values in the array v by equating it to the new_v array so that the following iterations would find the difference between the elements of the adjecent matrices -- hopefully this will show a reduction in residual
    for (int row=0; row < grid_rows; row++) {
      for (int col=0; col < grid_cols; col++) {
	v[row * grid_cols + col] = new_v[row * grid_cols + col];
      }
    }   
  }

  ofstream output ("potential.dat");
  for (int row=0; row < grid_rows; row++) {
    for (int col=0; col < grid_cols; col++) {
      output << new_v[row * grid_cols + col] << " ";
    }
    output << endl;
  }

  output.close();

  return 0;
}
