reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2

set title "Gauss-Fit"

set output "plot_fit.png"
plot \
 "profile_cmb.dat" using 1:2 title "Data" with lines lt rgb "black",\
 4251.864219*exp(-0.5*(x-657.0187302)**2/19.12991962**2) title "Fit"
