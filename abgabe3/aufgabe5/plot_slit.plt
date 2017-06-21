reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3


set ylabel "I({/Symbol q})/I_0"
set xlabel "{/Symbol q}"
set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2
set title "Intensity"
set output "plot_slit.png"
plot \
 "slit.txt" using 1:2 title "Intensity" with lines lt rgb "black"
 set output "plot_slit_cut.png"
set yrange [0:0.02]
plot \
 "slit.txt" using 1:2 title "Intensity" with lines lt rgb "black