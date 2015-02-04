set term postscript
set output "./plots/NumericalSol.eps"

set title "Numerical Solution of the Problem A"
set nokey
set pm3d map
set size square
set xrange[0:99]
set yrange[0:99]
set palette defined ( 0 0 0 0, 0.25 0 0 1, 0.5 0 1 0,\
     0.75 1 0 0, 1 1 1 1 )

splot "NumericalSol.txt" matrix