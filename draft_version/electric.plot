set term postscript
set output "ElectricField.eps"

set xrange[-1:100]
set yrange[-1:100]
set key off
set size square
set style arrow 1
plot "ElectricField.dat" using 1:2:3:4 with vectors lt 1 

