#include "main.h"

int error_check(float old_v[], float new_v[], int grid_rows, int grid_cols, float error_bound) {

  float test_max, difference, relative_err;
  int x,y;
  // The following loop find a relative error between the two arrays that are provided. Then if the error corresponds to the very first element of an array (0,0) the variable test_max (i.e. max error) is initialised with this value. If we are not checking the first elements of an array, then the relative error is compared to the previously initialised test value as the first element must have been accessed before. This way the largest relative error is found.
  for (int row=0; row < grid_rows; row++) {
    for(int col=0; col < grid_cols; col++) {
      difference = abs(old_v[row * grid_cols + col]) - abs(new_v[row * grid_cols + col]);
      //relative_err = difference / new_v[row * grid_cols + col];
      if (col == 0 && row == 0) {
	//test_max = relative_err;
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

  cout << "x " << x << " y " << y << " " << test_max << endl;

  // Since we know the largest relative error by now, it can be compared to the error bound.
  if (test_max > error_bound)
    return 1;
  else if (test_max <= error_bound)
    return 0;
  else {
    cout << "ERROR!!!\n " << test_max << endl;
    return -1;
  }
}

/*
int main() {
  float old_v[3*3] = {1.1, 1.2, 1.3, 2.1, 2.5, 2.3, 3.1, 3.2, 3.3};
  float new_v[3*3] = {1.2, 1.3, 1.4, 2.2, 2.3, 2.4, 3.2, 3.3, 3.4};

  int a=error_check(old_v, new_v, 3, 3, 0.1);
  if (a == 1)
    cout << "Darn" << endl;
  else if (a == 0)
    cout << "Yipy" << endl;
  else
    cout << "meh" << endl;

  return 0;
}
*/


