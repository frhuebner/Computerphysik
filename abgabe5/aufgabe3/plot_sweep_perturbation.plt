reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2

set yrange [-2:2]



set xzeroaxis lt -1
 
set output "plot_sweep_perturbation.png"
plot \
 "out_sweep_perturbation_885.txt" using 1:2 title "E = 0.885" with lines,\
 "out_sweep_perturbation_3043.txt" using 1:2 title "E = 3.043" with lines,\
 "out_sweep_perturbation_5800.txt" using 1:2 title "E = 5.800" with lines,\
 "out_sweep_perturbation_8929.txt" using 1:2 title "E = 8.929" with lines,\
 "out_sweep_perturbation_12357.txt" using 1:2 title "E = 12.357" with lines,\
 "out_sweep_perturbation_16038.txt" using 1:2 title "E = 16.038" with lines,\

 set xrange [0:3]
 
 set output "plot_sweep_perturbation_cut.png"
plot \
 "out_sweep_perturbation_885.txt" using 1:3 title "E = 0.885" with lines,\
 "out_sweep_perturbation_3043.txt" using 1:3 title "E = 3.043" with lines,\
 "out_sweep_perturbation_5800.txt" using 1:3 title "E = 5.800" with lines,\
 "out_sweep_perturbation_8929.txt" using 1:3 title "E = 8.929" with lines,\
 "out_sweep_perturbation_12357.txt" using 1:3 title "E = 12.357" with lines,\
 "out_sweep_perturbation_16038.txt" using 1:3 title "E = 16.038" with lines,\
