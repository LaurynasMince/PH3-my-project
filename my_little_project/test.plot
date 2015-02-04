set autoscale

unset surface
set contour base
set cntrparam order 4
set cntrparam linear
set cntrparam levels auto 26

splot "potential.dat" w l

set table "equipo.tmp"
replot
unset table
reset

plot "equipo.tmp"

set isosam 31, 31
set pm3d map 
set table "efield.tmp"
splot "ElectricField.dat"
unset table

unset autoscale

set xr [0:99]
set yr [0:99]
set isosam 31,31
set key off
plot "efield.tmp" u 1:2:3:4 w vec, "equipo.tmp" w l
reset