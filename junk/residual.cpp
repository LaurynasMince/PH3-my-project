// Successive over-relaxation method to achieve better precision. First we need to find the residual R(i,j)
  double temp;
  double R[Nx-1][Ny-1] = {0};
  int max_iterate = 5;
  double err_bound = pow(10, -6);
  for (int k=0; k <= max_iterate; k++) {
  for (int j=1; j<(Ny-1); j++) {
    for (int i=1; i<(Nx-1); i++) {
    R[i][j] = V[i-1][j] + V[i+1][j] + V[i][j-1] + V[i][j+1] - 4*V[i][j];
    cout << R[i][j] << endl;
       if (R[i][j] < err_bound) {
	cout << "Precision achieved at " << k << " iteration" << endl;
	break;
      }
       else if ((R[i][j] > err_bound) && (k == max_iterate)){
	 cout << "Not enough iterations to achieve the required precision" << endl;
	 exit(8);
       }
       else
	 V[i][j] += omega * R[i][j];
    }
  }
}
