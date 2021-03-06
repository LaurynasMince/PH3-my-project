#include "main.h"
#include "functions.h"

int main(void) {
  //Define a grid. Grid has now a constant size which can be changed by editing the code -- this could be extended to a user defined grid size
  const int grid_rows = 100;
  const int grid_cols = 100;
  const int grid_spacing = 1;

  float v[grid_rows * grid_cols] = { 0 };
  float new_v[grid_rows * grid_cols] = { 0 };

  // Boundary Conditions
  float left_plate = 2.5;
  float right_plate = -2.5;
 
  for (int row=0; row < grid_rows; row++) {
    v[row * grid_cols] = left_plate;
    v[row * grid_cols + (grid_cols - 1)] = right_plate;
    new_v[row * grid_cols] = left_plate;
    new_v[row * grid_cols + (grid_cols - 1)] = right_plate;
  }

  /*****************************************************************
   * Solution to the problem using the Finite Differece Method     *
   * without relaxation factor                                     *
   *****************************************************************/

  // First, iterate through v to find initial solution to the problem, then keep iterating to correct the previous value until the required accuracy is achieved

  /*********************************************************/
  // Parameters of a circle
  float r = 20.0f; // radius
  float a = grid_cols/2.0, b = grid_rows/2.0; // Centre of the circle
  float distance_sqrd, r_sqrd = r*r;
  /********************************************************/

  for (int row=0; row < grid_rows; row++) {
    for (int col=0; col < grid_cols; col++) {
      distance_sqrd = (col - a)*(col - a) + (row - b)*(row - b);
      if (distance_sqrd <= r_sqrd)
	v[row * grid_cols + col] = 0;
      else if (row == 0) // assuming v[(row-1)*grid_cols + col] is 0
	v[row * grid_cols + col] = (1/4.0)*(v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
      else if (row == (grid_rows - 1)) // similar to the above
	v[row * grid_cols + col] = (1/4.0)*(v[(row-1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
      else // general case
	v[row * grid_cols + col] = (1/4.0)*(v[(row-1) * grid_cols + col] + v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
    }
  }

  // Further iteration to increase accuracy
  int max_iterate = 100000;
  float err_bound = pow(10, -3);
  float relaxation = 1.5f;
  for (int k=1; k <= max_iterate; k++) {
    if (k%1000==0)
      cout << "Currently on the " << k << "th iteration" << endl;
    for (int row=0; row < grid_rows; row++) {
      for (int col=1; col < (grid_cols - 1); col++) {
	distance_sqrd = (col - a)*(col - a) + (row - b)*(row - b);
	if (distance_sqrd <= r_sqrd)
	  new_v[row * grid_cols + col] = 0;
	else if (row == 0) // assuming v[(row-1)*grid_cols + col] is 0
	  new_v[row * grid_cols + col] = (1-relaxation) + relaxation*(1/4.0)*(v[(row+1) * grid_cols + col] + new_v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
	else if (row == (grid_rows - 1)) // similar to the above
	  new_v[row * grid_cols + col] = (1-relaxation)*v[row*grid_cols + col] + relaxation*(1/4.0)*(new_v[(row-1) * grid_cols + col] + new_v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
	else // general case
	  new_v[row * grid_cols + col] = (1-relaxation)*v[row*grid_cols + col] + relaxation*(1/4.0)*(new_v[(row-1) * grid_cols + col] + v[(row+1) * grid_cols + col] + new_v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
      }
    }
    
    // Check the difference between the elements of the new matrix and the previous. Act appropriatly in case of different errors    
    float err = error_check(v, new_v, grid_rows, grid_cols, err_bound);
    cout << err << endl;
    if (err <= err_bound) {
      cout << "The accuracy achieved after " << k << "th iteration" << endl;
      break;
    }
    else if (err > err_bound && k == max_iterate) {
      cout << "Not enough iterations to achieve the required accuracy." << endl;
      cout << err << endl;
    }
    
    // Renew the values of the array v by equating it to the array new_v
    equate_matrix(v, new_v, grid_rows, grid_cols);
    
    // The end of the solution
  }
    
  // Produce a file and store the solution
  fprint_matrix(new_v, grid_rows, grid_cols);

  // Print out the potential values as well as the coordinates
  data_equipotential(new_v, grid_rows, grid_cols);

  
  // Find the electric field
  electric_field(new_v, grid_rows, grid_cols, grid_spacing);
  
  return 0;
}
