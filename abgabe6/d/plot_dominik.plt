reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2

set output "plot_dominik.png"
plot \
 "c_dominik.txt" using 1:2 title "Dominik" with lines lt rgb "black",\
 4245.641451*exp(-0.5*(x-657.0347926)**2/19.08818599**2)
