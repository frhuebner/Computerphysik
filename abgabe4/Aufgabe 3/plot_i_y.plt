reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2

set xlabel "y/m"
set ylabel "z/m"

set xtics 1e7
set ytics 1e7

set size square
set xrange [-2e7:2e7]
set yrange [-2e7:2e7]

set title "Draufsicht y-z-Ebene"

set parametric
set trange [0:2*pi]

set output "plot_i_y.png"
plot \
 "3.txt" using 3:4 title "v=(-1000,0,3000)km/s" with lines lt rgb "black",\
 "2.txt" using 3:4 title "v=(-1000,0,1000)km/s" with lines lt rgb "green",\
 "1.txt" using 3:4 title "v=(-1000,0,100)km/s" with lines lt rgb "red",\
 6370000*cos(t),6370000*sin(t) title "Earth" lt rgb "blue"

set output "plot_1_i_y.png"
plot \
 "1.txt" using 3:4 title "v=(-1000,0,100)km/s" with lines lt rgb "red",\
 6370000*cos(t),6370000*sin(t) title "Earth"

set output "plot_2_i_y.png"
plot \
 "2.txt" using 3:4 title "v=(-1000,0,1000)km/s" with lines lt rgb "red",\
 6370000*cos(t),6370000*sin(t) title "Earth"

set output "plot_3_i_y.png"
plot \
 "3.txt" using 3:4 title "v=(-1000,0,3000)km/s" with lines lt rgb "red",\
 6370000*cos(t),6370000*sin(t) title "Earth" 
 
 set output "plot_4_i_y.png"
plot \
 "4.txt" using 3:4 title "v=(-1000,0,10000)km/s" with lines lt rgb "red",\
 6370000*cos(t),6370000*sin(t) title "Earth"