#include <iostream>
#include <cmath>

using namespace std;

int error_check(float v[], float final_v[], int grid_rows, int grid_cols, float error_bound) {
  
  float difference;
  int counter=0;
  for (int row=0; row < grid_rows; row++) {
    for (int col=1; col < (grid_cols-1); col++) {
      difference = abs(v[row * grid_cols + col] - final_v[row * grid_cols + col]);
      if (difference <= error_bound)
	counter++;
    }
  }

  int return_value;
  if (counter == grid_rows * (grid_cols-2))
    return 0;
  else
    return 1;
}

int main() {
  return 0;
}
