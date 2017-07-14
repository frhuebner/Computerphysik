reset

set encoding utf8

set bars 0.5
show bars
set pointsize 3

set key right top
set term pngcairo size 1280,960 fontscale 2 linewidth 2

set logscale y

set output "plot_ctest.png"
plot \
 "ctest.txt" using 1:2 title "Test" with lines lt rgb "black",\
 exp(x*x)

set output "plot_ctest_h.png"
plot \
 "ctest.txt" using 1:4 title "Test" with lines lt rgb "black"
 
set output "plot_ctest_error.png"
plot \
 "ctest.txt" using 1:(($2)-exp(($1)**2)) title "Test" with lines lt rgb "black" 

set output "plot_ctest_error_rel.png"
plot \
 "ctest.txt" using 1:((($2)/exp(($1)**2))-1) title "Test" with lines lt rgb "black" 
 
 unset logscale
 
set output "plot_ctest1.png"
plot \
 "ctest1.txt" using 1:2 title "Test" with lines lt rgb "black",\
 cos(x)
 
  
set output "plot_ctest1_circ.png"
plot \
 "ctest1.txt" using 2:3 title "Test" with lines lt rgb "black",\
 
set logscale y
set output "plot_ctest1_error.png"
plot \
 "ctest1.txt" using 1:(($2)-cos(($1))) title "Test" with lines lt rgb "black"  

 set output "plot_ctest1_error_rel.png"
plot \
 "ctest1.txt" using 1:((($2)/cos(($1)))-1) title "Test" with lines lt rgb "black"  

 unset logscale
 
 set output "plot_ctest1_h.png"
plot \
 "ctest1.txt" using 1:5 title "Test" with lines lt rgb "black"
