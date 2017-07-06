reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2

set xlabel "t/s"
set ylabel "z/m"

#set xtics 1e7
#set ytics 1e7

set title "z(t)"

set output "plot_ii.png"
plot \
 "1.txt" using 1:4 title "v=(-1000,0,100)km/s" with lines lt rgb "red",\
 "2.txt" using 1:4 title "v=(-1000,0,1000)km/s" with lines lt rgb "blue",\
 "3.txt" using 1:4 title "v=(-1000,0,3000)km/s" with lines lt rgb "black"

set output "plot_1_ii.png"
plot \
 "1.txt" using 1:4 title "v=(-1000,0,100)km/s" with lines lt rgb "black"

set output "plot_2_ii.png"
plot \
 "2.txt" using 1:4 title "v=(-1000,0,1000)km/s" with lines lt rgb "black"

set output "plot_3_ii.png"
plot \
 "3.txt" using 1:4 title "v=(-1000,0,3000)km/s" with lines lt rgb "black"
 
 set output "plot_5_ii.png"
plot \
 "5.txt" using 1:4 title "v=(-1000,0,10000)km/s" with lines lt rgb "black"
 