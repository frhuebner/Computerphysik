reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2

set yrange [-2:2]

set xzeroaxis lt -1
 
set output "plot_sweep.png"
plot \
 "out_sweep_500.txt" using 1:2 title "E = 0.5" with lines,\
 "out_sweep_1500.txt" using 1:2 title "E = 1.5" with lines,\
 "out_sweep_2500.txt" using 1:2 title "E = 2.5" with lines,\
 "out_sweep_3500.txt" using 1:2 title "E = 3.5" with lines,\
 "out_sweep_4500.txt" using 1:2 title "E = 4.5" with lines,\

 set output "plot_sweep_cut.png"
plot \
 "out_sweep_500.txt" using 1:3 title "E = 0.5" with lines,\
 "out_sweep_1500.txt" using 1:3 title "E = 1.5" with lines,\
 "out_sweep_2500.txt" using 1:3 title "E = 2.5" with lines,\
 "out_sweep_3500.txt" using 1:3 title "E = 3.5" with lines,\
 "out_sweep_4500.txt" using 1:3 title "E = 4.5" with lines,\
 
 set output "plot_sweep_high_cut.png"
plot \
 "out_sweep_15500.txt" using 1:3 title "E = 15.5" with lines,\
 "out_sweep_16500.txt" using 1:3 title "E = 16.5" with lines,\
 "out_sweep_17500.txt" using 1:3 title "E = 17.5" with lines,\
 "out_sweep_18500.txt" using 1:3 title "E = 18.5" with lines,\
 "out_sweep_19500.txt" using 1:3 title "E = 19.5" with lines,\