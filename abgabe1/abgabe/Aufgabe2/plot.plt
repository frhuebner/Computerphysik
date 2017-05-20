reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set title "Mittlere Entweichzeit"
set ylabel "t_{ges}/s"
set xlabel "R_{rad}/m"
set format y "10^{%T}"
set format x "10^{%T}"
set logscale xy
set xrange [0.005:0.5]
set key right bottom
set term pngcairo size 1280,960 fontscale 2 linewidth 2
set output "plot.png"
plot \
 "output.txt" using 1:2 title "Mittlere Entweichzeit" pointtype 1 lt rgb "black",\
1.04285e-005*(x**1.99283) title "1.04285e-5*x^{1.99283}" lt rgb "black"