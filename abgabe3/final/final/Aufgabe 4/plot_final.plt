reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set ylabel "t/y"
set xlabel "z"
set logscale xy
set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2
set title "Alter des Universums"
set output "plot_log_final.png"
plot \
 "b.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0.7, {/Symbol W}_m=0.5" with lines lt rgb "blue",\
  "c.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0.73, {/Symbol W}_m=0.27" with lines lt rgb "red"

  set xrange [0:20]
unset logscale xy
set output "plot_final.png"
plot \
 "b.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0.7, {/Symbol W}_m=0.5" with lines lt rgb "blue",\
  "c.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0.73, {/Symbol W}_m=0.27" with lines lt rgb "red"
