reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2

set title "Draufsicht x-z-Ebene"

set output "plot_1_i.png"
plot \
 "1.txt" using 2:4 title "v=(-1000,0,100)km/s" with lines lt rgb "red",\
 sqrt(6370**2-x**2) notitle 