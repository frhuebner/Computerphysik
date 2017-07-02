reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2

set output "plot_a_test.png"
plot \
 "a.txt" using 1:2 title "Test" with lines lt rgb "black"

set output "plot_a_test_2.png"
plot \
 "a.txt" using 2:3 title "Test" with lines lt rgb "black"