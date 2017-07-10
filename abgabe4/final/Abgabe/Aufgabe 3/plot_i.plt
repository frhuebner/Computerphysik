reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2

set xlabel "x/m"
set ylabel "z/m"

set xtics 1e7
set ytics 1e7

set size square
set xrange [-2e7:2e7]
set yrange [-2e7:2e7]

set title "Draufsicht x-z-Ebene"

set parametric
set trange [0:2*pi]

set output "plot_i.png"
plot \
 "3.txt" using 2:4 title "v=(-1000,0,3000)km/s" with lines lt rgb "black",\
 "2.txt" using 2:4 title "v=(-1000,0,1000)km/s" with lines lt rgb "green",\
 "1.txt" using 2:4 title "v=(-1000,0,100)km/s" with lines lt rgb "red",\
 6370000*cos(t),6370000*sin(t) title "Earth" lt rgb "blue"

set output "plot_1_i.png"
plot \
 "1.txt" using 2:4 title "v=(-1000,0,100)km/s" with lines lt rgb "red",\
 6370000*cos(t),6370000*sin(t) title "Earth"

set output "plot_2_i.png"
plot \
 "2.txt" using 2:4 title "v=(-1000,0,1000)km/s" with lines lt rgb "red",\
 6370000*cos(t),6370000*sin(t) title "Earth"

set output "plot_3_i.png"
plot \
 "3.txt" using 2:4 title "v=(-1000,0,3000)km/s" with lines lt rgb "red",\
 6370000*cos(t),6370000*sin(t) title "Earth" 
 
 set output "plot_5_i.png"
plot \
 "5.txt" using 2:4 title "v=(-1000,0,10000)km/s" with lines lt rgb "red",\
 6370000*cos(t),6370000*sin(t) title "Earth"