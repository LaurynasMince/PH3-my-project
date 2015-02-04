#include <iostream>
#include <cmath>
#include <cstdlib>
#include <fstream>

using namespace std;

int main(void) {
  
  const int grid_cols = 10;
  const int grid_rows = 10;
  const int grid_spacing = 1;

  float v[grid_rows * grid_cols] = {0};
  float new_v[grid_rows * grid_cols]= {0};

  float left_plate = 2.5;
  float right_plate = -2.5;

  for (int row=0; row < grid_rows; row++) {
    v[row * grid_cols] = left_plate;
    v[row * grid_cols + (grid_cols-1)] = right_plate;
    new_v[row * grid_cols] = left_plate;
    new_v[row * grid_cols + (grid_cols-1)] = right_plate;
  }

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

  /***************************************************************/
   for (int row=0; row < grid_rows; row++) {
    for (int col=0; col < grid_cols; col++) {
      cout << v[row * grid_cols + col] << " ";
    }
    cout << endl;
  }
  cout << endl << endl;
  /***************************************************************/

  float s=1.49029f;
  int max_iterate = 10;
  for (int k=1; k <= max_iterate; k++) {
    for (int row=0; row < grid_rows; row++) {
      for (int col=1; col < grid_cols-1; col++) {
     	if (row == 0) {
	  new_v[row * grid_cols + col] = (1-s)*v[row * grid_cols + col] + (s/4.0)*(v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
	}
	else if (row == (grid_rows - 1)) {
	  new_v[row * grid_cols + col] = (1-s)*v[row * grid_cols + col] + (s/4.0)*(v[(row-1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
	  if (row == 9 && col == 8)
	    cout << "yes " << new_v[row *grid_cols +col] << endl;
	}
	else {
	  new_v[row * grid_cols + col] = (1-s)*v[row * grid_cols + col] + (s/4.0)*(v[(row-1) * grid_cols + col] + v[(row+1) * grid_cols + col] + v[row * grid_cols + (col-1)] + v[row * grid_cols + (col+1)]);
	}
      }
    }
    // Comparing the previous and new arrays to find the largest error (difference between the two corresonding elements in two arrays)
    
    float old_element, new_element, difference, relative_err;
    // float a = abs(v[1]), b = abs(new_v[1]);
    // float max_err = abs(a - b) / b;
    float max_err;
    for (int row=0; row < grid_rows; row++) {
      for (int col=1; col < grid_cols-1; col++) {
	old_element = v[row * grid_rows + col];
	new_element = new_v[row * grid_rows + col];

	difference = abs(old_element - new_element);
	if (difference == 0) {
	  cout << difference << " " << row << " " << col << endl;
	}
      }
    }
	/*
	relative_err = difference / new_element;
	if (row == 0 && col == 1) {
	  cout << "At the first element" << endl;
	  max_err = relative_err;
	}
	else {
	  if (max_err < relative_err)
	    max_err = relative_err;
	*/

  }

  // Printing out the array to see if the code works
  for (int row=0; row < grid_rows; row++) {
    for (int col=0; col < grid_cols; col++) {
      cout << new_v[row * grid_cols + col] << " ";
    }
    cout << endl;
  }

  return 0;
}
