reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set ylabel "log t/y"
set xlabel "log z"
set logscale xy
set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2
set title "Alter des Universums"
set output "plot_log.png"
plot \
# "real_log.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0.685, {/Symbol W}_m=0.315" with lines lt rgb "black",\
# "de_sitter_log.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0, {/Symbol W}_m=1" with lines lt rgb "red",\
 "b.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0.7, {/Symbol W}_m=0.5" with lines lt rgb "blue",\
  "c.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0.73, {/Symbol W}_m=0.27" with lines lt rgb "green"
  
  set output "plot_test_log.png"
plot \
 "real_log.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0.685, {/Symbol W}_m=0.315" with lines lt rgb "black",\
 "de_sitter_log.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0, {/Symbol W}_m=1" with lines lt rgb "red",\
 "b_log.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0.7, {/Symbol W}_m=0.5" with lines lt rgb "blue",\
  "c_log.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0.73, {/Symbol W}_m=0.27" with lines lt rgb "green",\
   "test1_log.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0, {/Symbol W}_m=0" with lines lt rgb "orange",\
     "test2_log.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0, {/Symbol W}_m=5" with lines lt rgb "gray",\
	  "test3_log.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=0.7, {/Symbol W}_m=0.29" with lines lt rgb "#AFDE08",\
	  "test4_log.txt" using 1:2 title "{/Symbol W}_{/Symbol L}=1000, {/Symbol W}_m=0" with lines lt rgb "#AF08DE"