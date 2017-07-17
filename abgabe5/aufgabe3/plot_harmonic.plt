reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2

set logscale y

set output "plot_ground_log.png"
plot \
 "out_500.txt" using 1:(abs($2)) title "E = 500" with lines,\
 "out_501.txt" using 1:(abs($2)) title "E = 501" with lines,\
 "out_499.txt" using 1:(abs($2)) title "E = 499" with lines,\
 exp(-x*x/2)

unset logscale

set yrange [-2:2]

set output "plot_ground.png"
plot \
 "out_500.txt" using 1:2 title "E = 500" with lines,\
 "out_501.txt" using 1:2 title "E = 501" with lines,\
 "out_499.txt" using 1:2 title "E = 499" with lines,\
 exp(-x*x/2)

set output "plot_higher_odd.png"
plot \
 "out_1500.txt" using 1:2 title "E = 1500" with lines,\
 "out_3500.txt" using 1:2 title "E = 3500" with lines,\
 "out_19500.txt" using 1:2 title "E = 19500" with lines,\
 exp(-x*x/2)
 
set output "plot_higher.png"
plot \
 "out_500.txt" using 1:2 title "E = 500" with lines,\
 "out_2500.txt" using 1:2 title "E = 2500" with lines,\
 "out_4500.txt" using 1:2 title "E = 4500" with lines,\
 "out_6500.txt" using 1:2 title "E = 6500" with lines,\
 "out_8500.txt" using 1:2 title "E = 8500" with lines,\
 "out_10500.txt" using 1:2 title "E = 10500" with lines,\
 "out_16500.txt" using 1:2 title "E = 16500" with lines,\
 "out_20500.txt" using 1:2 title "E = 20500" with lines,\
 exp(-x*x/2)
