#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <fstream>

using namespace std;

int main(void) {

  // Define a grid.
  // NOTE: The code could be extended by making a varying grid.
  const int grid_rows = 100;
  const int grid_cols = 100;
  // NOTE(Laurynas): Do we want this to be the case all the time?
  const int grid_spacing = 1;

  // Initialise the grid to 0
  float v[grid_rows * grid_cols] = {0};

  // Boundary conditions
  float left_plate = 2.5;
  float right_plate = -2.5;

  for (int row=0; row < grid_rows; row++) {
    // Initial conditions for a capacitor
    // NOTE(Laurynas): I did not initialize a circle at this point since all the elements, except from the boundaries, are 0
    v[row * grid_cols] = left_plate;
    v[row * grid_cols + (grid_cols - 1)] = right_plate;
  }

  // Find the mid-point in the grid
  float a = grid_cols/2, b = grid_rows/2;
  // The radius of a circle
  // NOTE(Laurynas): We will probably have to generalise the code to accept some other geometrical figures as well, and will need more user defined parameteres of those figures
  float r = 20.0;
  // Predefining variables referring to the distance from the centre of a circle squared and the radius squared to find out if an element is within a circle or outside
  float distance_sqrd, r_sqrd = r*r;
  for (int row=0; row < grid_rows; row++) {
    for (int col=1; col < (grid_cols-1); col++) {
      distance_sqrd = (col - a)*(col - a) + (row - b)*(row - b);
      if (distance_sqrd <= r_sqrd)
	v[row * grid_cols + col] = 0;
      // I have set additional conditions if we are on the edge of our array (other than the boundary with a positive/negative plate). If that is the case, then the value is approximated by taking average of three surounding points
      else if (row == 0)
	v[row * grid_cols + col] = (1/4.0)*(v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
      else if (row == (grid_rows - 1))
	v[row * grid_cols + col] = (1/4.0)*(v[(row-1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
      else
	v[row * grid_cols + col] = (1/4.0)*(v[(row-1) * grid_cols + col] + v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
    }
  }

  // Predifining the relaxation factor s
  // NOTE(Laurynas): there is an equation to calculate this factor for different grids, however it is not a general formula -- it is not valid for all the grids. In general this factor is found by trial and error, and it is between 0 and 2. DO WE WANT TO USE AN EQUATION, OR KEEP THE SAME GRID?

  float s = 1.939;
  int max_iterate = 50000;
  float err_bound = pow(10, -6);

  // Improving the accuracy of the solution by applying the successive over/under relaxation method
  // NOTE(Laurynas): Need to extend to incorporate error bound
  for (int k=1; k <= max_iterate; k++) {
    for (int row=0; row < grid_rows; row++) {
      for (int col=1; col < (grid_cols-1); col++) {
	// from the equation of a circle
	distance_sqrd = (col - a)*(col - a) + (row - b)*(row - b);
	// checking if a grid element is inside a circle
	if (distance_sqrd <= r_sqrd)
	  v[row * grid_cols + col] = 0;
	// If the point is on the edge, find its value by consider three, instead of four, points as before.
	else if (row == 0)
	  v[row * grid_cols + col] = (1-s)*v[row * grid_cols + col] + (s/4.0)*(v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
	else if (row == (grid_rows - 1))
	  v[row * grid_cols + col] = (1-s)*v[row * grid_cols + col] + (s/4.0)*(v[(row-1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
	else
	  v[row * grid_cols + col] = (1-s)*v[row * grid_cols + col] + (s/4.0)*(v[(row-1) * grid_cols + col] + v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
      }
    }
  }

  /*****************************************************************/

  /*calculation of the elements in the ELECTRIC FIELD*/

  double Ex[(grid_rows-1) * (grid_cols-1)] = {0};
  double Ey[(grid_rows-1) * (grid_cols-1)] = {0};
  double Ex_prime[(grid_rows-1) * (grid_cols-1)] = {0};
  double Ey_prime[(grid_rows-1) * (grid_cols-1)] = {0};


  //These for statements define the stagered electric field samples
  for (int row=0; row < (grid_rows-1); row++){
    for (int col=0; col < (grid_cols-1); col++){
      // LAURYNAS: I THINK MISTAKE WAS HERE -- Ex COMPONENT SHOULD HAVE BEEN FOUND BY TAKING AVERAGE BETWEEN COL
      Ex[row * grid_cols + col]= (v[row * grid_cols + col] - v[row * grid_cols + (col+1)])/grid_spacing;
      Ey[row * grid_cols + col]= (v[row * grid_cols + col] - v[(row+1) * grid_cols + col])/grid_spacing;
    }
  }

  //This is a fix for the previous electric fields to get a better result
for (int row=0; row < (grid_rows-1); row++){
    for (int col=0; col < (grid_cols-1); col++){
      Ex_prime[row * grid_cols + col]= 0.5 *(Ex[(row+1) * grid_cols + col] + Ex[row * grid_cols + col]);
      Ey_prime[row * grid_cols + col]= 0.5 * (Ey[row * grid_cols + (col+1)] + Ey[row * grid_cols + col]);
    }
 }
 
// outputting the data to file for plotting the ELECTRIC FIELD
  ofstream output3 ("ElectricField.dat");

  // Note the reversed order of outputting rows
  // Incrementing in steps of 3 to reduce the amount of vectors the plot -- ??? REDUCE THE SIZE OF ARRAYS THREE TIMES ????
  for (int row=0; row < grid_rows-1; row += 3 ) {
    for (int col=0; col<grid_cols-1; col += 3) {

      double x = Ex_prime[row * grid_cols + col];
      double y = Ey_prime[row * grid_cols + col];

      //this is used to normalize the vector(This was Marks idea so that the 
      //electric field would look better)
      double NormVector = sqrt(x*x + y*y);
      // LAURYNAS: SWAPPED "ROW" AND "COL" IN THE FIRST TWO OUTPUTS -- CURRENT PLOT SEEMS CORRECT. I tend to think that col refers to x-axis and row to y-axis.
      output3  << col << " "<< row << " "<< x/NormVector << " " << y/NormVector << endl;// " " << y/NormVector <<endl;

    }
    output3 << endl;
  }

  output3.close();

  // file output to record the solution and plot it later
  ofstream output ("potential.dat");
  ofstream output2 ("matrix_potential.dat");

  // Note the reversed order of outputting rows
  for (int row=grid_rows-1; row >= 0; row--) {
    for (int col=0; col<grid_cols; col++) {
      output << col << " " << row << " " << v[row * grid_cols + col] << endl;
      output2 << v[row * grid_cols + col] << " ";
    }
    output << endl;
    output2 << endl;
  }

  output.close();
  output2.close();
  return 0;
}
